/*
Basic library for controlling a strip of WS2812B LEDs (header only)

Taking guidance from code on this forum: https://forums.raspberrypi.com/viewtopic.php?t=322218
Delay code comes from cycle_delay.s provided in the forum.

*/
#pragma once
#include "pico/stdlib.h"
#include <math.h>

extern "C" { // Functions defined in cycle_delay.s
    void cycle_delay_t0h();
    void cycle_delay_t0l();
    void cycle_delay_t1h();
    void cycle_delay_t1l();
    uint32_t disable_and_save_interrupts();
    void enable_and_restore_interrupts(uint32_t);
}

class WS2812B_Controller {
    public:
    /**
     * @brief Union struct to represent a single LED in the WS2812B strip.
     *        The LEDs use GRB (Green, Red, Blue) ordering.
     */
    union LED {
        public:
        uint8_t data[3];
        struct subpixels {
            uint8_t g;
            uint8_t r;
            uint8_t b;
        } s;
    };

    private:
    uint8_t ledPin = 0;
    uint numLeds = 0;
    LED* leds = nullptr;
    absolute_time_t nextAllowedTransmissionTime = 0;

    public:
    WS2812B_Controller() = delete;
    /**
     * @brief Constructs a WS2812B LED controller object.
     * @param ledPin The GPIO pin number to which the WS2812B LEDs are connected.
     * @param numLeds The number of WS2812B LEDs in the strip.
     */
    WS2812B_Controller(const uint8_t ledPin, uint numLeds): ledPin(ledPin), numLeds(numLeds) {
        setNumLeds(numLeds);
    }

    ~WS2812B_Controller() {
        delete[] leds;
    }

    void setNumLeds(const uint numLeds) {
        this->numLeds = numLeds;

        if (leds) delete[] leds;

        if (numLeds) {
            leds = new LED[numLeds];
            for (uint i = 0; i < numLeds; i++) {
                for (int j = 0; j < 3; j++) {
                    leds[i].data[j] = 0;
                }
            }
        } else {
            leds = nullptr;
        }
    }

    uint getNumLeds() {
        return numLeds;
    }

    inline void setLed(const uint& index, const uint8_t& r, const uint8_t& g, const uint8_t& b) {
        if (index < numLeds) { // Ignore out of bounds requests
            leds[index].s.g = g;
            leds[index].s.r = r;
            leds[index].s.b = b;
        }
    }

    inline void setLed(const uint& index, const uint64_t& color) {
        setLed(index, (uint8_t) ((color >> 16) & 0xFF), (uint8_t) ((color >> 8) & 0xFF), (uint8_t) (color & 0xFF));
    }

    inline void setAllLeds(const uint8_t& r, const uint8_t& g, const uint8_t& b) {
        for (uint i = 0; i < numLeds; i++) {
            setLed(i, r, g, b);
        }
    }

    inline void setAllLeds(const uint64_t& color) {
        for (uint i = 0; i < numLeds; i++) {
            setLed(i, color);
        }
    }

    void setLedHSV(const uint& index, float h, float s, float v) {
        if (h < 0.0f) h = 0.0f;
        if (h > 360.0f) h = 360.0f;
        if (s < 0.0f) s = 0.0f;
        if (s > 1.0f) s = 1.0f;
        if (v < 0.0f) v = 0.0f;
        if (v > 1.0f) v = 1.0f;

        float c = v * s;
        float x = c * (1 - abs(fmod(h / 60.0f, 2) - 1));
        float m = v - c;

        float r, g, b;
        if (h >= 0 && h < 60) {
            r = c, g = x, b = 0;
        } else if (h >= 60 && h < 120) {
            r = x, g = c, b = 0;
        } else if (h >= 120 && h < 180) {
            r = 0, g = c, b = x;
        } else if (h >= 180 && h < 240) {
            r = 0, g = x, b = c;
        } else if (h >= 240 && h < 300) {
            r = x, g = 0, b = c;
        } else {
            r = c, g = 0, b = x;
        }

        r = (r + m) * 255;
        g = (g + m) * 255;
        b = (b + m) * 255;

        setLed(index, r, g, b);
    }

    void show() {
        while (absolute_time_diff_us(get_absolute_time(), nextAllowedTransmissionTime) > 0);

        uint32_t pin = 1ul << ledPin; // Mask of pin connected to strip
        uint32_t interrupt_mask = disable_and_save_interrupts();
        int8_t bit;
        uint8_t subpixel;
        for (uint led = 0; led < numLeds; led++) {
            for (subpixel = 0; subpixel < 3; subpixel++) {
                for (bit = 7; bit >= 0; bit--) {
                    if ((leds[led].data[subpixel] >> bit ) & 1) { // Bit high, on-time is longer than off-time
                        sio_hw->gpio_set = pin;
                        cycle_delay_t1h();
                        sio_hw->gpio_clr = pin;
                        cycle_delay_t1l();
                    } else { // Bit low, on-time is shorter than off-time
                        sio_hw->gpio_set = pin;
                        cycle_delay_t0h();
                        sio_hw->gpio_clr = pin;
                        cycle_delay_t0l();
                    }
                }
            }
        }
        
        sio_hw->gpio_clr = pin;
        enable_and_restore_interrupts(interrupt_mask);
        nextAllowedTransmissionTime = make_timeout_time_us(50);
    }
};


