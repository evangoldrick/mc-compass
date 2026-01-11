#pragma once

#include "pico/stdlib.h"
#include <array>

/*
The compass display class is designed to transform pixels into the correct locations on the display

LED indexes:
         0 1 2 3 4 5
 X       █>█>█>█>█>█>  6
    <█<█<█<█<█<█<█<█<█<█
   █>█>█>█>█>█>█>█>█>█>█>█>
  <█<█<█<█<█<█<█<█<█<█<█<█
   █>█>█>█>█>█>█>█>█>█>█>█>
  <█<█<█<█<█<█<█<█<█<█<█<█
   █>█>█>█>█>█>█>█>█>█>█>█>
  <█<█<█<█<█<█<█<█<█<█<█<█
     █>█>█>█>█>█>█>█>█>█
         █<█<█<█<█<█
       103

There are 104 LEDs total

(x, y) coordinates are based in the top left of the display
(0, 0) is outside of the compass display area marked with "X" in the diagram above
(7, 6) is the "center" of the compass, middle pixel is to the right and down since the grid has an 
    even number of pixels horizontally and vertically

*/


namespace DisplayConstants {
    static constexpr int displayWidth = 12;
    static constexpr int displayHeight = 10;
    typedef std::array<std::array<uint, displayWidth>, displayHeight> IndexMapType;
    static constexpr uint invalidIndex = ~0u;

    static constexpr IndexMapType generateIndexMap() {
        IndexMapType generatedIndexMap = IndexMapType{};
        int skipX = 0;

        // Set positions inside the compass to 0, and positions outside the compass to invalidIndex
        for (int y = 0; y < displayHeight; y++){
            
            if (y == 0 || y == 9) {
                skipX = 3;
            } else if (y == 1 || y == 8) {
                skipX = 1;
            } else {
                skipX = 0;
            } 

            for (int x = 0; x < displayWidth; x++) {
                if (x < skipX || x > displayWidth - skipX - 1)  {
                    generatedIndexMap[y][x] = invalidIndex; // Out of bounds
                } else {
                    generatedIndexMap[y][x] = 0u; // In bounds
                }
            }
        }

        // Set positions inside the compass to the correct LED index
        int ledIndex = 0;
        for (int y = 0; y < displayHeight; y++){
            for (int x = 0; x < displayWidth; x++) {
                if (y & 1) { // Reverse index directon on odd rows
                    if (generatedIndexMap[y][displayWidth - x - 1] != invalidIndex) {
                        generatedIndexMap[y][displayWidth - x - 1] = ledIndex++;
                    }
                } else {
                    if (generatedIndexMap[y][x] != invalidIndex) {
                        generatedIndexMap[y][x] = ledIndex++;
                    }
                }
            }
        }
        return generatedIndexMap;
    }

    // Map to convert x,y to LED index, generated at compile time
    static constexpr IndexMapType indexMap = generateIndexMap();

    static constexpr bool isInsideDisplay(const int& x, const int& y) {
        if (x >= 0 && x < DisplayConstants::displayWidth && y >= 0 && y < DisplayConstants::displayHeight) {
            return DisplayConstants::indexMap[y][x] != DisplayConstants::invalidIndex;
        } else {
            return false;
        }
    }

    static constexpr uint convertToLedIndex(const int& x, const int& y) {
        if (isInsideDisplay(x, y)) {
            return DisplayConstants::indexMap[y][x];
        } else {
            return DisplayConstants::invalidIndex;
        }
    }
}