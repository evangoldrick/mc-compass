#include "DisplayCompass.hpp"


DisplayCompass::DisplayCompass(WS2812B_Controller& ledController) : ledController(ledController){
    for (int y = 0; y < DisplayConstants::displayHeight; y++) {
        for (int x = 0; x < DisplayConstants::displayWidth; x++) {
            setDisplayPixel(0, x, y);
        }
    }
}

void DisplayCompass::drawCasingPixels() {
    WS2812B_Controller::Color colors[8];
    colors[0] = 0;

    if (brightness <= 4) {
        colors[1] = 0;
        for (int i = 2; i < 8; i++) {
            colors[i] = brightness;
        }
    } else if (brightness <= 8) {
        colors[1] = 1;
        for (int i = 2; i < 8; i++) {
            colors[i] = brightness;
        }
    } else {
        for (int i = 1; i < 8; i++) {
            colors[i] = brightness / (8 - i);
        }
    }
    

    // Assumes colors[i] is less than 256, makes color gray
    for (int i = 0; i < 8; i++) {
        colors[i] *= 0x010101;
    }

    setDisplayPixel(colors[5],  3,  0);
    setDisplayPixel(colors[5],  4,  0);
    setDisplayPixel(colors[4],  5,  0);
    setDisplayPixel(colors[3],  6,  0);
    setDisplayPixel(colors[4],  7,  0);
    setDisplayPixel(colors[3],  8,  0);

    setDisplayPixel(colors[3],  1,  1);
    setDisplayPixel(colors[4],  2,  1);
    setDisplayPixel(colors[1],  3,  1);
    setDisplayPixel(colors[1],  4,  1);
    setDisplayPixel(colors[1],  5,  1);
    setDisplayPixel(colors[1],  6,  1);
    setDisplayPixel(colors[1],  7,  1);
    setDisplayPixel(colors[1],  8,  1);
    setDisplayPixel(colors[3],  9,  1);
    setDisplayPixel(colors[3], 10,  1);

    setDisplayPixel(colors[3],  0,  2);
    setDisplayPixel(colors[1],  1,  2);
    setDisplayPixel(colors[1],  2,  2);
    setDisplayPixel(colors[1],  3,  2);
    setDisplayPixel(colors[1],  4,  2);
    setDisplayPixel(colors[1],  5,  2);
    setDisplayPixel(colors[1],  6,  2);
    setDisplayPixel(colors[1],  7,  2);
    setDisplayPixel(colors[1],  8,  2);
    setDisplayPixel(colors[1],  9,  2);
    setDisplayPixel(colors[1], 10,  2);
    setDisplayPixel(colors[3], 11,  2);

    setDisplayPixel(colors[1],  0,  3);
    setDisplayPixel(colors[1],  1,  3);
    setDisplayPixel(colors[1],  2,  3);
    setDisplayPixel(colors[1],  3,  3);
    setDisplayPixel(colors[1],  4,  3);
    setDisplayPixel(colors[1],  5,  3);
    setDisplayPixel(colors[1],  6,  3);
    setDisplayPixel(colors[1],  7,  3);
    setDisplayPixel(colors[1],  8,  3);
    setDisplayPixel(colors[1],  9,  3);
    setDisplayPixel(colors[1], 10,  3);
    setDisplayPixel(colors[1], 11,  3);

    setDisplayPixel(colors[1],  0,  4);
    setDisplayPixel(colors[1],  1,  4);
    setDisplayPixel(colors[1],  2,  4);
    setDisplayPixel(colors[1],  3,  4);
    setDisplayPixel(colors[1],  4,  4);
    setDisplayPixel(colors[1],  5,  4);
    setDisplayPixel(colors[1],  6,  4);
    setDisplayPixel(colors[1],  7,  4);
    setDisplayPixel(colors[1],  8,  4);
    setDisplayPixel(colors[1],  9,  4);
    setDisplayPixel(colors[1], 10,  4);
    setDisplayPixel(colors[1], 11,  4);

    setDisplayPixel(colors[5],  0,  5);
    setDisplayPixel(colors[1],  1,  5);
    setDisplayPixel(colors[1],  2,  5);
    setDisplayPixel(colors[1],  3,  5);
    setDisplayPixel(colors[1],  4,  5);
    setDisplayPixel(colors[1],  5,  5);
    setDisplayPixel(colors[1],  6,  5);
    setDisplayPixel(colors[1],  7,  5);
    setDisplayPixel(colors[1],  8,  5);
    setDisplayPixel(colors[1],  9,  5);
    setDisplayPixel(colors[1], 10,  5);
    setDisplayPixel(colors[4], 11,  5);

    setDisplayPixel(colors[4],  0,  6);
    setDisplayPixel(colors[7],  1,  6);
    setDisplayPixel(colors[7],  2,  6);
    setDisplayPixel(colors[1],  3,  6);
    setDisplayPixel(colors[1],  4,  6);
    setDisplayPixel(colors[1],  5,  6);
    setDisplayPixel(colors[1],  6,  6);
    setDisplayPixel(colors[1],  7,  6);
    setDisplayPixel(colors[1],  8,  6);
    setDisplayPixel(colors[4],  9,  6);
    setDisplayPixel(colors[4], 10,  6);
    setDisplayPixel(colors[3], 11,  6);

    setDisplayPixel(colors[5],  0,  7);
    setDisplayPixel(colors[6],  1,  7);
    setDisplayPixel(colors[6],  2,  7);
    setDisplayPixel(colors[7],  3,  7);
    setDisplayPixel(colors[7],  4,  7);
    setDisplayPixel(colors[5],  5,  7);
    setDisplayPixel(colors[6],  6,  7);
    setDisplayPixel(colors[6],  7,  7);
    setDisplayPixel(colors[5],  8,  7);
    setDisplayPixel(colors[3],  9,  7);
    setDisplayPixel(colors[2], 10,  7);
    setDisplayPixel(colors[2], 11,  7);

    setDisplayPixel(colors[5],  1,  8);
    setDisplayPixel(colors[6],  2,  8);
    setDisplayPixel(colors[6],  3,  8);
    setDisplayPixel(colors[6],  4,  8);
    setDisplayPixel(colors[4],  5,  8);
    setDisplayPixel(colors[5],  6,  8);
    setDisplayPixel(colors[5],  7,  8);
    setDisplayPixel(colors[3],  8,  8);
    setDisplayPixel(colors[3],  9,  8);
    setDisplayPixel(colors[2], 10,  8);

    setDisplayPixel(colors[5],  3,  9);
    setDisplayPixel(colors[6],  4,  9);
    setDisplayPixel(colors[4],  5,  9);
    setDisplayPixel(colors[5],  6,  9);
    setDisplayPixel(colors[5],  7,  9);
    setDisplayPixel(colors[3],  8,  9);
}


void DisplayCompass::drawNeedlePixels() {
    WS2812B_Controller::Color needleColor = brightness << 16;

    static constexpr float needleSemiMinor = 2.5f;
    static constexpr float needleSemiMajor = 4.5f;
    static constexpr float needleCenterX = 6.5f;
    static constexpr float needleCenterY = 4.5f;
    static constexpr int numSteps = 20;
    // TODO: Draw gray parts of needle

    // Draw red needle arm
    float needlePointX = needleSemiMajor * cos(compassAngle) + needleCenterX - 0.5f;
    float needlePointY = needleSemiMinor * sin(compassAngle) + needleCenterY - 0.5f;

    for (int i = 0; i < numSteps; i++) {
        float lerpX = (((needlePointX - needleCenterX) * ((float) i)) / ((float) numSteps)) + needleCenterX;
        float lerpY = (((needlePointY - needleCenterY) * ((float) i)) / ((float) numSteps)) + needleCenterY;

        setDisplayPixel(needleColor, lerpX, lerpY);
    }

}


void DisplayCompass::show() {
    for (int y = 0; y < DisplayConstants::displayHeight; y++) {
        for (int x = 0; x < DisplayConstants::displayWidth; x++) {
            setLEDPixel(getDisplayPixel(x, y), x, y);
        }
    }
    ledController.show();
}