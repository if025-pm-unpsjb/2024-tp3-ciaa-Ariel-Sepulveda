/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "FreeRTOS.h"
#include "task.h"
#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI
#include "semphr.h"

#define mainMAX_MSG_LEN                     25

#define TASK1_PERIOD 	4000
#define TASK2_PERIOD 	6000
#define TASK3_PERIOD 	9000
#define TASK4_PERIOD 	11000

#define TASK1_WCET		1000
#define TASK2_WCET		2000
#define TASK3_WCET		1000
#define TASK4_WCET		2000

void task(void *p);

SemaphoreHandle_t s;



static void prvTask( void* pvParameters );

struct xTaskStruct {
	TickType_t wcet;
	TickType_t period;
	pwmMap_t led;
};

typedef struct xTaskStruct xTask;

xTask task1 = { TASK1_WCET, TASK1_PERIOD, CIAA_BOARD_LED };
xTask task2 = { TASK2_WCET, TASK2_PERIOD, PWM7};
xTask task3 = { TASK3_WCET, TASK3_PERIOD, PWM8 };
xTask task4 = { TASK4_WCET, TASK4_PERIOD, PWM9 };

int main( void )
{

	    uartConfig( UART_USB, 9600);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        pwmConfig(0, PWM_ENABLE);
        //pwmConfig(PWM0, PWM_ENABLE_OUTPUT);
        pwmConfig(PWM7, PWM_ENABLE_OUTPUT);
        pwmConfig(PWM8, PWM_ENABLE_OUTPUT);
        pwmConfig(PWM9, PWM_ENABLE_OUTPUT);

    	s = xSemaphoreCreateMutex();

        xTaskCreate( prvTask, "T1", 256, (void*) &task1, configMAX_PRIORITIES - 1, NULL );
        xTaskCreate( prvTask, "T2", 256, (void*) &task2, configMAX_PRIORITIES - 2, NULL );
        xTaskCreate( prvTask, "T3", 256, (void*) &task3, configMAX_PRIORITIES - 3, NULL );
        xTaskCreate( prvTask, "T4", 256, (void*) &task4, configMAX_PRIORITIES - 4, NULL );
        vTaskStartScheduler();
        for(;;);
}


static void vBusyWait( TickType_t ticks)
{
    TickType_t elapsedTicks = 0;
    TickType_t currentTick = 0;
    while ( elapsedTicks < ticks ) {
        currentTick = xTaskGetTickCount();
        while ( currentTick == xTaskGetTickCount() ) {
            __asm__("nop");
        }
        elapsedTicks++;
    }
}


void prvTask( void *pvParameters )
{
	char cMessage[ mainMAX_MSG_LEN ];
	unsigned int uxReleaseCount = 0;
	TickType_t pxPreviousWakeTime = 0;
	xTask *task = (xTask*) pvParameters;

	for( ;; )
	{

        if (pwmRead(PWM7) == 255 && task->led != PWM7) {
            pwmWrite(PWM7, 64);
        }

		if (pwmRead(PWM8) == 255 && task->led != PWM8) {
	        pwmWrite(PWM8, 64);
	    }

		if (pwmRead(PWM9) == 255 && task->led != PWM9) {
            pwmWrite(PWM9, 64);
        }


		if (task->led == CIAA_BOARD_LED) {
            gpioWrite( CIAA_BOARD_LED, 1 );
		}
        pwmWrite(task->led, 255);


        sprintf( cMessage, "%s - %u - S - %u \n\r", pcTaskGetTaskName( NULL ), uxReleaseCount, xTaskGetTickCount());
        uartWriteString( UART_USB, cMessage);

        vBusyWait( (task->wcet) * 0.9 );
        pwmWrite(task->led, 0);
        if (task->led == CIAA_BOARD_LED) {
        	gpioWrite( CIAA_BOARD_LED, 0 );
        }

        sprintf( cMessage, "%s - %u - E - %u \n\r", pcTaskGetTaskName( NULL ), uxReleaseCount, xTaskGetTickCount());
        uartWriteString( UART_USB, cMessage);

        if (pwmRead(PWM7) == 64 && task->led != PWM7) {
			pwmWrite(PWM7, 255);
		}

		if (pwmRead(PWM8) == 64 && task->led != PWM8) {
			pwmWrite(PWM8, 255);
		}

		if (pwmRead(PWM9) == 64 && task->led != PWM9) {
			pwmWrite(PWM9, 255);
		}

		vTaskDelayUntil( &pxPreviousWakeTime, task->period );

		uxReleaseCount += 1;
	}

	vTaskDelete( NULL );
}

void task(void *p)
{
		TickType_t xPeriod = 10;
		int dutyCicle = 0;
        while( TRUE ) {
                pwmWrite(PWM7, dutyCicle);
                vTaskDelay( xPeriod );
                dutyCicle = ( dutyCicle + 1 ) % 255;
        }
}
