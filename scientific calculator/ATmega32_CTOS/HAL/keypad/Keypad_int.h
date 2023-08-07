/*
 * Keypad_int.h
 *
 *  Created on: Jul 24, 2023
 *      Author: Raghad
 */

#ifndef KEYPAD_INT_H_
#define KEYPAD_INT_H_

#define NOT_PRESSED 	0xff

ES_t Keypad_enuInit();

ES_t Keypad_enuGetPressedKey(u8* Copy_pu8Key);


#endif /* KEYPAD_INT_H_ */
