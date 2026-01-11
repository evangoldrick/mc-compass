#include <array>
#include "pico/cyw43_arch.h"
#include "WS2812B_Controller.hpp"
#include "DisplayConstants.hpp"
#include "DisplayCompass.hpp"

class CompassMain {
    WS2812B_Controller& ledController;
    DisplayCompass compassDisplay;

    public:
    CompassMain(WS2812B_Controller& ledController);
    void mainLoop();
};