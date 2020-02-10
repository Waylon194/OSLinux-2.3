#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void printName(void *pvParameters)
{
    UBaseType_t name = uxTask(NULL);
    TaskStatus_t xTaskDetails;

    xHandle = xTaskGetHandle(name);

    configASSERT( xHandle );
    printf("Name \n" + vTaskGetInfo(xHandle,&xTaskDetails,pdTRUE,eInvalid));
}

void app_main()
{
    for (int = 0; i < 20; i++){
        xTaskCreate(printName(), "Print_Name", 256, NULL, 5, NULL);
    }
}