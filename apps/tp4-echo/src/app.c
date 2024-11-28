/*
 * main.c
 *
 *  Created on: 13 oct. 2022
 *      Author: fep
 */

#include "app.h"         // <= Su propia cabecera (opcional)
#include "sapi.h"        // <= Biblioteca sAPI

#define mainMAX_MSG_LEN 25

int main( void )
{
		char cMessage[ mainMAX_MSG_LEN ];
		int messageIndex;
		uint8_t receivedByte;
		bool_t input;

	    uartConfig( UART_USB, 9600);

        // Inicializar y configurar la placa de desarrollo
        boardConfig();

        while (TRUE) {

        	input = uartReadByte( UART_USB, &receivedByte);
        	if (input == FALSE)
        		continue;

        	cMessage[messageIndex++] = receivedByte;
        	if (receivedByte == '\r') {
        		cMessage[messageIndex++] = '\0';
        		uartWriteString(UART_USB, cMessage);
        		messageIndex = 0;
        		for (int i = 0; i < 25; i++) {
        			cMessage[i] = 0;
        		}
        	}

        }

        for(;;);
}
