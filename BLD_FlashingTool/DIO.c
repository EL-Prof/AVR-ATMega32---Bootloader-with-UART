/*
 * DIO.c
 *
 *  Created on: Aug 31, 2018
 *      Author: asus
 */
#include "STD_Types.h"
#include "DIO.h"
#include "macros.h"

/*Local defines*/

/*PORTA Registers*/
#define DDRA_REGISTER  *((volatile u8*) 0x3A)
#define PORTA_REGISTER  *((volatile u8*) 0x3B)
#define PINA_REGISTER    *((volatile u8*) 0x39)

/*PORTB Registers*/
#define DDRB_REGISTER  *((volatile u8*) 0x37)
#define PORTB_REGISTER  *((volatile u8*) 0x38)
#define PINB_REGISTER    *((volatile u8*) 0x36)

/*PORTC Registers*/
#define DDRC_REGISTER  *((volatile u8*) 0x34)
#define PORTC_REGISTER  *((volatile u8*) 0x35)
#define PINC_REGISTER    *((volatile u8*) 0x33)

/*PORTD Registers*/
#define DDRD_REGISTER  *((volatile u8*) 0x31)
#define PORTD_REGISTER  *((volatile u8*) 0x32)
#define PIND_REGISTER    *((volatile u8*) 0x30)



void DIO_vidSetPortDir(u8 u8portName, u8 u8PortDir)
{
	switch (u8portName)
	{
		case PORTA :
		{
			if(u8PortDir == INPUT)
			{
				DDRA_REGISTER = 0x00 ;
			}
			else /*Direction is output*/
			{
				DDRA_REGISTER = 0xFF ;
			}
		}break;

		case PORTB :
		{
			if(u8PortDir == INPUT)
			{
				DDRB_REGISTER = 0x00 ;
			}
			else /*Direction is output*/
			{
				DDRB_REGISTER = 0xFF ;
			}
		}break;

		case PORTC :
		{
			if(u8PortDir == INPUT)
			{
				DDRC_REGISTER = 0x00 ;
			}
			else /*Direction is output*/
			{
				DDRC_REGISTER = 0xFF ;
			}
		}break;

		case PORTD :
		{
			if(u8PortDir == INPUT)
			{
				DDRD_REGISTER = 0x00 ;
			}
			else /*Direction is output*/
			{
				DDRD_REGISTER = 0xFF ;
			}
		}break;
	}
}



void DIO_vidSetPinDir(u8 u8portName, u8 u8pinNo , u8 u8PinDir){
	switch (u8portName)
	{
		case PORTA :
		{
			if (u8PinDir == INPUT)
			{
				clr_bit(DDRA_REGISTER ,u8pinNo);
			}
			else if (u8PinDir == OUTPUT)
			{
				set_bit(DDRA_REGISTER , u8pinNo );
			}
			else
			{
				/*Invalid Input*/
			}

		}break;

		case PORTB :
		{
			if (u8PinDir == INPUT)
			{
				clr_bit(DDRB_REGISTER ,u8pinNo);
			}
			else if (u8PinDir == OUTPUT)
			{
				set_bit(DDRB_REGISTER , u8pinNo );
			}
			else
			{
				/*Invalid Input*/
			}
		}break;

		case PORTC :
		{
			if (u8PinDir == INPUT)
			{
				clr_bit(DDRC_REGISTER ,u8pinNo);
			}
			else if (u8PinDir == OUTPUT)
			{
				set_bit(DDRC_REGISTER , u8pinNo );
			}
			else
			{
				/*Invalid Input*/
			}

		}break;
		case PORTD :
		{
			if (u8PinDir == INPUT)
			{
				clr_bit(DDRD_REGISTER ,u8pinNo);
			}
			else if (u8PinDir == OUTPUT)
			{
				set_bit(DDRD_REGISTER , u8pinNo );
			}
			else
			{
				/*Invalid Input*/
			}
		}break;
	}
}


void DIO_vidSetPortValue (u8 u8PortName , u8 u8PortValue)
{
	switch (u8PortName)
	{
		case PORTA :
		{
			PORTA_REGISTER = u8PortValue;
		}break;

		case PORTB :
		{
			PORTB_REGISTER = u8PortValue;
		}break ;

		case PORTC :
		{
			PORTC_REGISTER = u8PortValue;
		}break ;

		case PORTD :
		{
			PORTD_REGISTER = u8PortValue;
		}break ;
	}
}



void DIO_vidSetPinValue (u8 u8portName , u8 u8pinNo , u8 u8pinValue)
{
	switch (u8portName)
	{
		case PORTA :
		{
			assign_bit(PORTA_REGISTER,u8pinNo,u8pinValue) ;
		}break;

		case PORTB :
		{
			assign_bit(PORTB_REGISTER,u8pinNo,u8pinValue) ;
		}break ;

		case PORTC :
		{
			assign_bit(PORTC_REGISTER,u8pinNo,u8pinValue) ;
		}break ;

		case PORTD :
		{
			assign_bit(PORTD_REGISTER,u8pinNo,u8pinValue) ;
		}break;
	}
}



u8 DIO_vidGetPortValue(u8 u8PortName)
{
	u8 retVal ;
	switch(u8PortName)
	{
		case PORTA:
		{
			retVal = PINA_REGISTER;
		}break ;

		case PORTB:
		{
			retVal = PINB_REGISTER;
		}break ;

		case PORTC:
		{
			retVal = PINC_REGISTER;
		}break ;

		case PORTD:
		{
			retVal = PIND_REGISTER;
		}break ;
	}

	return retVal;
}



u8 DIO_u8GetPinaValue (u8 u8PortName , u8 u8PinNo )
{
	u8 Value ;
	switch (u8PortName)
	{
		case PORTA :
		{
			Value =  get_bit(PINA_REGISTER, u8PinNo);
		}break ;
		case PORTB :
		{
			Value =  get_bit(PINB_REGISTER, u8PinNo);
		}break ;
		case PORTC :
		{
			Value =  get_bit(PINC_REGISTER, u8PinNo);
		}break ;

		case PORTD :
		{
			Value =  get_bit(PIND_REGISTER, u8PinNo);
		}break ;
	}

	return Value ;
}













