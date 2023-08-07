/*
 * MATH.h
 *
 *  Created on: Jul 27, 2023
 *      Author: DeLL
 */

#ifndef ATMEGA32_CTOS_SERVICE_MATH_H_
#define ATMEGA32_CTOS_SERVICE_MATH_H_

#define M_PI		3.14159265358979323846

float _pow(float a, char b);

//float _fact(float x);

//f32 ln(f32 x);

float _sin(float x);

float _sqrt(float n);

float expo(float n);

float _cos(float x);

float _tan(float x);

double my_atan (double x);

double my_asin(double x);

double my_acos(double x);

f32 _sinh(f32 x);

f32 _cosh(f32 x);

f32 _tanh(f32 x);

//float lnn(float x);


void Dec2Hex(u16 DecNumber, u8 * HexNumber);
void Dec2Oct(u16 DecNumber, u8 * OctNumber);
void Dec2Bin(u16 DecNumber, u8 * BinNumber);

u16 Hex2Dec(char * HexNumber);
u16 Oct2Dec(u8 * OctNumber);
u16 Bin2Dec(u8 * BinNumber);



#endif /* ATMEGA32_CTOS_SERVICE_MATH_H_ */
