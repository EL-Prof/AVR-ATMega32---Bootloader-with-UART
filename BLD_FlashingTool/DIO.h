/*
 * DIO.h
 *
 *  Created on: Aug 31, 2018
 *      Author: asus
 */





#ifndef DIO_H_
#define DIO_H_

/*Define port number */
#define PORTA ((u8)1)
#define PORTB ((u8)2)
#define PORTC ((u8)3)
#define PORTD ((u8)4)


#define HIGH   1
#define LOW    0


/*Define port Dir */
#define OUTPUT  ((u8)1)
#define INPUT   ((u8)0)


#define PIN0  ((u8)0x00)
#define PIN1   ((u8)0x01)
#define PIN2   ((u8)0x02)
#define PIN3  ((u8)0x03)
#define PIN4   ((u8)0x04)
#define PIN5   ((u8)0x05)
#define PIN6  ((u8)0x06)
#define PIN7   ((u8)0x07)
#define PIN8   ((u8)0x08)






void DIO_vidSetPortDir(u8 u8portName, u8 u8PortDir);
void DIO_vidSetPinDir(u8 u8portName, u8 u8pinNo , u8 u8PortDir);
void DIO_vidSetPortValue (u8 u8PortName , u8 u8PortValue);
void DIO_vidSetPinValue (u8 u8portName , u8 u8pinNo , u8 u8pinValue );
u8 DIO_vidGetPortValue(u8 u8PortName);
u8 DIO_u8GetPinaValue (u8 u8PortName , u8 u8PinNo );

#endif /* DIO_H_ */
