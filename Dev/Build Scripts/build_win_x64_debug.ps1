# (C) Sebastian Fiault 2026

Write-Host "[INFO] To build on Windows you need to have Visual Studio 2026 installed on your System\nSince its CMake generator is needed for the build" -ForegroundColor Yellow

# Get the root directory of the project, since CMake does not seem to like ../
$ProjectRoot = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent

cmake -S $ProjectRoot --preset x64-Windows-Debug --fresh

cmake --build --preset x64-Windows-Debug

#TODO document this so that people can build it on GitHub
