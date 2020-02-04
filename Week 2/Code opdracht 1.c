#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void hello_task(void *pvParameters)
{
    TickType_t lastWait = xTaskGetTickCount();
    int waitTime = 3000 / portTICK_RATE_MS;
    while(1){
        UBaseType_t priority = uxTaskPriorityGet(NULL);
        for (int i = 0; i < 2; i++){
            vTaskDelayUntil(&lastWait, waitTime);
            printf("weer 3 seconden: %d\n", priority);
        }
        vTaskPrioritySet(NULL, priority - 1);
        if (uxTaskPriorityGet(NULL) == 1){
            vTaskDelete(NULL);
        }
    }
}

void app_main()
{
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;
    xReturned = xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    if( xReturned == pdPASS )
    {
        vTaskDelete( xHandle );
    }
}