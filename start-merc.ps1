param(
    [Parameter(Position = 0)]
    [ValidateSet("start", "stop", "restart", "status")]
    [string]$Action = "start",

    [string]$Distro
)

$ErrorActionPreference = "Stop"

function Convert-WindowsPathToWslPath {
    param(
        [Parameter(Mandatory = $true)]
        [string]$WindowsPath
    )

    $resolved = [System.IO.Path]::GetFullPath($WindowsPath)
    $uri = New-Object System.Uri($resolved)
    $path = $uri.AbsolutePath.TrimStart("/")

    if ($path.Length -lt 2 -or $path[1] -ne ":") {
        throw "無法轉換成 WSL 路徑: $resolved"
    }

    $drive = $path.Substring(0, 1).ToLowerInvariant()
    $rest = $path.Substring(2).Replace("\", "/").TrimStart("/")
    if ([string]::IsNullOrEmpty($rest)) {
        return "/mnt/$drive"
    }

    return "/mnt/$drive/$rest"
}

$repoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$wslRepoRoot = Convert-WindowsPathToWslPath -WindowsPath $repoRoot

$bashCommand = "cd '$wslRepoRoot' && ./start-merc.sh $Action"
$wslArgs = @()

if ($Distro) {
    $wslArgs += @("-d", $Distro)
}

$wslArgs += @("--", "bash", "-lc", $bashCommand)

Write-Host "[start-merc.ps1] WSL repo path: $wslRepoRoot"
if ($Distro) {
    Write-Host "[start-merc.ps1] distro: $Distro"
}
Write-Host "[start-merc.ps1] forwarding action '$Action' to ./start-merc.sh"

& wsl.exe @wslArgs
$exitCode = $LASTEXITCODE

if ($exitCode -ne 0) {
    throw "WSL launch failed with exit code: $exitCode"
}
