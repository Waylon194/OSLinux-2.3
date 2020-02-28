#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include <pthread.h> 
#include "freertos/semphr.h"
#include "freertos/queue.h"

QueueHandle_t queue;
SemaphoreHandle_t xMutex;
SemaphoreHandle_t writeSemaphore;
SemaphoreHandle_t readSemaphore;
void writer(void* pvParameter);
void reader(void* pvParameter);
int readerAmount = 0;

void app_main(void)
{
    queue = xQueueCreate(5, sizeof(uint32_t));
    writeSemaphore = xSemaphoreCreateBinary();
    xMutex = xSemaphoreCreateMutex();
    readSemaphore = xSemaphoreCreateBinary();
    writeSemaphore = xSemaphoreCreateCounting(10, 0);
    xMutex = xSemaphoreCreateCounting(10, 1);
    readSemaphore = xSemaphoreCreateCounting(10, 10);

    xTaskCreate(&writer, "Writer", 2048, NULL, 1, NULL);
    xTaskCreate(&writer, "Writer", 2048, NULL, 1, NULL);
    xTaskCreate(&reader, "Reader", 2048, NULL, 2, NULL);
    xTaskCreate(&reader, "Reader", 2048, NULL, 2, NULL);
    xTaskCreate(&reader, "Reader", 2048, NULL, 2, NULL);
}

void writer(void* pvParameter)
{
    TickType_t lastWait = xTaskGetTickCount();
    int waitTime = 5000 / portTICK_RATE_MS;
    do
    {
        uint32_t DataToSend = 32;
        const TickType_t xBlockTime = pdMS_TO_TICKS(500);
        xSemaphoreTake(writeSemaphore, xBlockTime);
        printf("Writing a message...\n");
        xQueueSend(queue, &DataToSend, xBlockTime);
        xSemaphoreGive(writeSemaphore);
    } while (1);
    vTaskDelayUntil(&lastWait, waitTime);
}

void reader(void* pvParameter)
{
    TickType_t lastWait = xTaskGetTickCount();
    int waitTime = 5000 / portTICK_RATE_MS;
    do
    {
        uint32_t DataToReceive = 16;
        const TickType_t xBlockTime = pdMS_TO_TICKS(500);
        xSemaphoreTake(readSemaphore, xBlockTime);
        readerAmount++;

        if (readerAmount == 1)
        {
            xSemaphoreTake(writeSemaphore, xBlockTime);
        }
        xSemaphoreGive(readSemaphore);
        printf("Reading a message...\n");
        xQueueReceive(queue, &DataToReceive, xBlockTime);
        xSemaphoreTake(readSemaphore, xBlockTime);
        readerAmount--;

        if (readerAmount == 0)
        {
            xSemaphoreGive(writeSemaphore);
        }
        xSemaphoreGive(readSemaphore);
    } while (1);

    vTaskDelayUntil(&lastWait, waitTime);
}
