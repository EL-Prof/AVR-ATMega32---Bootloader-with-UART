

#ifndef UART_H_
#define UART_H_

void UART_Init(void);

void UART_SendChar(u8 data);

u8 UART_GetChar(void);

void UART_vidRxHandler(void);

extern void flashing_vidRxNotification(u8* pu8Data , u8 u8len);


/*** configuration **/
#define USE_INTERRUPT	1


#endif /* UART_H_ */
