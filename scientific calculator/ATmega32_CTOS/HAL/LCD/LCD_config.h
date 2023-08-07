/*
 * LCD_config.h
 *
 *  Created on: Jul 22, 2023
 *      Author: DeLL
 */

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#include "../../MCAL/DIO/DIO_int.h"

// LCD Mode

#define LCD_MODE	EIGHT_BIT

#define DISPLAY_DIRECTION	LEFT_TO_RIGHT

//#define DISPLAY_LOCATION	POS_15 // from POS_0 to POS_15 for each line

// Control Pins

#define RS_PIN		DIO_u8PIN0
#define RS_PORT		DIO_u8PORTA

#define RW_PIN		DIO_u8PIN1
#define RW_PORT		DIO_u8PORTA

#define EN_PIN		DIO_u8PIN3
#define EN_PORT		DIO_u8PORTD

// Data Pins

#define DB0_PIN		DIO_u8PIN0
#define DB0_PORT	DIO_u8PORTB

#define DB1_PIN		DIO_u8PIN1
#define DB1_PORT	DIO_u8PORTB

#define DB2_PIN		DIO_u8PIN0
#define DB2_PORT	DIO_u8PORTC

#define DB3_PIN		DIO_u8PIN1
#define DB3_PORT	DIO_u8PORTC

#define DB4_PIN		DIO_u8PIN0
#define DB4_PORT	DIO_u8PORTD

#define DB5_PIN		DIO_u8PIN1
#define DB5_PORT	DIO_u8PORTD

#define DB6_PIN		DIO_u8PIN2
#define DB6_PORT	DIO_u8PORTA

#define DB7_PIN		DIO_u8PIN2
#define DB7_PORT	DIO_u8PORTC


#endif /* LCD_CONFIG_H_ */
