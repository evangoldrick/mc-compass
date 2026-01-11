#include "CompassMain.hpp"

CompassMain::CompassMain(WS2812B_Controller& ledController) : ledController(ledController), compassDisplay(ledController) {

}

void CompassMain::mainLoop() {
    absolute_time_t nextLoopTime = 0;
    while (1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_until(nextLoopTime);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        nextLoopTime = make_timeout_time_ms(50);
        compassDisplay.setAngle(compassDisplay.getAngle() + 0.1f);
        compassDisplay.setBrightness(16);
        compassDisplay.drawCasingPixels();
        compassDisplay.drawNeedlePixels();

        compassDisplay.show();
    }
}