#pragma once

#include "pico/stdlib.h"
#include "WS2812B_Controller.hpp"
#include "DisplayConstants.hpp"
#include <array>

/*
Casing colors
#ffffff
#d8d8d8
#a8a8a8
#828282
#5e5e5e
#4f4d4d
#353535
#2f2f2f
#181717

Needle colors
#646464
#4f4d4d
#ff1414
#cb1a1a
*/


class DisplayCompass {
    std::array<std::array<WS2812B_Controller::Color, DisplayConstants::displayWidth>, DisplayConstants::displayHeight> display;
    WS2812B_Controller& ledController;
    float compassAngle = 0;
    int brightness = 1;

    public:
    DisplayCompass() = delete;
    DisplayCompass(WS2812B_Controller& ledController);
    ~DisplayCompass() = default;

    inline void setBrightness(const int& brightness) {this->brightness = brightness;}
    inline const int& getBrightness() {return brightness;}

    inline void setAngle(const float& compassAngle) {this->compassAngle = compassAngle;}
    inline const float& getAngle() {return compassAngle;}

    inline bool isInDisplayRect(int x, int y) {return x >= 0 && x < DisplayConstants::displayWidth && y >= 0 && y < DisplayConstants::displayHeight;}

    inline void setDisplayPixel(WS2812B_Controller::Color c, int x, int y) {if (isInDisplayRect(x, y)) display[y][x] = c;}
    inline const WS2812B_Controller::Color& getDisplayPixel(int x, int y) {return display[y][x];}
    
    void drawCasingPixels();
    void drawNeedlePixels();

    void setLEDPixel(WS2812B_Controller::Color c, int x, int y) {ledController.setLed(DisplayConstants::convertToLedIndex(x, y), c);}

    void show();
};
