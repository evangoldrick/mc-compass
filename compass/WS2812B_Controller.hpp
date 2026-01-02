/*
Basic library for controlling a strip of WS2812B LEDs (header only)
*/
#pragma once
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include <math.h>

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

    /**
     * @brief Color type used to set rgb value in one variable. Format: 0x00RRGGBB.
     */
    typedef uint64_t Color;

    private:
    uint8_t ledPin = 0;
    uint numLeds = 0;
    LED* leds = nullptr;
    absolute_time_t nextAllowedTransmissionTime = 0;

    WS2812B_Controller() = delete;
    WS2812B_Controller(const WS2812B_Controller&) = delete;
    WS2812B_Controller(WS2812B_Controller&&) = delete;
    public:
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

    inline void setLed(const uint& index, const Color& color) {
        setLed(index, (uint8_t) ((color >> 16) & 0xFF), (uint8_t) ((color >> 8) & 0xFF), (uint8_t) (color & 0xFF));
    }

    inline void setAllLeds(const uint8_t& r, const uint8_t& g, const uint8_t& b) {
        for (uint i = 0; i < numLeds; i++) {
            setLed(i, r, g, b);
        }
    }

    inline void setAllLeds(const Color& color) {
        for (uint i = 0; i < numLeds; i++) {
            setLed(i, color);
        }
    }

    /**
     * @brief Set a LED to a color using HSV
     * @param index of the LED
     * @param h Hue
     * @param s Saturation
     * @param v Value
     */
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

    inline void setAllLedsHSV(float h, float s, float v) {
        for (uint i = 0; i < numLeds; i++) {
            setLedHSV(i, h, s, v);
        }
    }

    /**
     * @brief Output the current pixel buffer to the LEDs
     */
    void show() {
        sleep_until(nextAllowedTransmissionTime);

        uint32_t pin = 1ul << ledPin; // Mask of pin connected to strip
        uint32_t interrupt_mask = save_and_disable_interrupts();
        int8_t bit;
        uint8_t subpixel;
        for (uint led = 0; led < numLeds; led++) {
            for (subpixel = 0; subpixel < 3; subpixel++) {
                for (bit = 7; bit >= 0; bit--) {
                    if ((leds[led].data[subpixel] >> bit ) & 1) { // Bit high, on-time is longer than off-time
                        sio_hw->gpio_set = pin;
                        cycleDelay(cycleDelayConvertFromNanoseconds(800));
                        sio_hw->gpio_clr = pin;
                        cycleDelay(cycleDelayConvertFromNanoseconds(450));
                    } else { // Bit low, on-time is shorter than off-time
                        sio_hw->gpio_set = pin;
                        cycleDelay(cycleDelayConvertFromNanoseconds(400));
                        sio_hw->gpio_clr = pin;
                        cycleDelay(cycleDelayConvertFromNanoseconds(850));
                    }
                }
            }
        }
        
        sio_hw->gpio_clr = pin;
        restore_interrupts(interrupt_mask);
        nextAllowedTransmissionTime = make_timeout_time_us(50); // Don't allow another transmission until 50us after the last one
    }

    /**
     * @brief Convert nanoseconds to cycles of delay time for the cycleDelay function.
     * @details This is constexpr so the delay calculation does not add to the delay time. Each clock cycle taken is significant here.
     * @param delay How many nanoseconds to wait for.
     */
    constexpr uint cycleDelayConvertFromNanoseconds(uint delay) {
        double delayFactor = ((double) SYS_CLK_HZ) / (3.0 * 1.0E9); // 3 cycles per delay loop from the cycleDelay function
        return delay * delayFactor;
    }

    /**
     * @brief Function to delay by some number of cycles * 3.
     * @details Assuming the function is inlined properly, the number of cycles executed should be 3 * cycles + 1.
     * @param cycles How many cycles should be spent waiting.
     */
    inline void cycleDelay(uint cycles) {
        pico_default_asm_volatile(
            "1: SUBS %[num_cycles], #1\n" // 1 cycle
            "BNE 1b\n" // 2 cycles when branch taken 1 when not taken
            : : [num_cycles] "r" (cycles));
    }
};


