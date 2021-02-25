/* RMT example -- RGB LED Strip

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"


#include "board.h"
static const char *TAG = "example";



#define EXAMPLE_CHASE_SPEED_MS (1000)


void app_main(void)
{

    ESP_LOGI(TAG, "Initializing...");

    board_init();



    // Show simple rainbow chasing pattern
    ESP_LOGI(TAG, "LED Rainbow Chase Start");
    while (true) {

    	// make a RED Blink
        ESP_LOGI(TAG, "make a RED Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_OFF);

    	// make a GREEN Blink
        ESP_LOGI(TAG, "make a GREEN Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_OFF);

    	// make a BLUE Blink
        ESP_LOGI(TAG, "make a BLUE Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_OFF);


    	// make a RED + GREEN =  Blink
        ESP_LOGI(TAG, "make a RED + GREEN Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_OFF);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_OFF);

    	// make a RED + BLUE =  Blink
        ESP_LOGI(TAG, "make a RED + BLUE Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_OFF);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_OFF);

    	// make a GREEN + BLUE =  Blink
        ESP_LOGI(TAG, "make a GREEN + BLUE Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_OFF);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_OFF);



    	// make a WHITE Blink
        ESP_LOGI(TAG, "make a WHITE Blink");
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_ON);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_R, LED_OFF);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_G, LED_OFF);
    	vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
        board_led_operation(LED_B, LED_OFF);

    }
}
