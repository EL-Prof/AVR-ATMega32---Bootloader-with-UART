#ifndef _STD_TYPES_H
#define _STD_TYPES_H

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;

typedef signed char s8;
typedef signed short int s16;
typedef signed long int s32;

typedef float f32;
typedef double f64;

typedef  u8 boolean ;

#define TRUE	((boolean)0x01)
#define FALSE	((boolean)0x00)

#  define ISR(vector)            \
    void vector (void) __attribute__ ((signal)); \
    void vector (void)

#define vect(x)		__vector_##x


#endif
