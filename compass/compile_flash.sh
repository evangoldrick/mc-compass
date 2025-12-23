# Compile and flash program

cmake build
cmake --build build

picotool load ./build/compass.uf2
picotool reboot
