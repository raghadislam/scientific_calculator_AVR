/*
 * Keypad_prog.c
 *
 *  Created on: Jul 24, 2023
 *      Author: Raghad Islam
 */
#include "../../SERVICE/STDTypes.h"
#include "../../SERVICE/errorState.h"
#include <util/delay.h>

#include "../../MCAL/DIO/DIO_int.h"

#include "Keypad_config.h"


u8 Keypad_u8Buttons[NUM_OF_ROW][NUM_OF_COL] = KPD_BUTTONS ;

ES_t Keypad_enuInit()
{
	{
		ES_t Local_enuErrorState = ES_NOK;

		u32 Local_u32Check = 0;

		/* Set Column Pins as Output */
		Local_u32Check |= (DIO_enuSetPinDir(KPD_COL0_PORT, KPD_COL0_PIN, DIO_u8OUTPUT) << 0);
		Local_u32Check |= (DIO_enuSetPinDir(KPD_COL1_PORT, KPD_COL1_PIN, DIO_u8OUTPUT) << 3);
		Local_u32Check |= (DIO_enuSetPinDir(KPD_COL2_PORT, KPD_COL2_PIN, DIO_u8OUTPUT) << 6);
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_COL3_PORT, KPD_COL3_PIN, DIO_u8OUTPUT) << 9);
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_COL4_PORT, KPD_COL4_PIN, DIO_u8OUTPUT) << 9);
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_COL5_PORT, KPD_COL5_PIN, DIO_u8OUTPUT) << 9);

		DIO_enuSetPinDir(KPD_COL6_PORT, KPD_COL6_PIN, DIO_u8OUTPUT);
		DIO_enuSetPinDir(KPD_COL7_PORT, KPD_COL7_PIN, DIO_u8OUTPUT);
		DIO_enuSetPinDir(KPD_COL8_PORT, KPD_COL8_PIN, DIO_u8OUTPUT);
		DIO_enuSetPinDir(KPD_COL9_PORT, KPD_COL9_PIN, DIO_u8OUTPUT);


		/* Set Column Pins Value as High */
		DIO_enuSetPinVal(KPD_COL0_PORT, KPD_COL0_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL1_PORT, KPD_COL1_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL2_PORT, KPD_COL2_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL3_PORT, KPD_COL3_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL4_PORT, KPD_COL4_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL5_PORT, KPD_COL5_PIN, DIO_u8HIGH);


		DIO_enuSetPinVal(KPD_COL6_PORT, KPD_COL6_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL7_PORT, KPD_COL7_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL8_PORT, KPD_COL8_PIN, DIO_u8HIGH);
		DIO_enuSetPinVal(KPD_COL9_PORT, KPD_COL9_PIN, DIO_u8HIGH);



		/* Set Row Pins as Input */
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_ROW0_PORT, KPD_ROW0_PIN, DIO_u8INPUT) << 12);
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_ROW1_PORT, KPD_ROW1_PIN, DIO_u8INPUT) << 15);
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_ROW2_PORT, KPD_ROW2_PIN, DIO_u8INPUT) << 18);
		Local_u32Check |= ((u32)DIO_enuSetPinDir(KPD_ROW3_PORT, KPD_ROW3_PIN, DIO_u8INPUT) << 21);

		/* Set Row Pins Value as Pulled_up */
		DIO_enuSetPinVal(KPD_ROW0_PORT, KPD_ROW0_PIN, DIO_u8PULL_UP);
		DIO_enuSetPinVal(KPD_ROW1_PORT, KPD_ROW1_PIN, DIO_u8PULL_UP);
		DIO_enuSetPinVal(KPD_ROW2_PORT, KPD_ROW2_PIN, DIO_u8PULL_UP);
		DIO_enuSetPinVal(KPD_ROW3_PORT, KPD_ROW3_PIN, DIO_u8PULL_UP);

		u8 Local_u8Iterator;
		for(Local_u8Iterator = 0; Local_u8Iterator <= 21; Local_u8Iterator += 3)
		{
			if(( (Local_u32Check >> Local_u8Iterator) & (0b111) ) != ES_OK)
			{
				return ES_NOK;
			}

		}

		return ES_OK;
	}
}


ES_t Keypad_enuGetPressedKey(u8* Copy_pu8Key)
{
	ES_t Local_enuErrorState = ES_NOK;

	u8 Local_u8Row, Local_u8col, Local_u8pinval;

	*Copy_pu8Key =  DEFAULT_KEY;

	u8 Local_Au8ColPort[NUM_OF_COL] = KPD_COL_PORTS;
	u8 Local_Au8ColPins[NUM_OF_COL] = KPD_COL_PINS;

	u8 Local_Au8ROWPort[NUM_OF_ROW] = KPD_ROW_PORTS;
	u8 Local_Au8ROWPins[NUM_OF_ROW] = KPD_ROW_PINS;



	for(Local_u8col = 0; Local_u8col < NUM_OF_COL; ++Local_u8col)
	{

		Local_enuErrorState = DIO_enuSetPinVal(Local_Au8ColPort[Local_u8col], Local_Au8ColPins[Local_u8col], DIO_u8LOW );
		if(Local_enuErrorState != ES_OK) return Local_enuErrorState;

		for(Local_u8Row = 0; Local_u8Row < NUM_OF_ROW; ++Local_u8Row)
		{
			Local_enuErrorState = DIO_enuGetPinVal(Local_Au8ROWPort[Local_u8Row], Local_Au8ROWPins[Local_u8Row] ,&Local_u8pinval);
			if(Local_enuErrorState != ES_OK) return Local_enuErrorState;

			if(!Local_u8pinval)
			{
				if(!Local_u8pinval)
				{

					*Copy_pu8Key = Keypad_u8Buttons[Local_u8Row][Local_u8col];

					while(!Local_u8pinval)
					{
						Local_enuErrorState = DIO_enuGetPinVal(Local_Au8ROWPort[Local_u8Row], Local_Au8ROWPins[Local_u8Row] ,&Local_u8pinval);
					}

				}
			}

			if(*Copy_pu8Key != DEFAULT_KEY)  return Local_enuErrorState;
		}

		Local_enuErrorState = DIO_enuSetPinVal(Local_Au8ColPort[Local_u8col], Local_Au8ColPins[Local_u8col], DIO_u8HIGH );
		if(Local_enuErrorState != ES_OK) return Local_enuErrorState;

	}


	return Local_enuErrorState;
}
