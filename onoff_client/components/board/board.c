/* board.c - Board-specific hooks */

/*
 * Copyright (c) 2017 Intel Corporation
 * Additional Copyright (c) 2018 Espressif Systems (Shanghai) PTE LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>

#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

#include "iot_button.h"
#include "board.h"
#include "driver/rmt.h"
#include "led_strip.h"
#include "mesh_util.h"
#include "esp_ble_mesh_sensor_model_api.h"

static const char *TAG = "BOARD";

#define BUTTON_IO_NUM           39
#define BUTTON_ACTIVE_LEVEL     0

#define RMT_TX_CHANNEL RMT_CHANNEL_0


static led_strip_t *strip = (led_strip_t *)0;

extern void example_ble_mesh_send_gen_onoff_set(void);
extern void example_ble_mesh_send_sensor_message(uint32_t opcode);
extern void example_ble_mesh_send_vendor_message(bool resend);

static uint32_t send_opcode[] = {
    [0] = ESP_BLE_MESH_MODEL_OP_SENSOR_DESCRIPTOR_GET,
    [1] = ESP_BLE_MESH_MODEL_OP_SENSOR_CADENCE_GET,
    [2] = ESP_BLE_MESH_MODEL_OP_SENSOR_SETTINGS_GET,
    [3] = ESP_BLE_MESH_MODEL_OP_SENSOR_GET,
    [4] = ESP_BLE_MESH_MODEL_OP_SENSOR_SERIES_GET,
};
static uint8_t press_count;

struct _led_state led_state[3] = {
    { LED_OFF, LED_OFF, LED_R, "red"   },
    { LED_OFF, LED_OFF, LED_G, "green" },
    { LED_OFF, LED_OFF, LED_B, "blue"  },
};

static uint32_t red = 0;
static uint32_t green = 0;
static uint32_t blue = 0;

void board_led_operation(uint8_t pin, uint8_t onoff)
{

    //now we want to set the State.
    // Write RGB values to strip driver
    switch(pin)
    {
    case LED_ALL:
    	red = onoff ? 0xFF : 0x0;
    	green = onoff ? 0xFF : 0x0;
    	blue = onoff ? 0xFF : 0x0;
		pin = LED_R; //avoid Error.
    	break;
    case LED_R:
    	red = onoff ? 0xFF : 0x0;
    	break;
    case LED_G:
    	green = onoff ? 0xFF : 0x0;
    	break;
    case LED_B:
    	blue = onoff ? 0xFF : 0x0;
    	break;
    }
    for (int j = 0; j < CONFIG_EXAMPLE_STRIP_LED_NUMBER; j++) {
        // Build RGB values
    	ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
        // Flush RGB values to LEDs
        ESP_ERROR_CHECK(strip->refresh(strip, 100));
    }


    for (int i = 0; i < ARRAY_SIZE(led_state); i++) {
        if (led_state[i].pin != pin) {
            continue;
        }
        if (onoff == led_state[i].previous) {
            ESP_LOGW(TAG, "led %s is already %s",
                     led_state[i].name, (onoff ? "on" : "off"));
            return;
        }
		// no GPIO to set !
        led_state[i].previous = onoff;
        return;
    }

    ESP_LOGE(TAG, "LED is not found!");
}

static void board_led_init(void)
{
    for (int i = 0; i < ARRAY_SIZE(led_state); i++) {

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

static void button_tap_cb(void* arg)
{
    ESP_LOGI(TAG, "tap cb (%s)", (char *)arg);

    example_ble_mesh_send_gen_onoff_set();
    example_ble_mesh_send_sensor_message(send_opcode[press_count++]);
    example_ble_mesh_send_vendor_message(false);
    press_count = press_count % ARRAY_SIZE(send_opcode);
}

static void board_button_init(void)
{
    button_handle_t btn_handle = iot_button_create(BUTTON_IO_NUM, BUTTON_ACTIVE_LEVEL);
    if (btn_handle) {
        iot_button_set_evt_cb(btn_handle, BUTTON_CB_RELEASE, button_tap_cb, "RELEASE");
    }
}

void board_init(void)
{
    board_led_init();
    board_button_init();
}
