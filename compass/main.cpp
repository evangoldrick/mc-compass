#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "WS2812B_controller.hpp"

#define LED_STRIP_PIN 14

#define LED_NUM 60


int main()
{
    // init radio for onboard led
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(1000);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    gpio_init(LED_STRIP_PIN);
    gpio_set_dir(LED_STRIP_PIN, GPIO_OUT);
    gpio_put(LED_STRIP_PIN, false);

    WS2812B_Controller strip(LED_STRIP_PIN, LED_NUM);
    int x = 0;
    while (1) {
        // strip.setAllLeds(0x000000);
        // strip.show();
        // strip.setAllLeds(0xFF0000);
        // strip.show();
        // strip.setAllLeds(0x00FF00);
        // strip.show();
        // strip.setAllLeds(0x0000FF);
        // strip.show();
        // strip.setAllLeds(0xFFFF00);
        // strip.show();
        // strip.setAllLeds(0xFF00FF);
        // strip.show();
        // strip.setAllLeds(0x00FFFF);
        // strip.show();
        // strip.setAllLeds(0xFFFFFF);
        // strip.show();
        
        for (int i = 0; i < LED_NUM; i++) {
            strip.setLedHSV(i, (float)((x + i) % 360), 1.0f, 0.1f);
        }
        x += 1;
        strip.show();
    }
}

