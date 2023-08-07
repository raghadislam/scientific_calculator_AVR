/*
 * Keypad_config.h
 *
 *  Created on: Jul 24, 2023
 *      Author: Raghad
 */

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#include "../../MCAL/DIO/DIO_int.h"


#define DEFAULT_KEY		255

// Keypad type
#define NUM_OF_COL 10
#define NUM_OF_ROW 4

#define KPD_BUTTONS { {'c', '7', '8', '9', '*', '/',     ';', 'r', 'b', 'e'},\
                      {'^', '4', '5', '6', '-', 's',     'y', '[', 'v', 'w'},\
                      {'&', '1', '2', '3', '+', 'C',     'u', 'p', 'x', 'q'},\
                      {'!', '0', '.', '=', '+', 't',     'i', 'o', 'z', 'a'}, }

// columns pins & ports

#define KPD_COL0_PORT 	DIO_u8PORTA
#define KPD_COL1_PORT 	DIO_u8PORTA
#define KPD_COL2_PORT 	DIO_u8PORTA
#define KPD_COL3_PORT 	DIO_u8PORTA
#define KPD_COL4_PORT 	DIO_u8PORTC
#define KPD_COL5_PORT 	DIO_u8PORTC

#define KPD_COL6_PORT 	DIO_u8PORTD
#define KPD_COL7_PORT 	DIO_u8PORTD
#define KPD_COL8_PORT 	DIO_u8PORTD
#define KPD_COL9_PORT 	DIO_u8PORTD



#define KPD_COL0_PIN 	DIO_u8PIN4
#define KPD_COL1_PIN 	DIO_u8PIN5
#define KPD_COL2_PIN 	DIO_u8PIN6
#define KPD_COL3_PIN 	DIO_u8PIN7
#define KPD_COL4_PIN 	DIO_u8PIN5
#define KPD_COL5_PIN 	DIO_u8PIN6

#define KPD_COL6_PIN 	DIO_u8PIN7
#define KPD_COL7_PIN 	DIO_u8PIN6
#define KPD_COL8_PIN 	DIO_u8PIN5
#define KPD_COL9_PIN 	DIO_u8PIN4


#define KPD_COL_PORTS 	{KPD_COL0_PORT, KPD_COL1_PORT, KPD_COL2_PORT, KPD_COL3_PORT, KPD_COL4_PORT, KPD_COL5_PORT,\
	                     KPD_COL6_PORT, KPD_COL7_PORT, KPD_COL8_PORT, KPD_COL9_PORT}

#define KPD_COL_PINS 	{KPD_COL0_PIN, KPD_COL1_PIN, KPD_COL2_PIN, KPD_COL3_PIN, KPD_COL4_PIN, KPD_COL5_PIN,\
	                     KPD_COL6_PIN, KPD_COL7_PIN, KPD_COL8_PIN, KPD_COL9_PIN}

// Rows pins & ports


#define KPD_ROW0_PORT 	DIO_u8PORTB
#define KPD_ROW1_PORT 	DIO_u8PORTB
#define KPD_ROW2_PORT 	DIO_u8PORTB
#define KPD_ROW3_PORT 	DIO_u8PORTB


#define KPD_ROW0_PIN 	DIO_u8PIN7
#define KPD_ROW1_PIN 	DIO_u8PIN6
#define KPD_ROW2_PIN 	DIO_u8PIN5
#define KPD_ROW3_PIN 	DIO_u8PIN4


#define KPD_ROW_PORTS 	{KPD_ROW0_PORT, KPD_ROW1_PORT, KPD_ROW2_PORT, KPD_ROW3_PORT}
#define KPD_ROW_PINS 	{KPD_ROW0_PIN, KPD_ROW1_PIN, KPD_ROW2_PIN, KPD_ROW3_PIN}



#endif /* KEYPAD_CONFIG_H_ */
