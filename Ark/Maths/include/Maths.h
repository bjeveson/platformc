/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Maths.h                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _MATHS_H
#define _MATHS_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define MATH_NUM_IS_PALINDROME      1
#define MATH_NUM_NOT_PALINDROME     0
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 Math_Factorial(UINT32 n);

UINT32 Math_IsPalindromeNum(UINT32 num);

UINT32 Math_Random(UINT32 Para);

#endif
