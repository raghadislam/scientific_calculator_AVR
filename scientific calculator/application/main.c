/*
 * main.c
 *
 *  Created on: Jul 25, 2023
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

//f32 ans=0;

f32 Operands[8] = {0};
u8 operations[8] = {0};
u8 Local_u8It1 = 0, Local_u8It2 = 0,k;
s8 y;
u8 Local_u8Iter, j;

u16 n=-1;

u8 arr [17] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};
u8 arr1[17] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','\0'};


f32 num=0;
u8 Local_u8PressedKey = NOT_PRESSED;
u8 i=1;
u16 Local_u8FLAGS = 2; // 0b00000010
u16 Local_u16HYP =0;
f32 ans=0;
u8 shiftCnt = 0;
f32 mode =1;

int main()
{
	LCD_enuInit();
	Keypad_enuInit();


	while(1)
	{
		Local_u8PressedKey = NOT_PRESSED;

		while(Local_u8PressedKey == NOT_PRESSED)
		{
			Keypad_enuGetPressedKey(&Local_u8PressedKey);
			if(Get_bit(Local_u8FLAGS,must_clear) && Local_u8PressedKey != 'c' && Local_u8PressedKey != 'q' && Local_u8PressedKey != 'x' )
			{
				Local_u8PressedKey = NOT_PRESSED;
			}
		}
		Clr_bit(Local_u8FLAGS,must_clear);

		if(Local_u8PressedKey == '!') LCD_enuDisplayString("sqrt(");
		else if (Local_u8PressedKey == 's') LCD_enuDisplayString("sin(");
		else if (Local_u8PressedKey == 'C') LCD_enuDisplayString("cos(");
		else if (Local_u8PressedKey == 't') LCD_enuDisplayString("tan(");
		else if (Local_u8PressedKey == '^') LCD_enuDisplayString("(e^");

		else if (Local_u8PressedKey == 'y') LCD_enuDisplayString("asin(");
		else if (Local_u8PressedKey == '[') LCD_enuDisplayString("acos(");
		else if (Local_u8PressedKey == 'v') LCD_enuDisplayString("atan(");

		else if (Local_u8PressedKey == 'w') LCD_enuDisplayString("sinh(");
		else if (Local_u8PressedKey == 'u') LCD_enuDisplayString("cosh(");
		else if (Local_u8PressedKey == 'p') LCD_enuDisplayString("tanh(");
		else if((Local_u8PressedKey >= '0' && Local_u8PressedKey<='9') || Local_u8PressedKey == '.')
			LCD_enuSendChar(Local_u8PressedKey);

		if((Local_u8PressedKey >= '0' && Local_u8PressedKey<='9') || Local_u8PressedKey == '.' || Local_u8PressedKey == 'e' || Local_u8PressedKey == '&' )
		{
			// take the numbers ( operands, angels, powers.....)

			if(Local_u8PressedKey == '.')
			{
				Set_bit(Local_u8FLAGS,dot);
				continue;
			}
			Set_bit(Local_u8FLAGS,last_is_num) ;
			Clr_bit(Local_u8FLAGS,first);
			Clr_bit(Local_u8FLAGS,last_is_tri);

			if(Local_u8PressedKey == 'e')
			{
				num = M_PI;
				LCD_enuWriteNumber(M_PI);
				continue;
			}
			else if (Local_u8PressedKey == '&'){
				num = ans;
				LCD_enuDisplayString("Ans");
				continue;
			}

			if(Get_bit(Local_u8FLAGS,negative))
			{
				if( Get_bit(Local_u8FLAGS,dot) )
				{
					num = num - ((f32)(Local_u8PressedKey-'0')  / (i*10.0));
					i*=10;

					if(num > 0) num*=-1;
				}
				else
				{
					num = num*10 - (Local_u8PressedKey-'0');

					if(num > 0) num*=-1;
				}

			}
			else
			{
				if( Get_bit(Local_u8FLAGS,dot) )
				{
					num = num + ((f32)(Local_u8PressedKey-'0')  / (i*10.0));
					i*=10;
				}
				else
				{
					num = num*10 + (Local_u8PressedKey-'0');
				}
			}

		}
		else if(Local_u8PressedKey == '=') // calculating the result
		{
			if( Get_bit(Local_u8FLAGS,last_is_num) && !Get_bit(Local_u8FLAGS,first_is_op) && !Get_bit(Local_u8FLAGS,error) )
			{

				if(Get_bit(Local_u8FLAGS,Sin))
				{
					Operands[Local_u8It1] = _sin(num*(M_PI/180.0)*(1/mode));
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u8FLAGS,Cos ))
				{
					Operands[Local_u8It1] = _cos(num*(M_PI/180.0)*(1/mode));
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u8FLAGS,Sqrt))
				{
					if(num < 0)
					{
						Set_bit(Local_u8FLAGS, error);
					}
					Operands[Local_u8It1] = _sqrt(num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u8FLAGS,Tan))
				{
					if(num == 90 || num == -90)
					{
						Set_bit(Local_u8FLAGS, error);
						LCD_enuSendChar(')');

					}
					Operands[Local_u8It1] = _tan(num*(M_PI/180.0)*(1/mode));
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u8FLAGS,Exp ))
				{
					Operands[Local_u8It1] = expo(num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u16HYP,Atan))
				{
					Operands[Local_u8It1] = my_atan(num)*(180.0/M_PI)*mode;
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u16HYP,Asin))
				{
					if(num > 1 || num < -1)
					{
						Set_bit(Local_u8FLAGS, error);
						LCD_enuSendChar(')');
					}
					Operands[Local_u8It1] = my_asin(num)*(180.0/M_PI)*mode;
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u16HYP,Acos))
				{
					if(num > 1 || num < -1)
					{
						Set_bit(Local_u8FLAGS, error);
						LCD_enuSendChar(')');

					}
					Operands[Local_u8It1] = my_acos(num)*(180.0/M_PI)*mode;
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u16HYP,Sinh))
				{
					Operands[Local_u8It1] = _sinh(num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u16HYP,Cosh))
				{
					Operands[Local_u8It1] = _cosh(num);
					LCD_enuSendChar(')');
				}
				else if(Get_bit(Local_u16HYP,Tanh))
				{
					Operands[Local_u8It1] = _tanh(num);
					LCD_enuSendChar(')');
				}
				else
				{
					Operands[Local_u8It1] = num;
				}

				Local_u8FLAGS &= 0xfa1f;
				Local_u16HYP =0;
				Clr_bit(Local_u8FLAGS,last_is_tri);


				if(Get_bit(Local_u8FLAGS,error))
				{
					LCD_enuClearDisplay();
					LCD_enuDisplayString("Math error!!..");
					num =0;
					Local_u8It1 =0;
					Local_u8It2 =0;
					i=1;
					Local_u8FLAGS = 2;

					Local_u8It1 =0;
					Set_bit(Local_u8FLAGS,must_clear);
					continue;
				}

				LCD_enuSendChar(Local_u8PressedKey);
				LCD_enuGoto(2,0);


				for(Local_u8Iter = 0; Local_u8Iter < 8; ++Local_u8Iter)
				{
					if(operations[Local_u8Iter] == ';')
					{

						Operands[Local_u8Iter] = _pow(Operands[Local_u8Iter], Operands[Local_u8Iter + 1]);
						Operands[Local_u8Iter + 1] = 0;
						operations[Local_u8Iter] = 0;
						for(j = Local_u8Iter + 1; j < 7; ++j)
						{
							Operands[j] = Operands[j + 1];
						}
						for(j = Local_u8Iter ; j < 7; ++j)
						{
							operations[j] = operations[j + 1];
						}
						Local_u8Iter--;
					}
				}

				for(Local_u8Iter = 0; Local_u8Iter < 8; ++Local_u8Iter)
				{
					if(operations[Local_u8Iter] == '*' || operations[Local_u8Iter] == '/')
					{
						if(operations[Local_u8Iter] == '*') Operands[Local_u8Iter] *= Operands[Local_u8Iter + 1];
						else Operands[Local_u8Iter] /= Operands[Local_u8Iter + 1];

						Operands[Local_u8Iter + 1] = 0;
						operations[Local_u8Iter] = 0;
						for(j = Local_u8Iter + 1; j < 7; ++j)
						{
							Operands[j] = Operands[j + 1];
						}
						for(j = Local_u8Iter ; j < 7; ++j)
						{
							operations[j] = operations[j + 1];
						}
						Local_u8Iter--;
					}
				}

				while(operations[0])
				{
					if(operations[0] == '+') Operands[0] += Operands[1]; // 5+9  14
					else Operands[0] -= Operands[1];

					Operands[1] = 0;
					operations[0]=0;

					for(j = 1; j < 6; ++j)
					{
						Operands[j] = Operands[j + 1];
					}
					for(j = 0 ; j < 5; ++j)
					{
						operations[j] = operations[j + 1];
					}
				}
				LCD_enuWriteNumber(Operands[0]);
				ans = Operands[0];
				Operands[0] = 0;
				Local_u8It2 = 0, Local_u8It1 = 0;

				if(shiftCnt > 0)
				{
					for(k=0; k<shiftCnt ; k++)
					{
						LCD_enuSendCommand(0x1c);
					}
				}

				else if(shiftCnt < 0)
				{
					shiftCnt *= -1;
					for(k=0; k<shiftCnt ; k++)
					{
						LCD_enuSendCommand(0x18);
					}
				}

				for(k =0; k<8; ++k)
				{
					operations[k]= 0;
					Operands[k]=0;
				}
				num=0;

				Local_u8FLAGS = 2;
				Local_u16HYP = 0;

				i=1;
				Local_u8It1 =0;
				Local_u8It2 =0;
				Set_bit(Local_u8FLAGS,must_clear);

				shiftCnt = 0;
			}
			else
			{
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Math error!!.."); // something went wrong
				num = 0;
				i=1;

				Local_u8It1 =0;
				Local_u8It2 =0;

				shiftCnt = 0;
				Local_u8FLAGS = 2;
				Local_u16HYP = 0;
				continue;
			}

		}
		else if (Local_u8PressedKey == 'c') // clear the display
		{
			LCD_enuClearDisplay();
			num = 0;
			Local_u8FLAGS = 2;
			Local_u16HYP = 0;

			Local_u8It2 =0;
			shiftCnt = 0;
			Local_u8It1 =0;
			i=1;

		}
	        	/********************************** Number representation **********************************/

		else if (Local_u8PressedKey == 'i' || Local_u8PressedKey == 'z' || Local_u8PressedKey == 'a' || Local_u8PressedKey =='o')
		{
			Clr_bit(Local_u16HYP,NUMerror);
			num = 0;

			LCD_enuGoto(1,12);

			if(Local_u8PressedKey == 'i')
			{
				Set_bit(Local_u16HYP, DEC);
				for(y =0; y<16; ++y) arr[y]=' ';
				LCD_enuDisplayString("DEC");
			}
			else if(Local_u8PressedKey == 'z')
			{
				Set_bit(Local_u16HYP, BIN);
				LCD_enuDisplayString("BIN");
			}
			else if(Local_u8PressedKey == 'a')
			{
				Set_bit(Local_u16HYP, OCT);
				LCD_enuDisplayString("OCT");
			}
			else if(Local_u8PressedKey == 'o')
			{
				Set_bit(Local_u16HYP, HEX);
				LCD_enuDisplayString("HEX");
			}

			LCD_enuGoto(2,0);

			while (Local_u8PressedKey != '=')
			{
				Local_u8PressedKey = NOT_PRESSED;

				while(Local_u8PressedKey == NOT_PRESSED)
				{
					Keypad_enuGetPressedKey(&Local_u8PressedKey);
				}

				if((Local_u8PressedKey >= '0' && Local_u8PressedKey<='9') || Local_u8PressedKey == '.' )
				{
					LCD_enuSendChar(Local_u8PressedKey);

					if(Get_bit(Local_u16HYP,NUMerror)) continue;
					else if(Local_u8PressedKey == '.')
					{
						Set_bit(Local_u16HYP,NUMerror);
						continue;
					}
					else if(Get_bit(Local_u16HYP, OCT) && Local_u8PressedKey >'7' && Local_u8PressedKey <='9' )
					{
						Set_bit(Local_u16HYP,NUMerror);
						continue;
					}
					else if (Get_bit(Local_u16HYP, BIN) && Local_u8PressedKey != '0' && Local_u8PressedKey != '1')
					{
						Set_bit(Local_u16HYP,NUMerror);
						continue;
					}
					else{
						num = num*10 + (Local_u8PressedKey-'0');
						for(y = 15 ; y > 0; --y)
						{
							arr[y] = arr[y-1];
						}
						arr[0] = Local_u8PressedKey;
					}
				}
				else if(Local_u8PressedKey == 'x' || Local_u8PressedKey == 'q' ||Local_u8PressedKey == 'i' ||Local_u8PressedKey == 'o' ||Local_u8PressedKey == 'z' ||Local_u8PressedKey == 'a')
				{

					if(Local_u8PressedKey == 'x')LCD_enuSendChar('A');
					else if(Local_u8PressedKey == 'x')LCD_enuSendChar('A');
					else if(Local_u8PressedKey == 'q')LCD_enuSendChar('B');
					else if(Local_u8PressedKey == 'i')LCD_enuSendChar('C');
					else if(Local_u8PressedKey == 'o')LCD_enuSendChar('D');
					else if(Local_u8PressedKey == 'z')LCD_enuSendChar('E');
					else if(Local_u8PressedKey == 'a')LCD_enuSendChar('F');

					if(!Get_bit(Local_u16HYP, HEX)) Set_bit(Local_u16HYP,NUMerror);
					else
					{
						for(y = 15 ; y > 0; --y)
						{
							arr[y] = arr[y-1];
						}

						if(Local_u8PressedKey =='x') arr[0] = 'A';
						if(Local_u8PressedKey =='q') arr[0] = 'B';
						if(Local_u8PressedKey =='i') arr[0] = 'C';
						if(Local_u8PressedKey =='o') arr[0] = 'D';
						if(Local_u8PressedKey =='z') arr[0] = 'E';
						if(Local_u8PressedKey =='a') arr[0] = 'F';
					}

				}
				else if(Local_u8PressedKey != '=' && Local_u8PressedKey != 'c' )
				{
					Set_bit(Local_u16HYP,NUMerror);

					if(Local_u8PressedKey != '+' && Local_u8PressedKey != '-' && Local_u8PressedKey != '*' && Local_u8PressedKey != '/')
					{
						Set_bit(Local_u16HYP,bracets);
					}
					if(Local_u8PressedKey == '!') LCD_enuDisplayString("sqrt(");
					else if (Local_u8PressedKey == 's')LCD_enuDisplayString("sin(");
					else if (Local_u8PressedKey == 'C')LCD_enuDisplayString("cos(");
					else if (Local_u8PressedKey == 't')LCD_enuDisplayString("tan(");
					else if (Local_u8PressedKey == '^')LCD_enuDisplayString("(e^");

					else if (Local_u8PressedKey == 'y')LCD_enuDisplayString("asin(");
					else if (Local_u8PressedKey == '[')LCD_enuDisplayString("acos(");
					else if (Local_u8PressedKey == 'v')LCD_enuDisplayString("atan(");

					else if (Local_u8PressedKey == 'w')LCD_enuDisplayString("sinh(");
					else if (Local_u8PressedKey == 'u')LCD_enuDisplayString("cosh(");
					else if (Local_u8PressedKey == 'p')LCD_enuDisplayString("tanh(");
					else{
						if(Get_bit(Local_u16HYP,bracets)) LCD_enuSendChar(')');
						LCD_enuSendChar(Local_u8PressedKey);
						Clr_bit(Local_u16HYP,bracets);
					}
				}
				else if(Local_u8PressedKey == '=')
				{
					if(Get_bit(Local_u16HYP,bracets)) LCD_enuSendChar(')');
					break;
				}
				else if(Local_u8PressedKey == 'c')
				{
					LCD_enuClearDisplay();
					num =0;
					Local_u8FLAGS = 2;
					Local_u16HYP = 0;
					Local_u8It2 =0;
					Local_u8It1 =0;
					shiftCnt = 0;
					LCD_enuSendCommand(0x06);
					i=1;
					Clr_bit(Local_u16HYP,NUMerror);
					break;
				}
			}

			if(Get_bit(Local_u16HYP,NUMerror) && Local_u8PressedKey!='c' )
			{
				LCD_enuClearDisplay();
				LCD_enuDisplayString("Math error!!..");
				num = 0;
				i=1;

				Local_u8It1 =0;
				Local_u8It2 =0;

				Local_u8FLAGS = 2;
				Local_u16HYP = 0;
				shiftCnt=0;
				continue;
			}
			else if (Local_u8PressedKey!='c')
			{
				LCD_enuGoto(2,15);
				LCD_enuSendCommand(0x04);
				LCD_enuDisplayString(arr);
				while(1)
				{

					Local_u8PressedKey = NOT_PRESSED;

					while(Local_u8PressedKey != 'i' && Local_u8PressedKey != 'z' && Local_u8PressedKey != 'a' && Local_u8PressedKey != 'o' &&  Local_u8PressedKey != 'c' )
					{
						Keypad_enuGetPressedKey(&Local_u8PressedKey);
					}
					if(Local_u8PressedKey == 'c')
					{
						LCD_enuClearDisplay();
						num =0;
						Local_u8FLAGS = 2;
						for(y=0 ;y<16;++y)arr[y]='0';
						for(y=0 ;y<16;++y)arr1[y]='0';
						Local_u16HYP = 0;
						Local_u8It2 =0;
						Local_u8It1 =0;
						i=1;
						break;
					}
					else if(Get_bit(Local_u16HYP, DEC))
					{
						for(y=0 ;y<16;++y)arr[y]='0';

						if(Local_u8PressedKey == 'i')
						{
							k=0;
							n = num;
							do {
								n /= 10;
								++k;
							} while (n != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(y=0;  y < 15-k+1 ; ++y) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-k+1);
							LCD_enuWriteNumber(num);
							LCD_enuSendCommand(0x04);
						}
						else if(Local_u8PressedKey == 'o')
						{
							Dec2Hex(num,arr);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr[y]);
						}
						else if(Local_u8PressedKey == 'z')
						{
							Dec2Bin(num,arr);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr[y]);
						}
						else if(Local_u8PressedKey == 'a')
						{
							Dec2Oct(num,arr);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr[y]);
						}
					}
					else if(Get_bit(Local_u16HYP, OCT))
					{
						for(y=0 ;y<16;++y)arr1[y]  ='0';
						if(Local_u8PressedKey == 'i')
						{
							k=0;
							n = Oct2Dec(arr);
							do {
								n /= 10;
								++k;
							} while (n != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(y=0;  y < 15-k+1 ; ++y) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-k+1);
							LCD_enuWriteNumber(Oct2Dec(arr));
							LCD_enuSendCommand(0x04);
						}
						else if(Local_u8PressedKey == 'o')
						{
							n = Oct2Dec(arr);
							Dec2Hex(n,arr1);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr1[y]);
						}
						else if(Local_u8PressedKey == 'z')
						{
							n = Oct2Dec(arr);
							Dec2Bin(n,arr1);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr1[y]);
						}
						else if(Local_u8PressedKey == 'a')
						{
							LCD_enuGoto(2,15);
							LCD_enuSendCommand(0x04);
							LCD_enuDisplayString(arr);
						}
					}
					else if(Get_bit(Local_u16HYP, BIN))
					{
						for(y=0 ;y<16;++y)arr1[y]  ='0';
						if(Local_u8PressedKey == 'i')
						{
							k=0;
							n = Bin2Dec(arr);
							do {
								n /= 10;
								++k;
							} while (n != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(y=0;  y < 15-k+1 ; ++y) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-k+1);
							LCD_enuWriteNumber(Bin2Dec(arr));
							LCD_enuSendCommand(0x04);
						}
						else if(Local_u8PressedKey == 'o')
						{	n = Bin2Dec(arr);
							Dec2Hex(n,arr1);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr1[y]);
						}
						else if(Local_u8PressedKey == 'z')
						{
							LCD_enuGoto(2,15);
							LCD_enuSendCommand(0x04);
							LCD_enuDisplayString(arr);
						}
						else if(Local_u8PressedKey == 'a')
						{
							n = Bin2Dec(arr);
							Dec2Oct(n,arr1);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr1[y]);
						}
					}
					else if(Get_bit(Local_u16HYP, HEX))
					{
						for(y=0 ;y<16;++y)arr1[y]  ='0';
						if(Local_u8PressedKey == 'i')
						{
							k=0;
							n = Hex2Dec(arr);
							do {
								n /= 10;
								++k;
							} while (n != 0);
							LCD_enuSendCommand(0x06);
							LCD_enuGoto(2,0);
							for(y=0;  y < 15-k+1 ; ++y) LCD_enuSendChar(' ');
							LCD_enuGoto(2,15-k+1);
							LCD_enuWriteNumber(Hex2Dec(arr));
							LCD_enuSendCommand(0x04);
						}
						else if(Local_u8PressedKey == 'o')
						{
							LCD_enuGoto(2,15);
							LCD_enuSendCommand(0x04);
							LCD_enuDisplayString(arr);
						}
						else if(Local_u8PressedKey == 'z')
						{
							n = Hex2Dec(arr);
							Dec2Bin(n,arr1);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr1[y]);
						}
						else if(Local_u8PressedKey == 'a')
						{
							n = Hex2Dec(arr);
							Dec2Oct(n,arr1);
							LCD_enuGoto(2,15);
							for(y=15; y>=0; --y) LCD_enuSendChar(arr1[y]);
						}

					}

				}

			}
		}
		else if(Local_u8PressedKey == 'q') // shift right
		{
			LCD_enuSendCommand(0x18);
			shiftCnt++;
		}
		else if(Local_u8PressedKey == 'x') // shift left
		{
			LCD_enuSendCommand(0x1c);
			shiftCnt--;
		}
		else if(Local_u8PressedKey == 'b') // Redian mode
		{
			mode=(M_PI)/(180.0);
		}
		else if(Local_u8PressedKey == 'r') // Degree mode
		{
			mode=1;
		}
		else							   // Handling all operations
		{

			Clr_bit(Local_u8FLAGS,dot);
			Clr_bit(Local_u8FLAGS,negative);
			i=1;

			if(     Local_u8PressedKey == 't' || Local_u8PressedKey == 'C' || Local_u8PressedKey == 's' ||
					Local_u8PressedKey == '!' || Local_u8PressedKey == '^' || Local_u8PressedKey == 'v' ||
					Local_u8PressedKey == 'y' || Local_u8PressedKey == '[' || Local_u8PressedKey == 'w' ||
					Local_u8PressedKey == 'u' || Local_u8PressedKey == 'p')
			{
				switch (Local_u8PressedKey)
				{
				case 't':	Set_bit(Local_u8FLAGS, Tan);
				break;
				case 'C':	Set_bit(Local_u8FLAGS, Cos);
				break;
				case 's':	Set_bit(Local_u8FLAGS, Sin);
				break;
				case '!':	Set_bit(Local_u8FLAGS, Sqrt);
				break;
				case '^':	Set_bit(Local_u8FLAGS, Exp);
				break;
				case 'v':   Set_bit(Local_u16HYP, Atan);
				break;
				case 'y':   Set_bit(Local_u16HYP, Asin);
				break;
				case '[':   Set_bit(Local_u16HYP, Acos);
				break;
				case 'w':   Set_bit(Local_u16HYP, Sinh);
				break;
				case 'u':   Set_bit(Local_u16HYP, Cosh);
				break;
				case 'p':   Set_bit(Local_u16HYP, Tanh);
				break;
				}
				Set_bit(Local_u8FLAGS,last_is_tri);
				continue;
			}

			if(Get_bit(Local_u8FLAGS,first) && Local_u8PressedKey != '+' && Local_u8PressedKey != '-' )
			{
				if(Local_u8PressedKey != ';')
				LCD_enuSendChar(Local_u8PressedKey);
				Set_bit(Local_u8FLAGS,first_is_op);
			}
			Clr_bit(Local_u8FLAGS,first);

			if(Get_bit(Local_u8FLAGS,last_is_tri) && (Local_u8PressedKey == '*' || Local_u8PressedKey == '/'|| Local_u8PressedKey == ';') )
			{
				Set_bit(Local_u8FLAGS,error);
			}

			if(!Get_bit(Local_u8FLAGS,last_is_num))
			{

				if(Local_u8PressedKey == '-'){
					Set_bit(Local_u8FLAGS, negative);
					LCD_enuSendChar('-');
					continue;
				}
				else if(Local_u8PressedKey == '*' || Local_u8PressedKey == '/')
				{
					Set_bit(Local_u8FLAGS, error);
					continue;
				}
				else if(Local_u8PressedKey == '+')
				{
					LCD_enuSendChar('+');
					continue;
				}
			}

			if(Get_bit(Local_u8FLAGS,Sin))
			{
				Operands[Local_u8It1] = _sin(num*(M_PI/180.0)*(1/mode));
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u8FLAGS,Cos ))
			{
				Operands[Local_u8It1] = _cos(num*(M_PI/180.0)*(1/mode));
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u8FLAGS,Sqrt))
			{
				if(num < 0)
				{
					Set_bit(Local_u8FLAGS, error);
					LCD_enuSendChar(')');

				}
				Operands[Local_u8It1] = _sqrt(num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u8FLAGS,Tan))
			{
				if(num == 90 || num == -90 )
				{
					Set_bit(Local_u8FLAGS, error);
				}
				Operands[Local_u8It1] = _tan(num*(M_PI/180.0)*(1/mode));
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u8FLAGS,Exp ))
			{
				Operands[Local_u8It1] = expo(num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u16HYP,Atan ))
			{
				Operands[Local_u8It1] = my_atan(num)*(180.0/M_PI)*mode;
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u16HYP,Asin ))
			{
				if(num > 1 || (num < -1))
				{
					Set_bit(Local_u8FLAGS, error);
				}
				Operands[Local_u8It1] = my_asin(num)*(180.0/M_PI)*mode;
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u16HYP,Acos ))
			{
				if(num > 1 || (num < -1 ))
				{
					Set_bit(Local_u8FLAGS, error);
				}
				Operands[Local_u8It1] = my_acos(num)*(180.0/M_PI)*mode;
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u16HYP,Sinh ))
			{
				Operands[Local_u8It1] = _sinh(num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u16HYP,Cosh ))
			{
				Operands[Local_u8It1] = _cosh(num);
				LCD_enuSendChar(')');
			}
			else if(Get_bit(Local_u16HYP,Tanh ))
			{
				Operands[Local_u8It1] = _tanh(num);
				LCD_enuSendChar(')');
			}
			else{ Operands[Local_u8It1] = num;
			}
			Local_u8It1++;

			Clr_bit(Local_u8FLAGS,Sin);
			Clr_bit(Local_u8FLAGS,Cos );
			Clr_bit(Local_u8FLAGS,Sqrt);
			Clr_bit(Local_u8FLAGS,Tan);
			Clr_bit(Local_u8FLAGS,Exp);
			Clr_bit(Local_u16HYP,Atan);
			Clr_bit(Local_u16HYP,Acos);
			Clr_bit(Local_u16HYP,Asin);
			Clr_bit(Local_u16HYP,Sinh);
			Clr_bit(Local_u16HYP,Cosh);
			Clr_bit(Local_u16HYP,Tanh);

			Clr_bit(Local_u8FLAGS, last_is_tri);

			if(Local_u8PressedKey == '+')
			{
				operations[Local_u8It2] ='+';
				LCD_enuSendChar('+');
			}
			else if(Local_u8PressedKey == '-')
			{
				operations[Local_u8It2] ='-';
				LCD_enuSendChar('-');
			}
			else if(Local_u8PressedKey == '*')
			{
				operations[Local_u8It2] ='*';
				LCD_enuSendChar('*');
			}
			else if(Local_u8PressedKey == '/')
			{
				operations[Local_u8It2] ='/';
				LCD_enuSendChar('/');
			}
			else if(Local_u8PressedKey == ';')
			{
				operations[Local_u8It2] =';';
				LCD_enuSendChar('^');
			}

			Local_u8It2++;
			num =0;
			Clr_bit(Local_u8FLAGS, last_is_num);
			Clr_bit(Local_u8FLAGS,last_is_tri);
		}

	}

	return 0;
}
