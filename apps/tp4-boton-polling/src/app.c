/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

int main( void )
{
		gpioMap_t keys[] = {TEC1, TEC2, TEC3, TEC4};
		gpioMap_t leds[] = {CIAA_BOARD_LED, LED1, LED2, LED3};


		int ledValues[] = {0, 0, 0, 0};
		int actualLedValue;

		gpioConfig(TEC1, GPIO_ENABLE);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while (TRUE) {

        	for (int i = 0; i < 4; i++) {

				if (gpioRead(keys[i]) == 0) {
					gpioToggle(leds[i]);
				}
				actualLedValue = gpioRead(keys[i]);
        	}
        }

        for(;;);
}
