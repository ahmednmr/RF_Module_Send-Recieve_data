/*
 * main.c
 *
 *  Created on: 5 Oct 2015
 *      Author: EmbeddedFab
 */

#include <avr/io.h>
#include <avr/delay.h>
#include <stdio.h>
#include <string.h>
#include "HAL/Nrf24.h"
#include "MCAL/UART.h"


char temp;
char q = 0;
unsigned int i=0;
char data_received[11];
char data_send[11]="Ahmed Saleh";
char tx_address[5] = {0xE7,0xE7,0xE7,0xE7,0xE7};
char rx_address[5] = {0xD7,0xD7,0xD7,0xD7,0xD7};



int main()
{

	init_UART();
	_delay_ms(100);
	UART_SEND_string("System init\r\n");
	init_nrf24();


	/* Channel #2 , payload length: 4 */
	nrf24_config(2,11);

	nrf24_tx_address(tx_address);
	nrf24_rx_address(rx_address);

	UART_SEND_string("Start Prog\r\n");

	while(1)
	{
		i=0;

		while(i<80)
		{

			if(nrf24_dataReady())
			{
				nrf24_getData(data_received);
				UART_SEND_string("Data recieved ----->>  ");

				UART_SendChar(data_received[0]);
				UART_SendChar(data_received[1]);
				UART_SendChar(data_received[2]);
				UART_SendChar(data_received[3]);
				UART_SendChar(data_received[4]);
				UART_SendChar(data_received[5]);
				UART_SendChar(data_received[6]);
				UART_SendChar(data_received[7]);
				UART_SendChar(data_received[8]);
				UART_SendChar(data_received[9]);
				UART_SendChar(data_received[10]);
				UART_SEND_string("\r\n\r\n");
			}

			_delay_ms(100);
			i++;
		}


		UART_SEND_string("send the array\r\n");
		/* Automatically goes to TX mode */
		nrf24_send(data_send);

		/* Wait for transmission to end */
		while(nrf24_isSending());
		UART_SEND_string("transmision has been ended\r\n");

		/* Make analysis on last tranmission attempt */
		temp = nrf24_lastMessageStatus();

		if(temp == NRF24_TRANSMISSON_OK)
		{
			UART_SEND_string("Tranmission went OK\r\n");

		}
		else if(temp == NRF24_MESSAGE_LOST)
		{
			UART_SEND_string("Message is lost ...\r\n");

		}


		/* Retranmission count indicates the tranmission quality */
		temp = nrf24_retransmissionCount();
		UART_SEND_string("Retranmission count:");
		UART_SEND_number(temp);
		UART_SEND_string("\r\n");


		/* Optionally, go back to RX mode ... */
		nrf24_powerUpRx();
		_delay_ms(100);




		/* Or you might want to power down after TX */
		//		 nrf24_powerDown();

		/* Wait a little ... */
		//		_delay_ms(1000);
		UART_SEND_string("End\r\n");
	}
	return 0;
}


