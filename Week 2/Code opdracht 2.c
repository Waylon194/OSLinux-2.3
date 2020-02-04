#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#define GREENGPIO 27
#define REDGPIO 33

void rood(void *pvParameters)
{
    TickType_t lastWait = xTaskGetTickCount();
    int waitTime = 1000 / portTICK_RATE_MS;

    gpio_pad_select_gpio(33);
    gpio_set_direction(33, GPIO_MODE_OUTPUT);
    while(1) {
        printf("Turning off red the LED\n");
        gpio_set_level(33, 0);
        vTaskDelayUntil(&lastWait, waitTime);
        printf("Turning on the red LED\n");
        gpio_set_level(33, 1);
        vTaskDelayUntil(&lastWait, waitTime);
    }
}

void groen(void *pvParameters)
{
    TickType_t lastWait = xTaskGetTickCount();
    int waitTime = 5000 / portTICK_RATE_MS;

    gpio_pad_select_gpio(GREENGPIO);
    gpio_set_direction(GREENGPIO, GPIO_MODE_OUTPUT);
    while(1) {
        printf("Turning off the green LED\n");
        gpio_set_level(GREENGPIO, 0);
        vTaskDelayUntil(&lastWait, waitTime);
        printf("Turning on the green LED\n");
        gpio_set_level(GREENGPIO, 1);
        vTaskDelayUntil(&lastWait, waitTime);
    }
}

void app_main()
{
    xTaskCreate(rood, "rood", 2048, NULL, 1, NULL);
    xTaskCreate(groen, "groen", 2048, NULL, 1, NULL);
}