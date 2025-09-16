param(
    [string]$BuildType = "RelWithDebInfo"
)

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$cpp = Join-Path $root "cpp"
$build = Join-Path $cpp "build"

if (!(Test-Path $build)) { New-Item -ItemType Directory -Path $build | Out-Null }

# Bootstrap cmake/ninja via pip se não presentes
function Ensure-Tooling {
    $needCMake = -not (Get-Command cmake -ErrorAction SilentlyContinue)
    $needNinja = -not (Get-Command ninja -ErrorAction SilentlyContinue)
    if (-not ($needCMake -or $needNinja)) { return }

    $toolDir = Join-Path $root ".tooling"
    if (!(Test-Path $toolDir)) { New-Item -ItemType Directory -Path $toolDir | Out-Null }
    $venv = Join-Path $toolDir ".venv"
    if (!(Test-Path $venv)) {
        Write-Host "[tooling] Criando venv em $venv" -ForegroundColor Cyan
        python -m venv $venv
    }
    $activate = Join-Path $venv "Scripts/Activate.ps1"
    . $activate
    Write-Host "[tooling] Instalando pacotes: cmake ninja" -ForegroundColor Cyan
    python -m pip install --upgrade pip | Out-Null
    if ($needCMake) { python -m pip install cmake | Out-Null }
    if ($needNinja) { python -m pip install ninja | Out-Null }
    # Adiciona binários ao PATH desta sessão
    $scripts = Join-Path $venv "Scripts"
    $env:PATH = "$scripts;" + $env:PATH
}

function Reset-CMakeCache {
    if (Test-Path (Join-Path $build "CMakeCache.txt")) {
        Write-Host "[build] Limpando CMakeCache.txt e CMakeFiles" -ForegroundColor Yellow
        Remove-Item -Force (Join-Path $build "CMakeCache.txt") -ErrorAction SilentlyContinue
    }
    if (Test-Path (Join-Path $build "CMakeFiles")) {
        Remove-Item -Recurse -Force (Join-Path $build "CMakeFiles") -ErrorAction SilentlyContinue
    }
}

Ensure-Tooling

Push-Location $build

function Invoke-CMakeConfig {
    param(
        [string]$Generator
    )
    if ([string]::IsNullOrEmpty($Generator)) {
        cmake -DCMAKE_BUILD_TYPE=$BuildType ..
    } else {
        cmake -G $Generator -DCMAKE_BUILD_TYPE=$BuildType ..
    }
    return $LASTEXITCODE
}

# Detectar Ninja
$gen = $null
if (Get-Command ninja -ErrorAction SilentlyContinue) {
    $gen = "Ninja"
}

$configured = $false
$attempted = $false
if ($gen) {
    Write-Host "Configurando com gerador: $gen"
    $attempted = $true
    if ((Invoke-CMakeConfig -Generator $gen) -eq 0) { $configured = $true }
}

# Tentar Visual Studio 2022 e 2019
if (-not $configured) {
    if ($attempted) { Reset-CMakeCache }
    $vsGens = @(
        "Visual Studio 17 2022",
        "Visual Studio 16 2019"
    )
    foreach ($g in $vsGens) {
        Write-Host "Tentando gerador: $g (x64)"
        cmake -G $g -A x64 -DCMAKE_BUILD_TYPE=$BuildType ..
        if ($LASTEXITCODE -eq 0) { $configured = $true; break } else { Reset-CMakeCache }
    }
}

# Tentar sem gerador explícito
if (-not $configured) {
    Write-Host "Tentando cmake padrão (sem -G)"
    if ((Invoke-CMakeConfig -Generator $null) -eq 0) { $configured = $true }
}

if (-not $configured) {
    Pop-Location
    throw "Falha na configuração do CMake. Instale Visual Studio Build Tools (C++)."
}

cmake --build . --config $BuildType
$code = $LASTEXITCODE
Pop-Location

if ($code -ne 0) {
    throw "Falha no build ($code). Verifique se o compilador C++ (MSVC) está instalado."
}

Write-Host "Build C++ concluído. Saída: $build"