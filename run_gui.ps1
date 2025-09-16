param(
    [Parameter(ValueFromRemainingArguments=$true)]
    [string[]]$Args
)

$Root = Split-Path -Parent $MyInvocation.MyCommand.Path
$VenvPath = Join-Path $Root ".venv"

if (!(Test-Path $VenvPath)) {
    python -m venv $VenvPath
}

$Activate = Join-Path $VenvPath "Scripts\Activate.ps1"
. $Activate

if (-not $env:GSCX_SKIP_PIP) {
    python -m pip install --upgrade pip
    python -m pip install -r (Join-Path $Root "python/requirements.txt")
} else {
    Write-Host "[run_gui] Pulando instalação de requirements (GSCX_SKIP_PIP=1)" -ForegroundColor Yellow
}

# Adiciona diretório python ao PYTHONPATH
$env:PYTHONPATH = (Join-Path $Root "python")

# Repasse de argumentos para o app
python -m gscx_gui.app @Args