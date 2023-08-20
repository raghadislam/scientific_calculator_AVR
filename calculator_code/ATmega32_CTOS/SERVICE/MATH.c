
#include "STDTypes.h"
#include "errorState.h"
#include "util/delay.h"
#include "math.h"


#include "../MCAL/DIO/DIO_int.h"

#include "../HAL/LCD/LCD_int.h"
#include "../HAL/Keypad/Keypad_int.h"



float _pow(float a, signed char b) {
    if (b == 0)
        return 1;
    else if (b == 1)
        return a;
    else if (b < 0)
        return 1 / _pow(a, -b);
    else {
        float temp = _pow(a * a, b / 2);
        if ((int)b % 2 == 0)
            return temp;
        else
            return a * temp;
    }
}

float _cos(float x)
{
    while (x < -M_PI) x += 2 * M_PI;
    while (x > M_PI) x -= 2 * M_PI;

    u8 f =0;

    if((x > 3*M_PI/2.0 && x < 2*M_PI) || (x<0 && x>= -1*M_PI/2)) {
    	x = 2.0*M_PI + x ;
    	f=2;
    }


    if(x > 0 && x < M_PI/2) {
    	x = M_PI -x ;
    	f=1;
    }


    double result = 1.0;
    double term = 1.0;
    double x_squared = x * x;
    int sign = -1;

    for (int i = 2; i <= 20; i += 2) {
        term *= x_squared / ((i - 1) * i);
        result += sign * term;
        sign *= -1;
    }

    if(result > -0.0001 && result < 0.0001 ) return 0;

    if(f==1) return -1*result;
    return result;
}

float _sin(float x)
{
	return -1 * _cos((M_PI/2) + x);
}

float _tan(float x)
{
	return (_sin(x)/_cos(x));
}

float expo(float n)
{
    int a = 0, b = n > 0;
    float c = 1, d = 1, e = 1;
    for (b || (n = -n); e + .00001 < (e += (d *= n) / (c *= ++a)););
    return b ? e : 1 / e;
}


float _sqrt(float n)
{

    float x = n;
    float y = 1;

    double e = 0.000001;
    while(x - y > e)
    {
        x = (x + y)/2;
        y = n/x;
    }
    return x;
}


double my_atan (double x)
{


    double a, z, p, r, q, s, t;
    /* argument reduction:
       arctan (-x) = -arctan(x);
       arctan (1/x) = 1/2 * pi - arctan (x), when x > 0
    */

    if (x>=1000000) return M_PI/2;
    else if (x<-100000)return -M_PI/2;
    z = fabs (x);
    a = (z > 1.0) ? (1.0 / z) : z;
    s = a * a;
    q = s * s;
    /* core approximation: approximate atan(x) on [0,1] */
    p =            -2.0258553044340116e-5;  // -0x1.53e1d2a258e3ap-16
    t =             2.2302240345710764e-4;  //  0x1.d3b63dbb6167ap-13
    p = fma (p, q, -1.1640717779912220e-3); // -0x1.312788ddde71dp-10
    t = fma (t, q,  3.8559749383656407e-3); //  0x1.f9690c824aaf1p-9
    p = fma (p, q, -9.1845592187222193e-3); // -0x1.2cf5aabc7dbd2p-7
    t = fma (t, q,  1.6978035834594660e-2); //  0x1.162b0b2a3bcdcp-6
    p = fma (p, q, -2.5826796814492296e-2); // -0x1.a7256feb6f841p-6
    t = fma (t, q,  3.4067811082715810e-2); //  0x1.171560ce4a4ecp-5
    p = fma (p, q, -4.0926382420509999e-2); // -0x1.4f44d841450e8p-5
    t = fma (t, q,  4.6739496199158334e-2); //  0x1.7ee3d3f36bbc6p-5
    p = fma (p, q, -5.2392330054601366e-2); // -0x1.ad32ae04a9fd8p-5
    t = fma (t, q,  5.8773077721790683e-2); //  0x1.e17813d669537p-5
    p = fma (p, q, -6.6658603633512892e-2); // -0x1.11089ca9a5be4p-4
    t = fma (t, q,  7.6922129305867892e-2); //  0x1.3b12b2db5173cp-4
    p = fma (p, s, t);
    p = fma (p, s, -9.0909012354005267e-2); // -0x1.745d022f8dc5fp-4
    p = fma (p, s,  1.1111110678749421e-1); //  0x1.c71c709dfe925p-4
    p = fma (p, s, -1.4285714271334810e-1); // -0x1.2492491fa1742p-3
    p = fma (p, s,  1.9999999999755005e-1); //  0x1.99999999840cdp-3
    p = fma (p, s, -3.3333333333331838e-1); // -0x1.5555555555448p-2
    p = fma (p * s, a, a);
    /* back substitution in accordance with argument reduction */
    /* double-precision factorization of PI/2 courtesy of Tor Myklebust */
    r = (z > 1.0) ? fma (0.93282184640716537, 1.6839188885261840, -p) : p;
    return (copysign (r, x) ) ;
}

double my_asin(double x)
{
	return (my_atan(x/sqrt(1-(x*x))));
}

double my_acos(double x)
{
	return M_PI*0.5 - my_asin(x);
}




/****************** Number Representation ******************/


s8 i;
void Dec2Hex(u16 DecNumber, u8 * HexNumber)
{
	 i = 0;
	while (DecNumber != 0)
	{
		if ((DecNumber % 16) < 10)
		{
			HexNumber[15-i] = (DecNumber % 16) + '0';
		}
		else
		{
			HexNumber[15-i] = (DecNumber % 16) + 'A' - 10;
		}
		DecNumber /= 16;
		i++;
	}
}


void Dec2Oct(u16 DecNumber, u8 * OctNumber)
{
	i = 0;
	while (DecNumber != 0)
	{
		OctNumber[15-i] = (DecNumber % 8) + '0';
		DecNumber /= 8;
		i++;
	}
}


void Dec2Bin(u16 DecNumber, u8 * BinNumber)
{
	 i = 0;
	while (DecNumber != 0)
	{
		BinNumber[15-i] = (DecNumber % 2) + '0';
		DecNumber /= 2;
		i++;
	}
}



u16 Hex2Dec(char * HexNumber)
{
	u16 val = 0;
	u8 byte;
	for( i =15;i>=0;i-- )
	{
		byte = HexNumber[i];
		if (byte >= '0' && byte <= '9') byte = byte - '0';
		else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
		else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
		val = (val << 4) | byte; // 0000 0000 0000 0000
	}
	return val;                               //      123      32100000000000000000/0
}

u16 Oct2Dec(u8 * OctNumber)
{
	u64 val = 0;
	u8 byte;
	for( i =15;i>=0;i-- )
	{
		byte = OctNumber[i];
		val = (val << 3) | (byte - '0');
	}


	return val;
}



u16 Bin2Dec(u8 * BinNumber)
{
	u16 val = 0;
	u8 byte;
	for( i =15;i>=0;i-- )
	{
		byte = BinNumber[i]; // 1 0 1 0 0 0 0 0 0 0 0 0
		val = (val << 1) | (byte - '0');
	}
	return val;
}




f32 _sinh(f32 x)
{
	return (expo(x) - expo(-x))/2;
}



f32 _cosh(f32 x)
{
	return (expo(x) + expo(-x))/2;
}



f32 _tanh(f32 x)
{
	return _sinh(x)/_cosh(x);
}
