#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

int philosopherNumber = 0;

void createPhilosipher(int i)
{
	TickType_t lastWait = xTaskGetTickCount();
	int waitTime = 5000 / portTICK_RATE_MS;

	while (1)
	{
		if (i == (philosopherNumber % 5) || i == ((philosopherNumber + 1) % 5))
		{
			printf("Philosopher %d is eating\n", i);
		}
		else
		{
			printf("Philosopher %d is NOT eating\n", i);
		}
		vTaskDelayUntil(&lastWait, waitTime);
	}
}

void passFork(void* pvParameters)
{
	TickType_t lastWait = xTaskGetTickCount();
	int waitTime = 5000 / portTICK_RATE_MS;

	while (1)
	{
		vTaskDelayUntil(&lastWait, waitTime);
		printf("Passed fork to %d and %d \n", philosopherNumber % 5, (philosopherNumber + 1) % 5);
		philosopherNumber += 2;
	}
}

void app_main()
{
	for (int i = 0; i < 5; i++)
	{
		xTaskCreate(&createPhilosipher, "createPhilosipher", 2048, i, 5, NULL);
	}

	xTaskCreate(&passFork, "passFork", 2048, NULL, 1, NULL);
}
