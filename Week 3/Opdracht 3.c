#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

QueueHandle_t queue;
SemaphoreHandle_t xMutex;
SemaphoreHandle_t emptySemaphore;
SemaphoreHandle_t fullSemaphore;
void producer(void* pvParameter);
void consumer(void* pvParameter);

void app_main(void)
{
    queue = xQueueCreate(5, sizeof(uint32_t));
    emptySemaphore = xSemaphoreCreateBinary();
    xMutex = xSemaphoreCreateMutex();
    fullSemaphore = xSemaphoreCreateBinary();
    emptySemaphore = xSemaphoreCreateCounting(10, 0);
    xMutex = xSemaphoreCreateCounting(10, 1);
    fullSemaphore = xSemaphoreCreateCounting(10, 10);

    xTaskCreate(&producer, "producer", 2048, NULL, 5, NULL);
    xTaskCreate(&consumer, "consumer", 2048, NULL, 5, NULL);
}

void producer(void* pvParameter)
{
    do
    {
        uint32_t DataToSend = 32;
        const TickType_t xBlockTime = pdMS_TO_TICKS(500);
        xSemaphoreTake(emptySemaphore, xBlockTime);
        xSemaphoreTake(xMutex, xBlockTime);
        printf("Producing a message...\n");
        xQueueSend(queue, &DataToSend, xBlockTime);
        xSemaphoreGive(xMutex);
        xSemaphoreGive(fullSemaphore);
    } while (1);
}

void consumer(void* pvParameter)
{
    do
    {
        uint32_t DataToReceive = 16;
        const TickType_t xBlockTime = pdMS_TO_TICKS(500);
        xSemaphoreTake(fullSemaphore, portMAX_DELAY);
        xSemaphoreTake(xMutex, portMAX_DELAY);
        printf("Reading a message...\n");
        xQueueReceive(queue, &DataToReceive, xBlockTime);
        xSemaphoreGive(xMutex);
        xSemaphoreGive(emptySemaphore);
        xQueueReset(queue);
    } while (1);
}
