/*
 * LCD_int.h
 *
 *  Created on: Jul 22, 2023
 *      Author: DeLL
 */

#ifndef LCD_INT_H_
#define LCD_INT_H_



#define RIGHT_TO_LEFT 0x06
#define LEFT_TO_RIGHT 0x04

ES_t LCD_enuInit(void);

ES_t LCD_enuSendChar(u8 Copy_u8Data);

ES_t LCD_enuSendCommand(u8);

ES_t LCD_enuClearDisplay(void);

//ES_t LCD_enuCreatChar(u8* Copy_Au8NewChar, u8 Copy_u8Size, u8 Copy_u8StartPattern );

ES_t LCD_enuClearDisplay(void);

ES_t LCD_enuDisplayString(u8* Copy_pu8String);

ES_t LCD_enuGoto(u8 Copy_u8Line,u8 Copy_u8Block);

ES_t LCD_enuWriteNumber(f32 Copy_u8Num);





#endif /* LCD_INT_H_ */
