from PySide6.QtWidgets import QMainWindow, QWidget, QVBoxLayout, QPushButton, QTextEdit, QFileDialog, QLabel
from PySide6.QtCore import Qt
from .modules_loader import ModulesLoader


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("GSCX - PS3 Emulator (Scaffold)")
        self.resize(960, 640)

        self.loader = ModulesLoader(on_log=self.append_log)

        central = QWidget(self)
        layout = QVBoxLayout(central)

        self.lbl_fw = QLabel("Firmware/Recovery PUP ou imagem flash: não selecionado")
        btn_fw = QPushButton("Selecionar firmware/recovery...")
        btn_fw.clicked.connect(self.select_fw)

        btn_load = QPushButton("Carregar módulos")
        btn_load.clicked.connect(self.load_modules)

        btn_start = QPushButton("Iniciar emulador (stub)")
        btn_start.clicked.connect(self.start_stub)

        self.log = QTextEdit()
        self.log.setReadOnly(True)

        layout.addWidget(self.lbl_fw)
        layout.addWidget(btn_fw)
        layout.addWidget(btn_load)
        layout.addWidget(btn_start)
        layout.addWidget(self.log, 1)

        self.setCentralWidget(central)

    def append_log(self, text: str):
        self.log.append(text)

    def select_fw(self):
        path, _ = QFileDialog.getOpenFileName(self, "Selecione firmware/recovery (PUP/flash)", "", "Todos (*.*)")
        if path:
            self.lbl_fw.setText(f"Firmware selecionada: {path}")

    def load_modules(self):
        # Espera DLLs em ./build/bin ou similar; caminho pode ser ajustado
        self.loader.load_default_modules()

    def start_stub(self):
        self.append_log("Iniciando emulador (stub). Aqui, o host chamaria o recovery mode e CPU...")