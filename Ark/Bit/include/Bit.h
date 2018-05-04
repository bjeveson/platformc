/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Bit.h                                                          */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _BIT_H
#define _BIT_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define BIT_MACHINE_BIG_EDAIN       0
#define BIT_MACHINE_LITTLE_EDAIN    1

#define BIT_NUM_EVEN                0
#define BIT_NUM_ODD                 1
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

INT32 Bit_RightShift(INT32 a, UINT32 n);

INT32 Bit_LeftShift(INT32 a,UINT32 n);

UINT32 Bit_GetMNBits(UINT32 a, UINT32 m, UINT32 n);

UINT32 Bit_RotateRight(UINT32 x, UINT32 n);

UINT32 Bit_RotateLeft(UINT32 x, UINT32 n);


INT32 Bit_IsPowerOf2(INT32 a);

INT32 Bit_ReverseNBit(INT32 a, UINT32 n);

UINT32 Bit_CountBit1(UINT32 x);

INT32 Bit_Mul(INT32 x, INT32 y);

#endif

