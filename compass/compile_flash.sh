# Compile and flash program

cmake --build build

if [ $? -eq 0 ]; then
    picotool load -f ./build/compass.uf2
    # picotool reboot
else
    Write-Host "Build failed, program not flashed"
fi
