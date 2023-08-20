/*
 * LCD_priv.h
 *
 *  Created on: Jul 22, 2023
 *      Author: Raghad Islam
 */

#ifndef LCD_PRIV_H_
#define LCD_PRIV_H_

#define EIGHT_BIT	99
#define FOUR_BIT	98

#define RIGHT_TO_LEFT 0x06
#define LEFT_TO_RIGHT 0x04

#define POS_0 		  0x80
#define POS_1 		  0x81
#define POS_2 		  0x82
#define POS_3 		  0x83
#define POS_4 		  0x84
#define POS_5 		  0x85
#define POS_6 		  0x86
#define POS_7 		  0x87
#define POS_8 		  0x88
#define POS_9 		  0x89
#define POS_10 		  0x8A
#define POS_11 		  0x8B
#define POS_12 		  0x8C
#define POS_13 		  0x8D
#define POS_14 		  0x8E
#define POS_15 		  0x8F



static inline ES_t LCD_inenuSendCommand(u8 Copy_u8Command);

static ES_t LCD_inenuLatch( u8 Copy_u8Data );

#endif /* LCD_PRIV_H_ */
