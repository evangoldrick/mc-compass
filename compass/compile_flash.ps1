# Compile and flash program
picotool reboot -f -u # Reboot to usb mode

cmake --build build

if ($?) {
    Start-Sleep 1
    picotool load ./build/compass.uf2 # Flash program
    picotool reboot -a # Reboot and run application
} else {
    Write-Host "Build failed, program not flashed"
}
