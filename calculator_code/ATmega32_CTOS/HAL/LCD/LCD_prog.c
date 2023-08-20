/*
 * LCD_prog.c
 *
 *  Created on: Jul 22, 2023
 *      Author: Raghad Islam
 */

#include "../../SERVICE/STDTypes.h"
#include "../../SERVICE/errorState.h"

#include "../../MCAL/DIO/DIO_int.h"

#include "LCD_config.h"
#include "LCD_priv.h"
#include <util/delay.h>

u8 Global_u8Flag = 0;
u8 Local_u8Iterator;


ES_t LCD_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	u64 Local_u64Check = 0;

	Local_u64Check |= (((u64)DIO_enuSetPinDir(RS_PORT, RS_PIN, DIO_u8OUTPUT)<<0 ));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(RW_PORT, RW_PIN, DIO_u8OUTPUT)<<3 ));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(EN_PORT, EN_PIN, DIO_u8OUTPUT)<<6 ));


	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB4_PORT, DB4_PIN, DIO_u8OUTPUT)<<9 ));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB5_PORT, DB5_PIN, DIO_u8OUTPUT)<<12));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB6_PORT, DB6_PIN, DIO_u8OUTPUT)<<15));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB7_PORT, DB7_PIN, DIO_u8OUTPUT)<<18));

	_delay_ms(40);

#if LCD_MODE == EIGHT_BIT


	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB0_PORT, DB0_PIN, DIO_u8OUTPUT)<<21 ));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB1_PORT, DB1_PIN, DIO_u8OUTPUT)<<24 ));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB2_PORT, DB2_PIN, DIO_u8OUTPUT)<<27 ));
	Local_u64Check |= (((u64)DIO_enuSetPinDir(DB3_PORT, DB3_PIN, DIO_u8OUTPUT)<<30 ));

	// Function Set
		Local_u64Check |= ((u64)LCD_inenuSendCommand(0x38) << 33); //2 lines, 5*7 ,8 bit mode
		_delay_ms(1);

		//Display ON
		Local_u64Check |= ((u64)LCD_inenuSendCommand(0x0f) << 36); //cursor appear, blink, LCD ON
		_delay_ms(1);

		//Display clear
		Local_u64Check |= ((u64)LCD_inenuSendCommand(0x01) << 39);
		_delay_ms(2);

		//Set Entry mode
		Local_u64Check |= ((u64)LCD_inenuSendCommand(0x06) << 42); // shift disabled , DDRAM address increase

		for(Local_u8Iterator = 0; Local_u8Iterator < 45; Local_u8Iterator+=3)
		{
			if(((Local_u64Check >> Local_u8Iterator) & (0b111)) != ES_OK)
			{
				return ES_NOK;
			}
		}

	Local_enuErrorState = ES_OK;

	// function set
	//Local_enuErrorState = LCD_inenuSendCommand(0x38); //2 lines, 5*7 ,8 bit mode


#elif LCD_MODE == FOUR_BIT

	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x20)<<21 ));
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x20)<<24 ));
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x80)<<27 )); //2 lines, 5*7 ,4 bit mode
	_delay_ms(1);
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x00)<<30 ));
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0xF0)<<33 )); //Display ON, Cursor ON ,Blink ON
	_delay_ms(1);
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x00)<<36 ));
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x10)<<39 )); //Display Clear
	_delay_ms(2);
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x00)<<42 ));	//  increment AC and NO shift
	Local_u64Check |= (((u64)LCD_inenuSendCommand(0x60)<<45 ));


	Global_u8Flag = 1;


	for(Local_u8Iterator = 0; Local_u8Iterator < 48; Local_u8Iterator+=3)
	{
		if(((Local_u64Check >> Local_u8Iterator) & (0b111)) != ES_OK)
		{
			return ES_NOK;
		}
	}

Local_enuErrorState = ES_OK;

#else
#error "Unsupported mode"
#endif


	return Local_enuErrorState;
}


ES_t LCD_enuSendChar(u8 Copy_u8Data)
{
	ES_t Local_enuErrorState = ES_NOK;

	// select instruction register

	if( DIO_enuSetPinVal( RS_PORT, RS_PIN, DIO_u8HIGH ) == ES_OK)
	{
		Local_enuErrorState = LCD_inenuLatch(Copy_u8Data);
	}

	return Local_enuErrorState;
}


ES_t LCD_enuSendCommand(u8 Copy_u8Command)
{
	ES_t Local_enuErrorState = ES_NOK;

	// select instruction register
	if( DIO_enuSetPinVal(RS_PORT, RS_PIN, DIO_u8LOW ) == ES_OK )
	{
		Local_enuErrorState = LCD_inenuLatch(Copy_u8Command);
	}

	return Local_enuErrorState;

}

ES_t LCD_enuClearDisplay(void)
{
	ES_t Local_enuErrorState = ES_NOK;

	Local_enuErrorState = LCD_inenuSendCommand(0x01);

	return Local_enuErrorState;
}

ES_t LCD_enuDisplayString(u8* Copy_pu8String)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8Iterator = 0;
	if( DIO_enuSetPinVal(RS_PORT, RS_PIN, DIO_u8HIGH ) == ES_OK)
	{
		Local_enuErrorState = ES_OK;
		while( Copy_pu8String[Local_u8Iterator] && Local_enuErrorState == ES_OK )
		{
			Local_enuErrorState = LCD_inenuLatch(Copy_pu8String[Local_u8Iterator]);
			Local_u8Iterator++;
		}
	}

	return Local_enuErrorState;
}



ES_t LCD_enuGoto(u8 Copy_u8Line,u8 Copy_u8Block)
{
	ES_t Local_enuErrorState = ES_NOK;

	if (Copy_u8Line == 1)
	{
		Local_enuErrorState = LCD_inenuSendCommand(0x80 + Copy_u8Block);
	}
	else
	{
		Local_enuErrorState = LCD_inenuSendCommand(0xc0 +Copy_u8Block);
	}

	return Local_enuErrorState;
}


ES_t LCD_enuWriteNumber(f32 Copy_u8Num)
{
	ES_t Local_enuErrorState = ES_NOK;
	Local_enuErrorState = DIO_enuSetPinVal(RS_PORT, RS_PIN, DIO_u8HIGH );

	if(Copy_u8Num == 0.0)
	{
		Local_enuErrorState = LCD_inenuLatch('0');
		return Local_enuErrorState;
	}

	static u8 Local_Au8Digits[16] = {'0'};
	s8 Local_u8Iterator = -1;
	s32 Local_s32Number = Copy_u8Num;
	if(Copy_u8Num < 0 )
	{
		LCD_enuSendChar('-');
		Local_s32Number *= -1;
	}

	if((Copy_u8Num < 1.0 && Copy_u8Num > 0.0) || (Copy_u8Num > -1.0 && Copy_u8Num < 0.0)) LCD_enuSendChar('0');

	while (Local_s32Number > 0 && Local_enuErrorState == ES_OK)
	{
		Local_u8Iterator++;
		Local_Au8Digits[Local_u8Iterator] = Local_s32Number % 10;
		Local_s32Number /= 10;
	}

	while( Local_u8Iterator >= 0 && Local_enuErrorState == ES_OK)
	{
		Local_enuErrorState = LCD_inenuLatch( Local_Au8Digits[Local_u8Iterator] + '0');
		Local_u8Iterator--;
	}

	Copy_u8Num = (Copy_u8Num - (s32)Copy_u8Num);
	if(Copy_u8Num != 0.0)
	{
		LCD_inenuLatch('.');
		if(Copy_u8Num < 0) Copy_u8Num *= -1;
		u16 base = 10;
		while (base <= 10000)
		{
			Local_u8Iterator++;
			Local_Au8Digits[3 - Local_u8Iterator] = ((s32)(Copy_u8Num*base))%10;
			base *= 10;
		}

		while( Local_u8Iterator >= 0 && Local_enuErrorState == ES_OK)
		{
			Local_enuErrorState = LCD_inenuLatch( Local_Au8Digits[Local_u8Iterator] + '0');
			Local_u8Iterator--;
		}

	}

	return Local_enuErrorState;
}



static ES_t LCD_inenuLatch( u8 Copy_u8Data )
{
	ES_t Local_enuErrorState = ES_NOK;
	u32 Copy_u32Check = 0;

	// choose write mode
	Copy_u32Check |= ((u32)(DIO_enuSetPinVal(RW_PORT, RW_PIN, DIO_u8LOW )<<0));
	Copy_u32Check |= ((u32)(DIO_enuSetPinVal(EN_PORT, EN_PIN, DIO_u8LOW )<<3));

	// write command

	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB7_PORT, DB7_PIN, (Copy_u8Data>>7)&1 )<<6 ));
	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB6_PORT, DB6_PIN, (Copy_u8Data>>6)&1 )<<9 ));
	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB5_PORT, DB5_PIN, (Copy_u8Data>>5)&1 )<<12 ));
	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB4_PORT, DB4_PIN, (Copy_u8Data>>4)&1 )<<15));

#if LCD_MODE == EIGHT_BIT

	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB3_PORT, DB3_PIN, (Copy_u8Data>>3)&1 )<<18));
	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB2_PORT, DB2_PIN, (Copy_u8Data>>2)&1 )<<21));
	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB1_PORT, DB1_PIN, (Copy_u8Data>>1)&1 )<<24));
	Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB0_PORT, DB0_PIN, (Copy_u8Data>>0)&1 )<<27));

	for(Local_u8Iterator = 0; Local_u8Iterator < 30; Local_u8Iterator+=3)
	{
		if(((Copy_u32Check >> Local_u8Iterator) & (0b111)) != ES_OK)
		{
			return ES_NOK;
		}
	}

	Local_enuErrorState = ES_OK;

#elif LCD_MODE == FOUR_BIT

	if(Global_u8Flag)
	{
		DIO_enuSetPinVal(EN_PORT, EN_PIN, DIO_u8HIGH);
		_delay_ms(1);
		DIO_enuSetPinVal(EN_PORT, EN_PIN, DIO_u8LOW);
		_delay_ms(1);
		Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB7_PORT, DB7_PIN, (Copy_u8Data>>3)&1 )<<6 ));
		Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB6_PORT, DB6_PIN, (Copy_u8Data>>2)&1 )<<9 ));
		Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB5_PORT, DB5_PIN, (Copy_u8Data>>1)&1 )<<12 ));
		Copy_u32Check |= (((u32)DIO_enuSetPinVal(DB4_PORT, DB4_PIN, (Copy_u8Data>>0)&1 )<<15));
	}

	for(Local_u8Iterator = 0; Local_u8Iterator < 18; Local_u8Iterator+=3)
	{
		if(((Copy_u32Check >> Local_u8Iterator) & (0b111)) != ES_OK)
		{
			return ES_NOK;
		}
	}

	Local_enuErrorState = ES_OK;


#else
#error "Unsupported mode"
#endif
	// enable latch

	DIO_enuSetPinVal(EN_PORT, EN_PIN, DIO_u8HIGH);
	_delay_ms(1);
	DIO_enuSetPinVal(EN_PORT, EN_PIN, DIO_u8LOW);
	//_delay_ms(1);

	return Local_enuErrorState;
}


static inline ES_t LCD_inenuSendCommand(u8 Copy_u8Command)
{
	ES_t Local_enuErrorState = ES_NOK;

	// select instruction register
	if(DIO_enuSetPinVal(RS_PORT, RS_PIN, DIO_u8LOW ) == ES_OK)
	{
		Local_enuErrorState = LCD_inenuLatch(Copy_u8Command);
	}

	return Local_enuErrorState;
}


