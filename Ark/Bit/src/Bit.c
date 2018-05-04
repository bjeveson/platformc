/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Bit.c                                                          */
/*  Descript : Bit Operation Function                                         */
/*  API Pref : Bit                                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include "Arch.h"
#include "VarType.h"

#include "Bit.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Bit_RightShift                                                 */
/*  Input    : a   --Operation Num                                            */
/*             n   --Shift Bit Num                                            */
/*  Output   :                                                                */
/*  Return   : Result Of Right Shift                                          */
/*  Desc     : a Right Shift n bit                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Bit_RightShift(INT32 a, UINT32 n)
{
	INT32 b, c;

	b = (~(~0 << n) & a) << (32 - n);
	a >>= n;
	a &= ~((~0) << (32 - n));
	c = a | b;

	return c;
}

/*============================================================================*/
/*  Function : Bit_RightShift                                                 */
/*  Input    : a   --Operation Num                                            */
/*             n   --Shift Bit Num                                            */
/*  Output   :                                                                */
/*  Return   : Result Of Left Shift                                           */
/*  Desc     : a left Shift n bit                                             */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Bit_LeftShift(INT32 a,UINT32 n)
{
	INT32 b,c;

	b = a >> (32 - n);
	b = b & ~(~0 << n);
	a = a << n;
	c = a | b;

	return c;
}

/*============================================================================*/
/*  Function : Bit_GetNBits                                                   */
/*  Input    : a   --Operation Num                                            */
/*             n   --Bit Num                                                  */
/*  Output   :                                                                */
/*  Return   : Result Of a's m-n bit                                          */
/*  Desc     : Return a's m-n bit                                             */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Bit_GetMNBits(UINT32 a, UINT32 m, UINT32 n)
{
	UINT32 len = n - m + 1;

    if (n < m || m > 31 || n > 31)
    {
        return 0;
    }

    if (n == m)
    {
        return (a >> m) & 0x01;
    }

    if (m == 0 && n == 31)
    {
        return a;
    }
    
	return (a >> m) & ~(~0 << len);
}

/*============================================================================*/
/*  Function : Bit_GetNByte                                                   */
/*  Input    : %1 -- Operation Num                                            */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Get N Byte                                                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 Bit_GetNByte(UINT32 ulOperNum, UINT8 ucNByte)
{
    if ((ucNByte < 1) || (ucNByte > 4))
    {
        return 0;
    }

    return (ulOperNum >> ((ucNByte - 1) * 8)) & 0xFF;
}

/*============================================================================*/
/*  Function : Bit_IsPowerOf2                                                 */
/*  Input    : a --operation Num                                              */
/*  Output   :                                                                */
/*  Return   : 0 -- Is Powerof2                                               */
/*             1 -- Not Powerof2                                              */
/*  Desc     : Judge a is PowerOf2                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Bit_IsPowerOf2(INT32 a)
{
	return a & (a - 1) ?  0 : 1;
}

/*============================================================================*/
/*  Function : Bit_ReverseNBit                                                */
/*  Input    : a   --Operation Num                                            */
/*             n   --Bit Num                                                  */
/*  Output   :                                                                */
/*  Return   : Result Of Reverse                                              */
/*  Desc     : a Reverse Bit N 0->1, 1->0                                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Bit_ReverseNBit(INT32 a, UINT32 n)
{
	return a ^ (1 << n);
}

/*============================================================================*/
/*  Function : Bit_CountBit1                                                  */
/*  Input    : x -- Operation Num                                             */
/*  Output   :                                                                */
/*  Return   : Num Of Bit 1                                                   */
/*  Desc     : Return Num Of Bit is 1                                         */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Bit_CountBit1(UINT32 x)
{
	UINT32 count = 0;

    do
    {
        if (x & 0x01)
            count++;
    }while (x >>= 0x01);

	return count;
}

/*============================================================================*/
/*  Function : Bit_RotateLeft                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.18                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Bit_RotateLeft(UINT32 x, UINT32 n)
{
    if (n > 31)
    {
        return 0;
    }
    
    return (x << n) | ((x) >> ((8 * sizeof(x)) - n));
}

/*============================================================================*/
/*  Function : Bit_RotateRight                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.18                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Bit_RotateRight(UINT32 x, UINT32 n)
{
    if (n > 31)
    {
        return 0;
    }

    return (x >> n) | (x << ((8 * sizeof(x)) - n));
}

/*============================================================================*/
/*  Function : Bit_Mul                                                        */
/*  Input    : x -- Operation Num1                                            */
/*  Input    : y -- Operation Num1                                            */
/*  Output   :                                                                */
/*  Return   : Result of x Multiply y                                         */
/*  Desc     : Return Result x * y By Bit Opertaion                           */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Bit_Mul(INT32 x, INT32 y)
{

	INT32 sum = 0, flag = 0;

	if (x < 0) 
	{
		x = -x;
		flag = 1;
	}

	while (x != 0) 
	{
		if ((x & 0x01) == 1) 
		{
			 sum += y;
        }
        y <<= 1;
        x >>= 1;
	}
	
	if (flag == 1)
	{
		sum = -sum;
    }
    
	return sum;
}

/*============================================================================*/
/*  Function : Bit_GetEdian                                                   */
/*  Input    : %1 -- void                                                     */
/*  Output   :                                                                */
/*  Return   : 0:big edian                                                    */
/*             1:small edian                                                  */
/*  Desc     : Judge this machine edian                                       */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.22                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Bit_GetEdian(void)
{
    UINT32 i = 1;

    if (*(INT8 *)(&i) == 0)
    {
        return BIT_MACHINE_BIG_EDAIN;
    }
    else
    {
        return BIT_MACHINE_LITTLE_EDAIN;
    }
}

/*============================================================================*/
/*  Function : Bit_OddEven                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.1                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Bit_OddEven(UINT32 Num)
{
    if (1 == (Num & 0x01))
    {
        return BIT_NUM_EVEN;
    }
    else
    {
        return BIT_NUM_ODD;
    }
}
/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

