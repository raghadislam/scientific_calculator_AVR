#ifndef BIT_MATH_H
#define BIT_MATH_H



// SET BITNUM AT REG HIGH
#define Set_bit(REG , BITNUM)     (REG |= (1<<(BITNUM)))


// CLEAR BITNUM AT REG TO LOW
#define Clr_bit(REG , BITNUM)     (REG &= (~(1<<(BITNUM))))


// CHANGE VALUE OF BITNUM AT REG 
#define Toggle_bit(REG , BITNUM)  (REG ^= (1<<(BITNUM)))


// GET BIT VALUE 
#define Get_bit(REG , BITNUM)     (((REG) >> (BITNUM)) & (0x01))




#endif
