#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "esp_random.h"
#include "neural_network.h"

// #include "Layers.h"
// #include "NeuralNetwork.h"

#define BLINK_GPIO 2 
#define CONFIG_BLINK_PERIOD 200
#define CONFIG_RAN_GEN_PERIOD 1000

static const char *TAG = "Debug";

static uint8_t s_led_state = 0;

// Define Tasks 
TaskHandle_t Task1 = NULL;
TaskHandle_t Task2 = NULL;


void app_main(void)
{
    
    xTaskCreatePinnedToCore(Blinker, "Blinker", 4096, NULL, 10, &Task1,0);
    // xTaskCreatePinnedToCore(RNG, "RNG", 4096, NULL, 10, &Task2,1);
    xTaskCreatePinnedToCore(NN, "RNG", 4096, NULL, 10, &Task2,1);

}

void Blinker(void *arg)
{
    // blinking
    configure_led();
    
    while(1){
        blink_led();
        s_led_state = !s_led_state;

        vTaskDelay(CONFIG_BLINK_PERIOD/ portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Turning the LED %s! at GPIO: %d for a period of: %d on core: %d\
        ", s_led_state ==true ? "ON" : "OFF", BLINK_GPIO, CONFIG_BLINK_PERIOD, xPortGetCoreID());
    }
}

void RNG(void *arg)
{
    //Random Number Generator

    while(1){
        uint8_t random_number = esp_random();
        vTaskDelay(CONFIG_RAN_GEN_PERIOD/ portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Generated random number: %d on core: %d", random_number, xPortGetCoreID()); 
    }
}

void NN(void *arg)
{
    //Neural Network
    float inputs[4][2] = {{0,0},{0,1},{1,0},{1,1}};
    float desired[4][1] = {{0},{1},{1},{0}};


    while(1){
       //predictions 
        for (int i=0; i<4; i++){
            //default argument prmotion in printf function
            ESP_LOGI(TAG, "Output desired: %.2f", desired[i][0]); 
        } 

        uint8_t random_number = esp_random();
        vTaskDelay(CONFIG_RAN_GEN_PERIOD/ portTICK_PERIOD_MS);
        ESP_LOGI(TAG, "Generated random number: %d on core: %d", random_number, xPortGetCoreID()); 
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Configuring blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

static void blink_led(void){
    // Set the GPIO level according to the state
    gpio_set_level(BLINK_GPIO, s_led_state);
}
