$ProjectDir = $PSScriptRoot

Push-Location $ProjectDir

try {
    & "C:\devkitPro\msys2\usr\bin\make.exe" -B

    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }

    & "C:\devkitPro\msys2\usr\bin\bash.exe" -lc "3dslink '/c/Users/benho/RiderProjects/MAZBALL3DS/MAZBALL3DS/MAZBALL3DS.3dsx' -a 192.168.0.135"

    exit $LASTEXITCODE
}
finally {
    Pop-Location
}