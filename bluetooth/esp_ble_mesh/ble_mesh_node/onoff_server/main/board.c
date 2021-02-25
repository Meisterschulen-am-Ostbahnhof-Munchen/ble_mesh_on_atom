/* board.c - Board-specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 * Additional Copyright (c) 2018 Espressif Systems (Shanghai) PTE LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "sdkconfig.h"
#include "esp_log.h"
#include "board.h"
#include "driver/rmt.h"
#include "led_strip.h"

static const char *TAG = "BOARD";


#define RMT_TX_CHANNEL RMT_CHANNEL_0


static led_strip_t *strip = (led_strip_t *)0;

struct _led_state led_state[3] = {
    { LED_OFF, LED_OFF, LED_R, "red"   },
    { LED_OFF, LED_OFF, LED_G, "green" },
    { LED_OFF, LED_OFF, LED_B, "blue"  },
};

static uint32_t red = 0;
static uint32_t green = 0;
static uint32_t blue = 0;

void board_led_operation(uint8_t pin, uint8_t on)
{
    for (int i = 0; i < 3; i++) {
        if (led_state[i].pin != pin) {
            continue;
        }
        if (on == led_state[i].previous) {
            ESP_LOGW(TAG, "led %s is already %s",
                     led_state[i].name, (on ? "on" : "off"));
            return;
        }
        //now we want to set the State.
        // Write RGB values to strip driver
        switch(pin)
        {
        case LED_ALL:
        	red = on ? 0xFF : 0x0;
        	green = on ? 0xFF : 0x0;
        	blue = on ? 0xFF : 0x0;
        	break;
        case LED_R:
        	red = on ? 0xFF : 0x0;
        	break;
        case LED_G:
        	green = on ? 0xFF : 0x0;
        	break;
        case LED_B:
        	blue = on ? 0xFF : 0x0;
        	break;
        }
        for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
            // Build RGB values
        	ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
            // Flush RGB values to LEDs
            ESP_ERROR_CHECK(strip->refresh(strip, 100));
        }
        led_state[i].previous = on;
        return;
    }
    ESP_LOGE(TAG, "LED is not found!");
}

static void board_led_init(void)
{
    for (int i = 0; i < 3; i++) {

        led_state[i].previous = LED_OFF;
    }


    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(CONFIG_EXAMPLE_STRIP_LED_NUMBER, (led_strip_dev_t)config.channel);
    strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    // Clear LED strip (turn off all LEDs)
    ESP_ERROR_CHECK(strip->clear(strip, 100));
    board_led_operation(LED_ALL, LED_OFF);
}

void board_init(void)
{
    board_led_init();
}
