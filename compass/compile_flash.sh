# Compile and flash program

cmake build
cmake --build build

picotool load -f ./build/compass.uf2
# picotool reboot
