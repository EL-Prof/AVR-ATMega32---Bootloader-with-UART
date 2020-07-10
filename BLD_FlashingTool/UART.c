
#include "STD_TYPES.h"
#include "macros.h"

#include "UART.h"

#include "DIO.h"

#define UBRRL_REGISTER	*((volatile u8*) 0x29)
#define UBRRH_REGISTER	*((volatile u8*) 0x40)
#define UCSRA_REGISTER	*((volatile u8*) 0x2B)
#define UCSRB_REGISTER	*((volatile u8*) 0x2A)
#define UCSRC_REGISTER	*((volatile u8*) 0x40)
#define UDR_REGISTER	*((volatile u8*) 0x2C)

#define IDLE 		0
#define RUNNING	1

#define UART_RX_COMPLETE	vect(13)


u8 u8len;

u8 au8RxBuffer[255] ;

void UART_Init(void)
{
  // Set BaudRate -> 9600/12MhZ
  UBRRL_REGISTER = 77;
  UBRRH_REGISTER = 0;
  // Set Frame Format -> 8 data, 1 stop, No Parity
  UCSRC_REGISTER = 0x86;
  // Enable RX and TX and Rx interrupt
  UCSRB_REGISTER = 0x18;

#if USE_INTERRUPT == 1
  UCSRB_REGISTER |= 0x80 ;
  /*Enable Global interrupt*/
  asm("sei");
#endif

}

void UART_SendChar(u8 data)
{

  // Wait until transmission Register Empty
  while((UCSRA_REGISTER&0x20) == 0x00);
  UDR_REGISTER = data;

}

u8 UART_GetChar(void)
{
  u8 Result;
  // Wait until Reception Complete
  while((UCSRA_REGISTER&0x80) == 0x00);
  Result = UDR_REGISTER;
  
  /* Clear Flag */
  set_bit(UCSRA_REGISTER,7);
  return Result;
}

#if USE_INTERRUPT == 1
ISR(UART_RX_COMPLETE)
{
	static u8 RxState = IDLE ;
	static u8 bufferIdx = 0 ;

	if(RxState == IDLE)
	{
		u8len = UDR_REGISTER ;
		RxState = RUNNING ;
	}
	else
	{
		au8RxBuffer[bufferIdx] = UDR_REGISTER ;
		bufferIdx ++ ;
		if(bufferIdx == u8len)
		{
			/*all data is received */
			bufferIdx = 0 ;
			RxState = IDLE ;

			/*call application callback*/
			flashing_vidRxNotification(au8RxBuffer , u8len) ;
		}
	}
}

#else

void UART_vidRxHandler(void)
{
	static u8 RxState = IDLE ;
	static u8 bufferIdx = 0 ;

	/*Check if a byte received*/
	if( (get_bit(UCSRA_REGISTER , 7)) == 1)
	{
		if(RxState == IDLE)
		{
			u8len = UDR_REGISTER ;
			RxState = RUNNING ;
		}
		else
		{
			au8RxBuffer[bufferIdx] = UDR_REGISTER ;
			bufferIdx ++ ;
			if(bufferIdx == u8len)
			{
				/*all data is received */
				bufferIdx = 0 ;
				RxState = IDLE ;
				/*call application callback*/
				flashing_vidRxNotification(au8RxBuffer , u8len) ;
			}
		}
		clr_bit(UCSRA_REGISTER , 7);
	}
}
#endif

