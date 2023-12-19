/*
 * main.c
 *
 *      Created on: Jul 25, 2023
 *      Author: Raghad islam
 */


#include <util/delay.h>


#include "../ATmega32_CTOS/SERVICE/STDTypes.h"
#include "../ATmega32_CTOS/SERVICE/errorState.h"
#include "../ATmega32_CTOS/SERVICE/BIT_MATH.h"
#include "../ATmega32_CTOS/SERVICE/MATH.h"

#include "../ATmega32_CTOS/HAL/keypad/Keypad_int.h"
#include "../ATmega32_CTOS/HAL/LCD/LCD_int.h"

enum {
	negative,
	first,
	error,
	last_is_num,
	first_is_op,
	Sqrt,
	Sin,
	Cos,
	Tan,
	dot,
	Exp,
	last_is_tri,
	numRep,
	must_clear
};


enum {
	Sinh,
	Cosh,
	Tanh,
	Asin,
	Acos,
	Atan,
	HEX,
	OCT,
	DEC,
	BIN,
	NUMerror,
	bracets,
	pow,
	Abs
};

// initialize global variables

f32 Global_Af32Operands[8] = {0};
u8  Global_u8Operations[8] = {0};

u8 Global_u8It1 = 0, Global_u8It2 = 0,Global_u8It3, Global_u8It4, Global_u8It5;
s8  Global_s8Iter;

u16 Global_u16Num =-1;

u8 Global_u8Arr[17]    = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
u8 Global_u8SecArr[17] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};


f32 Global_f32Num = 0;
u8  Global_u8PressedKey = NOT_PRESSED;
u8  Global_u8Cnt = 1;
u16 Global_u16FLAGS = 2; // 0b00000010
u16 Global_u16HYP = 0;
f32 Global_f32Ans = 0;
u8  Global_u8ShiftCnt = 0;
f32 Global_f32mode = 1; // radian or degree
u8  Global_u8Shift=0;

void ClearGlobalVar(void)
{
	Global_f32Num = 0;
	Global_u8Cnt=1;
	Global_u8It1 =0;
	Global_u8It2 =0;
	Global_u8ShiftCnt = 0;
	Global_u16FLAGS = 2;
	Global_u16HYP = 0;
	Global_u8Shift=0;
}


int main()
{
	// initialize LCD and keypad
	LCD_enuInit();
	Keypad_enuInit();


	// start program
	while(1)
	{
		Global_u8PressedKey = NOT_PRESSED;

		while(Global_u8PressedKey == NOT_PRESSED)
		{
			Keypad_enuGetPressedKey(&Global_u8PressedKey);
			if(Get_bit(Global_u16FLAGS,must_clear) && Global_u8PressedKey != 'c' && Global_u8PressedKey != 'q' && Global_u8PressedKey != 'x' )
			{
				Global_u8PressedKey = NOT_PRESSED;
			}
		}
		Clr_bit(Global_u16FLAGS,must_clear);

		/* check number of characters for each button to know when to shift the lcd screen */

		if(Global_u8PressedKey == '!') LCD_enuDisplayString("sqrt("), Global_u8Shift+=5;
		else if (Global_u8PressedKey == 's') LCD_enuDisplayString("sin("), Global_u8Shift+=4;
		else if (Global_u8PressedKey == 'C') LCD_enuDisplayString("cos("), Global_u8Shift+=4;
		else if (Global_u8PressedKey == 't') LCD_enuDisplayString("tan("), Global_u8Shift+=4;
		else if (Global_u8PressedKey == '^') LCD_enuDisplayString("(e^"), Global_u8Shift+=3;

		else if (Global_u8PressedKey == 'y') LCD_enuDisplayString("asin("), Global_u8Shift+=5;
		else if (Global_u8PressedKey == '[') LCD_enuDisplayString("acos("), Global_u8Shift+=5;
		else if (Global_u8PressedKey == 'v') LCD_enuDisplayString("atan("), Global_u8Shift+=5;

		else if (Global_u8PressedKey == 'w') LCD_enuDisplayString("sinh("), Global_u8Shift+=5;
		else if (Global_u8PressedKey == 'u') LCD_enuDisplayString("cosh("), Global_u8Shift+=5;
		else if (Global_u8PressedKey == 'p') LCD_enuDisplayString("tanh("), Global_u8Shift+=5;
		else if((Global_u8PressedKey >= '0' && Global_u8PressedKey<='9') || Global_u8PressedKey == '.')
			LCD_enuSendChar(Global_u8PressedKey), Global_u8Shift++;

		if((Global_u8PressedKey >= '0' && Global_u8PressedKey<='9') || Global_u8PressedKey == '.' || Global_u8PressedKey == 'e' || Global_u8PressedKey == '&' )
		{
			if(Global_u8Shift>16)
			{
				for(Global_u8It3=0;Global_u8It3<(Global_u8Shift-16); ++Global_u8It3)
				{
					LCD_enuSendCommand(0x18);
					Global_u8ShiftCnt++;
				}
				Global_u8Shift=16;
			}
			// take the numbers ( operands, angels, powers.....)

			if(Global_u8PressedKey == '.')
			{
				Set_bit(Global_u16FLAGS,dot);
				continue;
			}
			Set_bit(Global_u16FLAGS,last_is_num) ;
			Clr_bit(Global_u16FLAGS,first);
			Clr_bit(Global_u16FLAGS,last_is_tri);

			if(Global_u8PressedKey == 'e')
			{
				Global_f32Num = M_PI;
				Global_u8Shift+=2;
				LCD_enuDisplayString("PI");
				continue;
			}
			else if (Global_u8PressedKey == '&'){
				Global_f32Num = Global_f32Ans;
				LCD_enuDisplayString("Ans");
				Global_u8Shift+=3;
				continue;
			}
			if(Get_bit(Global_u16FLAGS,negative))
			{
				if( Get_bit(Global_u16FLAGS,dot) )
				{
					Global_f32Num = Global_f32Num - ((f32)(Global_u8PressedKey-'0')  / (Global_u8Cnt*10.0));
					Global_u8Cnt*=10;

					if(Global_f32Num > 0) Global_f32Num*=-1;
				}
				else
				{
					Global_f32Num = Global_f32Num*10 - (Global_u8PressedKey-'0');

					if(Global_f32Num > 0) Global_f32Num*=-1;
				}

			}
			else
			{
				if( Get_bit(Global_u16FLAGS,dot) )
				{
					Global_f32Num = Global_f32Num + ((f32)(Global_u8PressedKey-'0')  / (Global_u8Cnt*10.0));
					Global_u8Cnt*=10;
				}
				else
				{
					Global_f32Num = Global_f32Num*10 + (Global_u8PressedKey-'0');
				}
			}

		}
		else if(Global_u8PressedKey == '=') /* calculating the result */
		{

			if( Get_bit(Global_u16FLAGS,last_is_num) && !Get_bit(Global_u16FLAGS,first_is_op) && !Get_bit(Global_u16FLAGS,error) )
			{

				if(Get_bit(Global_u16FLAGS,Sin))
				{
					Global_Af32Operands[Global_u8It1] = _sin(Global_f32Num*(M_PI/180.0)*(1/Global_f32mode));
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16FLAGS,Cos ))
				{
					Global_Af32Operands[Global_u8It1] = _cos(Global_f32Num*(M_PI/180.0)*(1/Global_f32mode));
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16FLAGS,Sqrt))
				{
					if(Global_f32Num < 0)
					{
						Set_bit(Global_u16FLAGS, error);
					}
					Global_Af32Operands[Global_u8It1] = _sqrt(Global_f32Num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16FLAGS,Tan))
				{
					if((Global_f32Num == 90 || Global_f32Num == -90 || Global_f32Num == 270 || Global_f32Num == -270 ) && Global_f32mode == 1)
					{
						Set_bit(Global_u16FLAGS, error);
						LCD_enuSendChar(')');
					}

					if((Global_f32Num == M_PI/2 || Global_f32Num == -M_PI/2 || Global_f32Num == 3*M_PI/2 || Global_f32Num == -3*M_PI/2 ) && Global_f32mode == M_PI/180.0)
					{
						Set_bit(Global_u16FLAGS, error);
						LCD_enuSendChar(')');
					}

					Global_Af32Operands[Global_u8It1] = _tan(Global_f32Num*(M_PI/180.0)*(1/Global_f32mode));
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16FLAGS,Exp ))
				{
					Global_Af32Operands[Global_u8It1] = expo(Global_f32Num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16HYP,Atan))
				{
					Global_Af32Operands[Global_u8It1] = my_atan(Global_f32Num)*(180.0/M_PI)*Global_f32mode;
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16HYP,Asin))
				{
					if(Global_f32Num > 1 || Global_f32Num < -1)
					{
						Set_bit(Global_u16FLAGS, error);
						LCD_enuSendChar(')');
					}
					Global_Af32Operands[Global_u8It1] = my_asin(Global_f32Num)*(180.0/M_PI)*Global_f32mode;
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16HYP,Acos))
				{
					if(Global_f32Num > 1 || Global_f32Num < -1)
					{
						Set_bit(Global_u16FLAGS, error);
						LCD_enuSendChar(')');

					}
					Global_Af32Operands[Global_u8It1] = my_acos(Global_f32Num)*(180.0/M_PI)*Global_f32mode;
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16HYP,Sinh))
				{
					Global_Af32Operands[Global_u8It1] = _sinh(Global_f32Num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16HYP,Cosh))
				{
					Global_Af32Operands[Global_u8It1] = _cosh(Global_f32Num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Global_u16HYP,Tanh))
				{
					Global_Af32Operands[Global_u8It1] = _tanh(Global_f32Num);
					LCD_enuSendChar(')');
				}
				else
				{
					Global_Af32Operands[Global_u8It1] = Global_f32Num;
				}

				Global_u16FLAGS &= 0xfa1f;
				Global_u16HYP =0;
				Clr_bit(Global_u16FLAGS,last_is_tri);


				if(Get_bit(Global_u16FLAGS,error))
				{
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Math error!!..");

					ClearGlobalVar();

					Set_bit(Global_u16FLAGS,must_clear);
					continue;
				}

				LCD_enuSendChar(Global_u8PressedKey);
				LCD_enuGoto(2,0);


				for(Global_u8It4 = 0; Global_u8It4 < 8; ++Global_u8It4)
				{
					if(Global_u8Operations[Global_u8It4] == ';')
					{

						Global_Af32Operands[Global_u8It4] = _pow(Global_Af32Operands[Global_u8It4], Global_Af32Operands[Global_u8It4 + 1]);
						Global_Af32Operands[Global_u8It4 + 1] = 0;
						Global_u8Operations[Global_u8It4] = 0;
						for(Global_u8It5 = Global_u8It4 + 1; Global_u8It5 < 7; ++Global_u8It5)
						{
							Global_Af32Operands[Global_u8It5] = Global_Af32Operands[Global_u8It5 + 1];
						}
						for(Global_u8It5 = Global_u8It4 ; Global_u8It5 < 7; ++Global_u8It5)
						{
							Global_u8Operations[Global_u8It5] = Global_u8Operations[Global_u8It5 + 1];
						}
						Global_u8It4--;
					}
				}

				for(Global_u8It4 = 0; Global_u8It4 < 8; ++Global_u8It4)
				{
					if(Global_u8Operations[Global_u8It4] == '*' || Global_u8Operations[Global_u8It4] == '/')
					{
						if(Global_u8Operations[Global_u8It4] == '*') Global_Af32Operands[Global_u8It4] *= Global_Af32Operands[Global_u8It4 + 1];
						else Global_Af32Operands[Global_u8It4] /= Global_Af32Operands[Global_u8It4 + 1];

						Global_Af32Operands[Global_u8It4 + 1] = 0;
						Global_u8Operations[Global_u8It4] = 0;
						for(Global_u8It5 = Global_u8It4 + 1; Global_u8It5 < 7; ++Global_u8It5)
						{
							Global_Af32Operands[Global_u8It5] = Global_Af32Operands[Global_u8It5 + 1];
						}
						for(Global_u8It5 = Global_u8It4 ; Global_u8It5 < 7; ++Global_u8It5)
						{
							Global_u8Operations[Global_u8It5] = Global_u8Operations[Global_u8It5 + 1];
						}
						Global_u8It4--;
					}
				}

				while(Global_u8Operations[0])
				{
					if(Global_u8Operations[0] == '+') Global_Af32Operands[0] += Global_Af32Operands[1]; // 5+9  14
					else Global_Af32Operands[0] -= Global_Af32Operands[1];

					Global_Af32Operands[1] = 0;
					Global_u8Operations[0]=0;

					for(Global_u8It5 = 1; Global_u8It5 < 6; ++Global_u8It5)
					{
						Global_Af32Operands[Global_u8It5] = Global_Af32Operands[Global_u8It5 + 1];
					}
					for(Global_u8It5 = 0 ; Global_u8It5 < 5; ++Global_u8It5)
					{
						Global_u8Operations[Global_u8It5] = Global_u8Operations[Global_u8It5 + 1];
					}
				}
				LCD_enuWriteNumber(Global_Af32Operands[0]);
				Global_f32Ans = Global_Af32Operands[0];
				Global_Af32Operands[0] = 0;
				Global_u8It2 = 0, Global_u8It1 = 0;

				if(Global_u8ShiftCnt > 0)
				{
					for(Global_u8It3=0; Global_u8It3<Global_u8ShiftCnt ; Global_u8It3++)
					{
						LCD_enuSendCommand(0x1c);
					}
				}

				else if(Global_u8ShiftCnt < 0)
				{
					Global_u8ShiftCnt *= -1;
					for(Global_u8It3=0; Global_u8It3<Global_u8ShiftCnt ; Global_u8It3++)
					{
						LCD_enuSendCommand(0x18);
					}
				}

				for(Global_u8It3 =0; Global_u8It3<8; ++Global_u8It3)
				{
					Global_u8Operations[Global_u8It3]= 0;
					Global_Af32Operands[Global_u8It3]=0;
				}

				ClearGlobalVar();

				Set_bit(Global_u16FLAGS,must_clear);

			}
			else
			{
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Math error!!.."); // something went wrong
				ClearGlobalVar();
				continue;
			}

		}
		else if (Global_u8PressedKey == 'c') // clear the display
		{
			LCD_enuClearDisplay();
			ClearGlobalVar();

		}
	        	/********************************** Number representation **********************************/

		else if (Global_u8PressedKey == 'i' || Global_u8PressedKey == 'z' || Global_u8PressedKey == 'a' || Global_u8PressedKey =='o')
		{
			Clr_bit(Global_u16HYP,NUMerror);
			Global_f32Num = 0;

			LCD_enuGoto(1,12);

			if(Global_u8PressedKey == 'i')
			{
				Set_bit(Global_u16HYP, DEC);
				for(Global_s8Iter =0; Global_s8Iter<16; ++Global_s8Iter) Global_u8Arr[Global_s8Iter]=' ';
				LCD_enuDisplayString("DEC");
			}
			else if(Global_u8PressedKey == 'z')
			{
				Set_bit(Global_u16HYP, BIN);
				LCD_enuDisplayString("BIN");
			}
			else if(Global_u8PressedKey == 'a')
			{
				Set_bit(Global_u16HYP, OCT);
				LCD_enuDisplayString("OCT");
			}
			else if(Global_u8PressedKey == 'o')
			{
				Set_bit(Global_u16HYP, HEX);
				LCD_enuDisplayString("HEX");
			}

			LCD_enuGoto(2,0);

			while (Global_u8PressedKey != '=')
			{
				Global_u8PressedKey = NOT_PRESSED;

				while(Global_u8PressedKey == NOT_PRESSED)
				{
					Keypad_enuGetPressedKey(&Global_u8PressedKey);
				}

				if((Global_u8PressedKey >= '0' && Global_u8PressedKey<='9') || Global_u8PressedKey == '.' )
				{
					LCD_enuSendChar(Global_u8PressedKey);

					if(Get_bit(Global_u16HYP,NUMerror)) continue;
					else if(Global_u8PressedKey == '.')
					{
						Set_bit(Global_u16HYP,NUMerror);
						continue;
					}
					else if(Get_bit(Global_u16HYP, OCT) && Global_u8PressedKey >'7' && Global_u8PressedKey <='9' )
					{
						Set_bit(Global_u16HYP,NUMerror);
						continue;
					}
					else if (Get_bit(Global_u16HYP, BIN) && Global_u8PressedKey != '0' && Global_u8PressedKey != '1')
					{
						Set_bit(Global_u16HYP,NUMerror);
						continue;
					}
					else{
						Global_f32Num = Global_f32Num*10 + (Global_u8PressedKey-'0');
						for(Global_s8Iter = 15 ; Global_s8Iter > 0; --Global_s8Iter)
						{
							Global_u8Arr[Global_s8Iter] = Global_u8Arr[Global_s8Iter-1];
						}
						Global_u8Arr[0] = Global_u8PressedKey;
					}
				}
				else if(Global_u8PressedKey == 'x' || Global_u8PressedKey == 'q' ||Global_u8PressedKey == 'i' ||Global_u8PressedKey == 'o' ||Global_u8PressedKey == 'z' ||Global_u8PressedKey == 'a')
				{

					if(Global_u8PressedKey == 'x')LCD_enuSendChar('A');
					else if(Global_u8PressedKey == 'x')LCD_enuSendChar('A');
					else if(Global_u8PressedKey == 'q')LCD_enuSendChar('B');
					else if(Global_u8PressedKey == 'i')LCD_enuSendChar('C');
					else if(Global_u8PressedKey == 'o')LCD_enuSendChar('D');
					else if(Global_u8PressedKey == 'z')LCD_enuSendChar('E');
					else if(Global_u8PressedKey == 'a')LCD_enuSendChar('F');

					if(!Get_bit(Global_u16HYP, HEX)) Set_bit(Global_u16HYP,NUMerror);
					else
					{
						for(Global_s8Iter = 15 ; Global_s8Iter > 0; --Global_s8Iter)
						{
							Global_u8Arr[Global_s8Iter] = Global_u8Arr[Global_s8Iter-1];
						}

						if(Global_u8PressedKey =='x') Global_u8Arr[0] = 'A';
						if(Global_u8PressedKey =='q') Global_u8Arr[0] = 'B';
						if(Global_u8PressedKey =='i') Global_u8Arr[0] = 'C';
						if(Global_u8PressedKey =='o') Global_u8Arr[0] = 'D';
						if(Global_u8PressedKey =='z') Global_u8Arr[0] = 'E';
						if(Global_u8PressedKey =='a') Global_u8Arr[0] = 'F';
					}

				}
				else if(Global_u8PressedKey != '=' && Global_u8PressedKey != 'c' )
				{
					Set_bit(Global_u16HYP,NUMerror);

					if(Global_u8PressedKey != '+' && Global_u8PressedKey != '-' && Global_u8PressedKey != '*' && Global_u8PressedKey != '/')
					{
						Set_bit(Global_u16HYP,bracets);
					}
					if(Global_u8PressedKey == '!') LCD_enuDisplayString("sqrt(");
					else if (Global_u8PressedKey == 's')LCD_enuDisplayString("sin(");
					else if (Global_u8PressedKey == 'C')LCD_enuDisplayString("cos(");
					else if (Global_u8PressedKey == 't')LCD_enuDisplayString("tan(");
					else if (Global_u8PressedKey == '^')LCD_enuDisplayString("(e^");

					else if (Global_u8PressedKey == 'y')LCD_enuDisplayString("asin(");
					else if (Global_u8PressedKey == '[')LCD_enuDisplayString("acos(");
					else if (Global_u8PressedKey == 'v')LCD_enuDisplayString("atan(");

					else if (Global_u8PressedKey == 'w')LCD_enuDisplayString("sinh(");
					else if (Global_u8PressedKey == 'u')LCD_enuDisplayString("cosh(");
					else if (Global_u8PressedKey == 'p')LCD_enuDisplayString("tanh(");
					else{
						if(Get_bit(Global_u16HYP,bracets)) LCD_enuSendChar(')');
						LCD_enuSendChar(Global_u8PressedKey);
						Clr_bit(Global_u16HYP,bracets);
					}
				}
				else if(Global_u8PressedKey == '=')
				{
					if(Get_bit(Global_u16HYP,bracets)) LCD_enuSendChar(')');
					break;
				}
				else if(Global_u8PressedKey == 'c')
				{
					LCD_enuClearDisplay();
					ClearGlobalVar();
					LCD_enuSendCommand(0x06);
					Clr_bit(Global_u16HYP,NUMerror);
					break;
				}
			}

			if(Get_bit(Global_u16HYP,NUMerror) && Global_u8PressedKey!='c' )
			{
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Math error!!..");
				ClearGlobalVar();
				continue;
			}
			else if (Global_u8PressedKey!='c')
			{
				LCD_enuGoto(2,15);
				LCD_enuSendCommand(0x04);
				LCD_enuDisplayString(Global_u8Arr);
				while(1)
				{

					Global_u8PressedKey = NOT_PRESSED;

					while(Global_u8PressedKey != 'i' && Global_u8PressedKey != 'z' && Global_u8PressedKey != 'a' && Global_u8PressedKey != 'o' &&  Global_u8PressedKey != 'c' )
					{
						Keypad_enuGetPressedKey(&Global_u8PressedKey);
					}
					if(Global_u8PressedKey == 'c')
					{
						LCD_enuClearDisplay();
						Global_f32Num =0;
						Global_u16FLAGS = 2;
						for(Global_s8Iter=0 ;Global_s8Iter<16;++Global_s8Iter)Global_u8Arr[Global_s8Iter]='0';
						for(Global_s8Iter=0 ;Global_s8Iter<16;++Global_s8Iter)Global_u8SecArr[Global_s8Iter]='0';
						Global_u16HYP = 0;
						Global_u8It2 =0;
						Global_u8It1 =0;
						Global_u8Cnt=1;
						break;
					}
					else if(Get_bit(Global_u16HYP, DEC))
					{
						for(Global_s8Iter=0 ;Global_s8Iter<16;++Global_s8Iter)Global_u8Arr[Global_s8Iter]='0';

						if(Global_u8PressedKey == 'i')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("CED");
							Global_u8It3=0;
							Global_u16Num = Global_f32Num;
							do {
								Global_u16Num /= 10;
								++Global_u8It3;
							} while (Global_u16Num != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(Global_s8Iter=0;  Global_s8Iter < 15-Global_u8It3+1 ; ++Global_s8Iter) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-Global_u8It3+1);
							LCD_enuWriteNumber(Global_f32Num);
							LCD_enuSendCommand(0x04);
						}
						else if(Global_u8PressedKey == 'o')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("XEH");
							Dec2Hex(Global_f32Num,Global_u8Arr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8Arr[Global_s8Iter]);
						}
						else if(Global_u8PressedKey == 'z')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("NIB");
							Dec2Bin(Global_f32Num,Global_u8Arr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8Arr[Global_s8Iter]);
						}
						else if(Global_u8PressedKey == 'a')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("TCO");
							Dec2Oct(Global_f32Num,Global_u8Arr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8Arr[Global_s8Iter]);
						}
					}
					else if(Get_bit(Global_u16HYP, OCT))
					{
						for(Global_s8Iter=0 ;Global_s8Iter<16;++Global_s8Iter)Global_u8SecArr[Global_s8Iter]  ='0';
						if(Global_u8PressedKey == 'i')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("CED");
							Global_u8It3=0;
							Global_u16Num = Oct2Dec(Global_u8Arr);
							do {
								Global_u16Num /= 10;
								++Global_u8It3;
							} while (Global_u16Num != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(Global_s8Iter=0;  Global_s8Iter < 15-Global_u8It3+1 ; ++Global_s8Iter) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-Global_u8It3+1);
							LCD_enuWriteNumber(Oct2Dec(Global_u8Arr));
							LCD_enuSendCommand(0x04);
						}
						else if(Global_u8PressedKey == 'o')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("XEH");
							Global_u16Num = Oct2Dec(Global_u8Arr);
							Dec2Hex(Global_u16Num,Global_u8SecArr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8SecArr[Global_s8Iter]);
						}
						else if(Global_u8PressedKey == 'z')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("NIB");
							Global_u16Num = Oct2Dec(Global_u8Arr);
							Dec2Bin(Global_u16Num,Global_u8SecArr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8SecArr[Global_s8Iter]);
						}
						else if(Global_u8PressedKey == 'a')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("TCO");
							LCD_enuGoto(2,15);
							LCD_enuSendCommand(0x04);
							LCD_enuDisplayString(Global_u8Arr);
						}
					}
					else if(Get_bit(Global_u16HYP, BIN))
					{
						for(Global_s8Iter=0 ;Global_s8Iter<16;++Global_s8Iter)Global_u8SecArr[Global_s8Iter]  ='0';
						if(Global_u8PressedKey == 'i')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("CED");
							Global_u8It3=0;
							Global_u16Num = Bin2Dec(Global_u8Arr);
							do {
								Global_u16Num /= 10;
								++Global_u8It3;
							} while (Global_u16Num != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(Global_s8Iter=0;  Global_s8Iter < 15-Global_u8It3+1 ; ++Global_s8Iter) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-Global_u8It3+1);
							LCD_enuWriteNumber(Bin2Dec(Global_u8Arr));
							LCD_enuSendCommand(0x04);
						}
						else if(Global_u8PressedKey == 'o')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("XEH");
							Global_u16Num = Bin2Dec(Global_u8Arr);
							Dec2Hex(Global_u16Num,Global_u8SecArr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8SecArr[Global_s8Iter]);
						}
						else if(Global_u8PressedKey == 'z')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("NIB");
							LCD_enuGoto(2,15);
							LCD_enuSendCommand(0x04);
							LCD_enuDisplayString(Global_u8Arr);
						}
						else if(Global_u8PressedKey == 'a')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("TCO");
							Global_u16Num = Bin2Dec(Global_u8Arr);
							Dec2Oct(Global_u16Num,Global_u8SecArr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8SecArr[Global_s8Iter]);
						}
					}
					else if(Get_bit(Global_u16HYP, HEX))
					{
						for(Global_s8Iter=0 ;Global_s8Iter<16;++Global_s8Iter)Global_u8SecArr[Global_s8Iter]  ='0';
						if(Global_u8PressedKey == 'i')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("CED");
							Global_u8It3=0;
							Global_u16Num = Hex2Dec(Global_u8Arr);
							do {
								Global_u16Num /= 10;
								++Global_u8It3;
							} while (Global_u16Num != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(Global_s8Iter=0;  Global_s8Iter < 15-Global_u8It3+1 ; ++Global_s8Iter) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-Global_u8It3+1);
							LCD_enuWriteNumber(Hex2Dec(Global_u8Arr));
							LCD_enuSendCommand(0x04);
						}
						else if(Global_u8PressedKey == 'o')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("XEH");
							LCD_enuGoto(2,15);
							LCD_enuSendCommand(0x04);
							LCD_enuDisplayString(Global_u8Arr);
						}
						else if(Global_u8PressedKey == 'z')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("NIB");
							Global_u16Num = Hex2Dec(Global_u8Arr);
							Dec2Bin(Global_u16Num,Global_u8SecArr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8SecArr[Global_s8Iter]);
						}
						else if(Global_u8PressedKey == 'a')
						{
							LCD_enuGoto(1,14);
							LCD_enuDisplayString("TCO");
							Global_u16Num = Hex2Dec(Global_u8Arr);
							Dec2Oct(Global_u16Num,Global_u8SecArr);
							LCD_enuGoto(2,15);
							for(Global_s8Iter=15; Global_s8Iter>=0; --Global_s8Iter) LCD_enuSendChar(Global_u8SecArr[Global_s8Iter]);
						}

					}

				}

			}
		}
		else if(Global_u8PressedKey == 'q') // shift right
		{
			LCD_enuSendCommand(0x18);
			Global_u8ShiftCnt++;
		}
		else if(Global_u8PressedKey == 'x') // shift left
		{
			LCD_enuSendCommand(0x1c);
			Global_u8ShiftCnt--;
		}
		else if(Global_u8PressedKey == 'b') // Redian mode
		{
			Global_f32mode=(M_PI)/(180.0);
		}
		else if(Global_u8PressedKey == 'r') // Degree mode
		{
			Global_f32mode=1;
		}
		else							   // Handling all operations
		{

			if(Global_u8Shift>16)
			{
				for(Global_u8It3=0;Global_u8It3<(Global_u8Shift-16); ++Global_u8It3)
				{
					LCD_enuSendCommand(0x18);
					Global_u8ShiftCnt++;
				}
				Global_u8Shift=16;
			}

			Global_u8Shift++;
			Clr_bit(Global_u16FLAGS,dot);
			Clr_bit(Global_u16FLAGS,negative);
			Global_u8Cnt=1;

			if(     Global_u8PressedKey == 't' || Global_u8PressedKey == 'C' || Global_u8PressedKey == 's' ||
					Global_u8PressedKey == '!' || Global_u8PressedKey == '^' || Global_u8PressedKey == 'v' ||
					Global_u8PressedKey == 'y' || Global_u8PressedKey == '[' || Global_u8PressedKey == 'w' ||
					Global_u8PressedKey == 'u' || Global_u8PressedKey == 'p')
			{
				switch (Global_u8PressedKey)
				{
				case 't':	Set_bit(Global_u16FLAGS, Tan);
				break;
				case 'C':	Set_bit(Global_u16FLAGS, Cos);
				break;
				case 's':	Set_bit(Global_u16FLAGS, Sin);
				break;
				case '!':	Set_bit(Global_u16FLAGS, Sqrt);
				break;
				case '^':	Set_bit(Global_u16FLAGS, Exp);
				break;
				case 'v':   Set_bit(Global_u16HYP, Atan);
				break;
				case 'y':   Set_bit(Global_u16HYP, Asin);
				break;
				case '[':   Set_bit(Global_u16HYP, Acos);
				break;
				case 'w':   Set_bit(Global_u16HYP, Sinh);
				break;
				case 'u':   Set_bit(Global_u16HYP, Cosh);
				break;
				case 'p':   Set_bit(Global_u16HYP, Tanh);
				break;
				}
				Set_bit(Global_u16FLAGS,last_is_tri);
				continue;
			}

			if(Get_bit(Global_u16FLAGS,first) && Global_u8PressedKey != '+' && Global_u8PressedKey != '-' )
			{
				if(Global_u8PressedKey != ';')
				LCD_enuSendChar(Global_u8PressedKey);
				Set_bit(Global_u16FLAGS,first_is_op);
			}
			Clr_bit(Global_u16FLAGS,first);

			if(Get_bit(Global_u16FLAGS,last_is_tri) && (Global_u8PressedKey == '*' || Global_u8PressedKey == '/'|| Global_u8PressedKey == ';') )
			{
				Set_bit(Global_u16FLAGS,error);
			}

			if(!Get_bit(Global_u16FLAGS,last_is_num))
			{

				if(Global_u8PressedKey == '-'){
					Set_bit(Global_u16FLAGS, negative);
					LCD_enuSendChar('-');
					continue;
				}
				else if(Global_u8PressedKey == '*' || Global_u8PressedKey == '/')
				{
					Set_bit(Global_u16FLAGS, error);
					continue;
				}
				else if(Global_u8PressedKey == '+')
				{
					LCD_enuSendChar('+');
					continue;
				}
			}

			if(Get_bit(Global_u16FLAGS,Sin))
			{
				Global_Af32Operands[Global_u8It1] = _sin(Global_f32Num*(M_PI/180.0)*(1/Global_f32mode));
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16FLAGS,Cos ))
			{
				Global_Af32Operands[Global_u8It1] = _cos(Global_f32Num*(M_PI/180.0)*(1/Global_f32mode));
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16FLAGS,Sqrt))
			{
				if(Global_f32Num < 0)
				{
					Set_bit(Global_u16FLAGS, error);
					LCD_enuSendChar(')');

				}
				Global_Af32Operands[Global_u8It1] = _sqrt(Global_f32Num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16FLAGS,Tan))
			{
				if(Global_f32Num == 90 || Global_f32Num == -90 )
				{
					Set_bit(Global_u16FLAGS, error);
				}
				Global_Af32Operands[Global_u8It1] = _tan(Global_f32Num*(M_PI/180.0)*(1/Global_f32mode));
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16FLAGS,Exp ))
			{
				Global_Af32Operands[Global_u8It1] = expo(Global_f32Num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16HYP,Atan ))
			{
				Global_Af32Operands[Global_u8It1] = my_atan(Global_f32Num)*(180.0/M_PI)*Global_f32mode;
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16HYP,Asin ))
			{
				if(Global_f32Num > 1 || (Global_f32Num < -1))
				{
					Set_bit(Global_u16FLAGS, error);
				}
				Global_Af32Operands[Global_u8It1] = my_asin(Global_f32Num)*(180.0/M_PI)*Global_f32mode;
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16HYP,Acos ))
			{
				if(Global_f32Num > 1 || (Global_f32Num < -1 ))
				{
					Set_bit(Global_u16FLAGS, error);
				}
				Global_Af32Operands[Global_u8It1] = my_acos(Global_f32Num)*(180.0/M_PI)*Global_f32mode;
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16HYP,Sinh ))
			{
				Global_Af32Operands[Global_u8It1] = _sinh(Global_f32Num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16HYP,Cosh ))
			{
				Global_Af32Operands[Global_u8It1] = _cosh(Global_f32Num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Global_u16HYP,Tanh ))
			{
				Global_Af32Operands[Global_u8It1] = _tanh(Global_f32Num);
				LCD_enuSendChar(')');
			}
			else{ Global_Af32Operands[Global_u8It1] = Global_f32Num;
			}
			Global_u8It1++;
			if(Get_bit(Global_u16FLAGS, last_is_tri)) Global_u8Shift++;

			Clr_bit(Global_u16FLAGS,Sin);
			Clr_bit(Global_u16FLAGS,Cos );
			Clr_bit(Global_u16FLAGS,Sqrt);
			Clr_bit(Global_u16FLAGS,Tan);
			Clr_bit(Global_u16FLAGS,Exp);
			Clr_bit(Global_u16HYP,Atan);
			Clr_bit(Global_u16HYP,Acos);
			Clr_bit(Global_u16HYP,Asin);
			Clr_bit(Global_u16HYP,Sinh);
			Clr_bit(Global_u16HYP,Cosh);
			Clr_bit(Global_u16HYP,Tanh);

			Clr_bit(Global_u16FLAGS, last_is_tri);

			if(Global_u8PressedKey == '+')
			{
				Global_u8Operations[Global_u8It2] ='+';
				LCD_enuSendChar('+');
			}
			else if(Global_u8PressedKey == '-')
			{
				Global_u8Operations[Global_u8It2] ='-';
				LCD_enuSendChar('-');
			}
			else if(Global_u8PressedKey == '*')
			{
				Global_u8Operations[Global_u8It2] ='*';
				LCD_enuSendChar('*');
			}
			else if(Global_u8PressedKey == '/')
			{
				Global_u8Operations[Global_u8It2] ='/';
				LCD_enuSendChar('/');
			}
			else if(Global_u8PressedKey == ';')
			{
				Global_u8Operations[Global_u8It2] =';';
				LCD_enuSendChar('^');
			}

			Global_u8It2++;
			Global_f32Num =0;
			Clr_bit(Global_u16FLAGS, last_is_num);
			Clr_bit(Global_u16FLAGS,last_is_tri);
			if(Global_u8Shift>16)
			{
				for(Global_u8It3=0;Global_u8It3<(Global_u8Shift-16); ++Global_u8It3)
				{
					LCD_enuSendCommand(0x18);
					Global_u8ShiftCnt++;
				}
				Global_u8Shift=16;
			}
		}
	}

	return 0;
}
