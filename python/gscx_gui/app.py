import sys
import argparse
from .modules_loader import ModulesLoader


def main(argv=None):
    parser = argparse.ArgumentParser(description="GSCX GUI/CLI - Boot Recovery HLE")
    g = parser.add_mutually_exclusive_group()
    g.add_argument("--load-default", action="store_true", help="Carrega DLLs dos diretórios padrão de build")
    g.add_argument("--bundle", type=str, help="Caminho para bundle GSCore (.gscb)")
    parser.add_argument("--boot-recovery", action="store_true", help="Invoca GSCX_RecoveryEntry após carregar os módulos")
    args = parser.parse_args(argv)

    def on_log(msg: str):
        print(msg, end="")

    loader = ModulesLoader(on_log)

    if args.bundle:
        print(f"Carregando bundle: {args.bundle}")
        loader.load_from_gscore(args.bundle)
    else:
        print("Carregando módulos dos diretórios padrão...")
        loader.load_default_modules()

    if args.boot_recovery:
        loader.boot_recovery()

    # Mantenha o processo vivo se necessário (no futuro para GUI)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())