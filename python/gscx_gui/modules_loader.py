import os
import ctypes
from typing import Callable
import struct
import tempfile
import shutil
from ctypes import CFUNCTYPE, WINFUNCTYPE, c_char_p, c_void_p, c_bool

MODULES = [
    "gscx_cpu_cell",
    "gscx_gpu_rsx",
    "gscx_recovery",
]

ENTRY_GETINFO = b"GSCX_GetModuleInfo"
ENTRY_INIT = b"GSCX_Initialize"
ENTRY_SHUT = b"GSCX_Shutdown"

# Em host_services_c.h, GSCX_CALL = __stdcall no Windows
LOG_FN = WINFUNCTYPE(None, c_char_p) if hasattr(ctypes, 'WINFUNCTYPE') else CFUNCTYPE(None, c_char_p)

class HostServices(ctypes.Structure):
    _fields_ = [
        ("log_info", LOG_FN),
        ("log_warn", LOG_FN),
        ("log_error", LOG_FN),
    ]

# Callbacks que encaminham para a GUI/console
INFO_CB = LOG_FN(lambda msg: print(msg.decode('utf-8'), end=''))
WARN_CB = LOG_FN(lambda msg: print(msg.decode('utf-8'), end=''))
ERR_CB  = LOG_FN(lambda msg: print(msg.decode('utf-8'), end=''))

class ModulesLoader:
    def __init__(self, on_log: Callable[[str], None]):
        self.on_log = on_log
        self.loaded = []
        self._temp_dir = None

    def _log(self, msg: str):
        if self.on_log:
            self.on_log(msg)

    def _host_services(self) -> HostServices:
        return HostServices(INFO_CB, WARN_CB, ERR_CB)

    def _load_from_dirs(self, dirs):
        dll_names = [m + ".dll" for m in MODULES]
        for d in dirs:
            for dll in dll_names:
                path = os.path.join(d, dll)
                if not os.path.isfile(path):
                    continue
                try:
                    lib = ctypes.WinDLL(path)
                    self._log(f"Carregado: {path}")
                    # Opcional: invocar GSCX_GetModuleInfo
                    try:
                        get_info = getattr(lib, ENTRY_GETINFO.decode())
                        get_info.restype = None
                    except AttributeError:
                        self._log(f"Entrypoint {ENTRY_GETINFO.decode()} ausente em {dll}")
                    # Inicializar
                    try:
                        init = getattr(lib, ENTRY_INIT.decode())
                        init.argtypes = [c_void_p]
                        init.restype = c_bool
                        hs = self._host_services()
                        ok = init(ctypes.byref(hs))
                        self._log(f"Initialize retornou: {ok}")
                    except AttributeError:
                        self._log(f"Entrypoint {ENTRY_INIT.decode()} ausente em {dll}")
                    self.loaded.append((dll, lib))
                except OSError as e:
                    self._log(f"Falha ao carregar {path}: {e}")

    def load_default_modules(self):
        # Tenta localizar DLLs em ./build/bin ou ./bin
        candidates = [
            os.path.abspath(os.path.join(os.getcwd(), "build")),
            os.path.abspath(os.path.join(os.getcwd(), "cpp", "build")),
            os.path.abspath(os.path.join(os.getcwd(), "bin")),
        ]
        found_dirs = [d for d in candidates if os.path.isdir(d)]
        if not found_dirs:
            self._log("Diretórios de build não encontrados. Compile as DLLs primeiro.")
            return
        self._load_from_dirs(found_dirs)

    def load_from_gscore(self, bundle_path: str):
        # Parser simples do formato GSCore (.gscb)
        try:
            with open(bundle_path, 'rb') as f:
                data = f.read()
        except Exception as e:
            self._log(f"Falha ao abrir bundle: {e}")
            return
        # Header: magic(4) ver(2) count(2)
        if len(data) < 8:
            self._log("Bundle inválido (tamanho menor que cabeçalho)")
            return
        magic, ver, count = struct.unpack_from('<IHH', data, 0)
        if magic != 0x47534352:
            self._log("Magic inválido no GSCore (esperado 'GSCR')")
            return
        off = 8
        entries = []
        try:
            for _ in range(count):
                type_, name_len = struct.unpack_from('<HH', data, off); off += 4
                name = data[off:off+name_len].decode('utf-8'); off += name_len
                payload_off, size = struct.unpack_from('<II', data, off); off += 8
                entries.append((type_, name, payload_off, size))
        except Exception as e:
            self._log(f"Erro lendo tabela de entradas: {e}")
            return
        # Extrair para pasta temporária
        if self._temp_dir:
            try:
                shutil.rmtree(self._temp_dir, ignore_errors=True)
            except Exception:
                pass
        self._temp_dir = tempfile.mkdtemp(prefix='gscx_bundle_')
        for type_, name, payload_off, size in entries:
            blob = data[payload_off:payload_off+size]
            out_name = name
            out_path = os.path.join(self._temp_dir, out_name)
            try:
                # garantir subpastas se nome contiver '/'
                os.makedirs(os.path.dirname(out_path), exist_ok=True)
                with open(out_path, 'wb') as wf:
                    wf.write(blob)
                self._log(f"Extraído: {out_name} ({size} bytes)")
            except Exception as e:
                self._log(f"Falha ao extrair {out_name}: {e}")
        # Carregar módulos a partir da pasta extraída
        self._load_from_dirs([self._temp_dir])

    def boot_recovery(self):
        # Procura a DLL de recovery carregada e chama a entry
        for name, lib in self.loaded:
            if name.startswith('gscx_recovery'):
                try:
                    entry = getattr(lib, 'GSCX_RecoveryEntry')
                    entry.restype = None
                    entry.argtypes = []
                    self._log("Chamando GSCX_RecoveryEntry...")
                    entry()
                    self._log("Recovery finalizado.")
                except AttributeError:
                    self._log("GSCX_RecoveryEntry ausente no módulo de recovery.")
                break

    def unload_all(self):
        for name, lib in self.loaded:
            try:
                shut = getattr(lib, ENTRY_SHUT.decode())
                shut()
            except Exception:
                pass
            self._log(f"Descarregado: {name}")
        self.loaded.clear()
        if self._temp_dir:
            try:
                shutil.rmtree(self._temp_dir, ignore_errors=True)
            except Exception:
                pass
            self._temp_dir = None