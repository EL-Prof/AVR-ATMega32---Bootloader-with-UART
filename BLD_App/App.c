/*
 * App.c
 *
 *  Created on: Jun 14, 2020
 *      Author: elProf
 */
#include "avr/io.h"
#include "macros.h"
#include "avr/eeprom.h"
#include "avr/interrupt.h"
#define F_CPU 12000000UL
#include "avr/delay.h"


#define REQ_FROM_APP_ADDRESS	((uint8_t*) 0x01)

void LOC_vidHandleUART_Rx(void);


uint8_t au8RxBuffer[255] ;
volatile uint8_t u8ProgrammingReq = 0 ;

#define USE_INT 1

void main(void)
{
	DDRA = 0xFF;
	PORTA = 0x01;

	/*******UART Init*******/
  // Set BaudRate -> 9600/12MhZ
  UBRRL = 77;
  UBRRH = 0;
  // Set Frame Format -> 8 data, 1 stop, No Parity
  UCSRC = 0x86;
  // Enable RX and TX and Rx interrupt
  UCSRB = 0x18;

#if USE_INT == 1
  UCSRB |= 0x80 ;
  /*Enable Global interrupt*/
  asm("sei");
#endif


	while(1)
	{
#if USE_INT == 0
		LOC_vidHandleUART_Rx();
#endif

		/*Blinking LED*/
		PORTA ^= 0x03;
		_delay_ms(1000);

		if(u8ProgrammingReq==1)
		{
			/*Update App Request Byte in EEPROM */
			eeprom_update_byte (REQ_FROM_APP_ADDRESS, 1);
			/*Jump to BLD*/
			asm("jmp 0x7000");
		}

	}
}

#define IDLE 0
#define RUNNING 1

/*UART Rx complete Vector*/
#if USE_INT == 1
ISR(_VECTOR(13))
{
	static uint8_t RxState = IDLE ;
	static uint8_t bufferIdx = 0 , len;

	if(RxState == IDLE)
	{
		len = UDR ;
		RxState = RUNNING ;
	}
	else
	{
		au8RxBuffer[bufferIdx] = UDR ;
		bufferIdx ++ ;
		if(bufferIdx == len)
		{
			/*all data is received */
			bufferIdx = 0 ;
			RxState = IDLE ;

			/*Check if programming session*/
			if( (len == 2) && (au8RxBuffer[0] == 0x10) &&(au8RxBuffer[1]==0x03) )
			{
				u8ProgrammingReq = 1 ;
			}
		}
	}
}

#else

void LOC_vidHandleUART_Rx(void)
{
	static uint8_t RxState = IDLE ;
	static uint8_t bufferIdx = 0 , len;

	if( (get_bit(UCSRA , 7)) == 1)
	{
		if(RxState == IDLE)
		{
			len = UDR ;
			RxState = RUNNING ;
		}
		else
		{
			au8RxBuffer[bufferIdx] = UDR ;
			bufferIdx ++ ;
			if(bufferIdx == len)
			{
				/*all data is received */
				bufferIdx = 0 ;
				RxState = IDLE ;

				/*Check if programming session*/
				if( (len == 2) && (au8RxBuffer[0] == 0x10) &&(au8RxBuffer[1]==0x03) )
				{
					u8ProgrammingReq = 1 ;
				}
			}
		}
		clr_bit(UCSRA , 7);
	}
}
#endif


