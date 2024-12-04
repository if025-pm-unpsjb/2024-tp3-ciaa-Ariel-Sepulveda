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
		gpioMap_t leds[] = {CIAA_BOARD_LED, LED1, LED2, LED3};
		int idx;
		uint8_t receivedByte;
		bool_t input;
		bool_t value;

	    uartConfig( UART_USB, 9600);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while (TRUE) {

        	input = uartReadByte( UART_USB, &receivedByte);
        	if (input == FALSE)
        		continue;

        	idx = receivedByte - '1';

        	uartWriteByte(UART_USB, receivedByte);
        	if (idx >= 0 && idx <= 3) {
				gpioToggle(leds[idx]);
        	}
        }

        for(;;);
}
