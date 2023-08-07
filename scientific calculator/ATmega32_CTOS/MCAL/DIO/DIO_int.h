/*
 * DIO_int.h
 *
 *  Created on: Jul 15, 2023
 *      Author: Raghad Islam
 */

#ifndef DIO_INT_H_
#define DIO_INT_H_

#define DIO_u8PORTA   0
#define DIO_u8PORTB   1
#define DIO_u8PORTC   2
#define DIO_u8PORTD   3

#define DIO_u8PIN0   0
#define DIO_u8PIN1   1
#define DIO_u8PIN2   2
#define DIO_u8PIN3   3
#define DIO_u8PIN4   4
#define DIO_u8PIN5   5
#define DIO_u8PIN6   6
#define DIO_u8PIN7   7

ES_t DIO_enuInit(void);

ES_t DIO_enuSetPortDir (u8 Copy_u8PortID , u8 Copy_u8Val );

ES_t DIO_enuSetPortVal( u8 Copy_u8PortID , u8 Copy_u8Val );

ES_t DIO_enuTogglePort( u8 Copy_u8PortID );

ES_t DIO_enuGetPortVal( u8 Copy_u8PortID , u8* Copy_pu8Val );


ES_t DIO_enuSetPinDir( u8 Copy_u8PortID , u8 Copy_u8PinID , u8 Copy_u8Val );

ES_t DIO_enuSetPinVal( u8 Copy_u8PortID , u8 Copy_u8PinID , u8 Copy_u8Val );

ES_t DIO_enuTogglePin( u8 Copy_u8PortID , u8 Copy_u8PinID  );

ES_t DIO_enuGetPinVal( u8 Copy_u8PortID , u8 Copy_u8PinID , u8* Copy_pu8Val );


#define DIO_u8OUTPUT  1
#define DIO_u8INPUT   0

#define DIO_u8HIGH    1
#define DIO_u8LOW     0

#define DIO_u8FLOAT   0
#define DIO_u8PULL_UP 1

#define DIO_u8DIO_MASK_BIT 1


#endif /* DIO_INT_H_ */
