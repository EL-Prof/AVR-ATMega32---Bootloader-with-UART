/*
 * macros.h
 *
 *  Created on: Aug 11, 2018
 *      Author: hamed
 */

#ifndef MACROS_H_
#define set_bit(Data,Bit) Data|= (1<<Bit)
#define clr_bit(Data,Bit) Data&= ~(1<<Bit)
#define toggle_bit(Data,Bit) Data^= (1<<Bit)
#define get_bit(Data,Bit) (Data>>Bit)&1
#define assign_bit(Data,Bit,Val) (Val==1)? (set_bit(Data,Bit)):(clr_bit(Data,Bit))
#define MACROS_H_



#endif /* MACROS_H_ */
