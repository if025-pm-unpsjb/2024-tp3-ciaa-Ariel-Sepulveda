/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

void myFunc(void*);
gpioMap_t leds[] = {CIAA_BOARD_LED, LED1, LED2, LED3};

int main( void )
{

	    uartConfig( UART_USB, 9600);

	    uartInterrupt(UART_USB, TRUE);
	    uartCallbackSet(UART_USB, UART_RECEIVE, myFunc, NULL);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        for(;;);
}

void myFunc(void*) {

	int value = uartRxRead(UART_USB);

	int idx = value - '1';

	uartWriteByte(UART_USB, value);
	if (idx >= 0 && idx <= 3) {
		gpioToggle(leds[idx]);
	}

}
