#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/clocks.h"

#include "WS2812B_controller.hpp"
#include "CompassMain.hpp"

#define LED_STRIP_PIN 14
#define LED_NUM 104

int main()
{
    uint vco_freq_out = 0;
    uint div1_out = 0;
    uint div2_out = 0;
    sleep_ms(1000);

    if (check_sys_clock_hz(100 * MHZ, &vco_freq_out, &div1_out, &div2_out)) {
        //clock_configure(clk_sys, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX, CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, 125 * MHZ, 100 * MHZ);
        

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

        if (true) {
            WS2812B_Controller ledController{LED_STRIP_PIN, LED_NUM};
            CompassMain cm{ledController};
            
            cm.mainLoop();
        } else {
        
            WS2812B_Controller strip(LED_STRIP_PIN, LED_NUM);
            int x = 0;
            absolute_time_t nextLoopTime = 0;
            while (1) {
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                sleep_until(nextLoopTime);
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
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
                printf("f:%d 1:%d 2:%d\n", vco_freq_out, div1_out, div2_out);
                printf("freq:%ld\n", clock_get_hz(clk_sys));
                for (int i = 0; i < LED_NUM; i++) {
                    strip.setLedHSV(i, (float)((x + i * 2) % 360), 1.0f, 0.1f);
                }
                x += 2;
                strip.show();

            }
        }
    }
}

