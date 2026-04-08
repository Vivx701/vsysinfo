param(
    [string]$toolchain = "mingw"  # default
)

# ================= PROJECT ROOT =================

$ProjectRoot = Resolve-Path "$PSScriptRoot"

# ================= CONFIG =================

# MinGW paths
$MinGWRoot = "C:/Qt/Tools/mingw1310_64"
$MinGWBin  = "$MinGWRoot/bin"

# ================= BUILD DIR =================

$BaseBuildDir = "build"

if ($toolchain -eq "msvc") {
    $BuildDir = "$BaseBuildDir/msvc"
}
elseif ($toolchain -eq "mingw") {
    $BuildDir = "$BaseBuildDir/mingw"
}
elseif ($toolchain -eq "msvc-vs") {
    $BuildDir = "$BaseBuildDir/msvc-vs"
}
else {
    Write-Host "Invalid toolchain: $toolchain" -ForegroundColor Red
    exit 1
}

Write-Host "Toolchain: $toolchain"
Write-Host "Build directory: $BuildDir"

# ================= CLEAN =================

if (Test-Path $BuildDir) {
    Write-Host "Cleaning $BuildDir..."
    Remove-Item -Recurse -Force $BuildDir
}

# ================= CREATE =================

New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null
Set-Location $BuildDir

# ================= TOOLCHAIN =================

if ($toolchain -eq "mingw") {

    Write-Host "=== Using MinGW ===" -ForegroundColor Cyan

    $env:PATH = "$MinGWBin;$env:PATH"

    cmake "$ProjectRoot" `
        -G "MinGW Makefiles" `
        -DCMAKE_BUILD_TYPE=Release

}
elseif ($toolchain -eq "msvc") {

    Write-Host "=== Using MSVC (Ninja) ===" -ForegroundColor Cyan

    $vcvars = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

    if (!(Test-Path $vcvars)) {
        Write-Host "ERROR: vcvars64.bat not found!" -ForegroundColor Red
        exit 1
    }

    Write-Host "Using vcvars from: $vcvars"

    # Run everything inside cmd with correct project path
    cmd /c "`"$vcvars`" && cmake `"$ProjectRoot`" -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build . && ctest --output-on-failure"

    if ($LASTEXITCODE -ne 0) {
        Write-Host "Build failed!" -ForegroundColor Red
        exit 1
    }

    Write-Host "=== Build SUCCESS ===" -ForegroundColor Green
    exit 0
}
elseif ($toolchain -eq "msvc-vs") {

    Write-Host "=== Using MSVC (Visual Studio Generator) ===" -ForegroundColor Cyan

    cmake "$ProjectRoot" `
        -B "$ProjectRoot/build/msvc-vs" `
        -G "Visual Studio 17 2022" `
        -A x64

    Write-Host "Solution generated at build/msvc-vs"
}
# ================= BUILD =================

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    exit 1
}

Write-Host "=== Building ===" -ForegroundColor Yellow

cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed!" -ForegroundColor Red
    exit 1
}

# ================= TEST =================

Write-Host "=== Running tests ===" -ForegroundColor Yellow

ctest --output-on-failure -C Release

Write-Host "=== Build SUCCESS ===" -ForegroundColor Green
