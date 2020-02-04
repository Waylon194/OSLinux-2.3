#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void printName(void *pvParameters)
{
    printf("Name \n");
}

void app_main()
{
    for (int = 0; i < 20; i++){
        xTaskCreate(printName(), "Groen", 256, NULL, 5, NULL);
    }
}