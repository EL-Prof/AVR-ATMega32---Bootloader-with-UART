#include "STD_Types.h"
#include "macros.h"
#include "App.h"
#include "avr/eeprom.h"
#include "UART.h"
#include "DIO.h"
#include "Flashing_Mngr.h"




char eedata[] EEMEM = {0x00,0x00};

int main(void)
{
	u8 validAPP_Val, reqFromApp_Val;
	validAPP_Val = eeprom_read_byte (VALID_APP_ADDRESS);
	reqFromApp_Val = eeprom_read_byte (REQ_FROM_APP_ADDRESS);

	if ( (validAPP_Val != 1) | (reqFromApp_Val == 1))
	{
		/*for debugging*/
		DIO_vidSetPinDir(PORTA , PIN7 , OUTPUT);
		DIO_vidSetPinValue(PORTA , PIN7 , HIGH);


		UART_Init();
		DIO_vidSetPinDir(PORTD , PIN0 , INPUT);
		DIO_vidSetPinDir(PORTD , PIN1 , OUTPUT);
#if USE_INTERRUPT == 1
		/*Move Vector Table to boot loader section*/
		APP_vidMoveIVT(BLD_SECTION);
#endif

		if(reqFromApp_Val == 1)
		{
			flashingMngr_vidHandleReqFromApp() ;
			/*clr application request*/
			eeprom_update_byte (REQ_FROM_APP_ADDRESS, 0);
		}

		while(1)
		{
		#if USE_INTERRUPT == 0
			UART_vidRxHandler();
		#endif
			flashingMngr_vidMainTask() ;
		}
	}
	else
	{
		/*Valid app, jump directly*/
		asm("jmp 0");
	}

	return 1 ;
}

extern void APP_vidMoveIVT(u8 u8Section)
{
	/*Move IVt to Application [starting from 0]*/
	if(u8Section == APP_SECTION)
	{
		/* Enable change of interrupt vectors */
		GICR = (1<<IVCE);
		/* Move interrupts to Application section */
		clr_bit(GICR,IVSEL);
	}

	/*Move IVt to Bootloader [starting after end of app section]*/
	else
	{
		/* Enable change of interrupt vectors */
		GICR = (1<<IVCE);
		/* Move interrupts to boot Flash section */
		GICR = (1<<IVSEL);
	}
}


