$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$proj = Join-Path $root "dotnet/Spoofing/Spoofing.csproj"

dotnet build $proj -c Release