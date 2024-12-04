/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "FreeRTOS.h"
#include "task.h"


static void prvTask( void* pvParameters );

gpioMap_t keys[] = {TEC1, TEC2, TEC3, TEC4};
gpioMap_t leds[] = {CIAA_BOARD_LED, LED1, LED2, LED3};

int main( void )
{

	gpioConfig(TEC1, GPIO_ENABLE);

	boardConfig();

	xTaskCreate( prvTask, "T1", 256, NULL, configMAX_PRIORITIES, NULL );
	vTaskStartScheduler();

	for(;;);
}


void prvTask()
{
	TickType_t pxPreviousWakeTime = 0;
	int ledValues[] = {0, 0, 0, 0};
	int actualLedValue;

	for( ;; )
		{
		for (int i = 0; i < 4; i++) {

			if (gpioRead(keys[i]) == 0) {
				gpioToggle(leds[i]);
			}
			actualLedValue = gpioRead(keys[i]);
		}
		vTaskDelayUntil(&pxPreviousWakeTime, 100);
	}
	vTaskDelete( NULL );

}
