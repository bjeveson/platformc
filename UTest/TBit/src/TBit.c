/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TBit.c                                                         */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.18                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>

#include "VarType.h"
#include "TestDrv.h"
#include "Log.h"
#include "Bit.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void Bit_GetNBits_Case(void *Para);

static void Bit_RotateLeft_Case(void *Para);

static void Bit_GetEdian_Case(void *Para);

static void Bit_Mul_Case(void *Para);

static void Bit_OddEven_Case(void *Para);

static void Bit_CountBit1_Case(void *Para);
/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/

/*============================================================================*/
/*  Function : TBit_AddTestCase                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TBit_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Bit_GetNBits_Case",
            Bit_GetNBits_Case,
            NULL
        },
        {
            "Bit_RotateLeft_Case",
            Bit_RotateLeft_Case,
            NULL
        },
        {
            "Bit_GetEdian_Case",
            Bit_GetEdian_Case,
            NULL
        },        
        {
            "Bit_Mul_Case",
            Bit_Mul_Case,
            NULL
        },      
        {
            "Bit_OddEven_Case",
            Bit_OddEven_Case,
            NULL
        },        
        {
            "Bit_CountBit1_Case",
            Bit_CountBit1_Case,
            NULL
        },            
        {
            "Over",
            NULL,
            NULL
        }
    };
    
    return TD_AddTestCase(TestCase);
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : Bit_GetNBits_Case                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Bit_GetNBits_Case(void *Para)
{
    UINT32 a = 0xd5555555;
    UINT32 Result;
    
    Result = Bit_GetMNBits(a, 1, 0);
    TD_ASSERT_EQUAL(Result, 0);

    Result = Bit_GetMNBits(a, 0, 0);
    TD_ASSERT_EQUAL(Result, 1);

    Result = Bit_GetMNBits(a, 0, 31);
    TD_ASSERT_EQUAL(Result, a);

    Result = Bit_GetMNBits(a, 31, 31);
    TD_ASSERT_EQUAL(Result, 1);

    Result = Bit_GetMNBits(a, 5, 13);
    TD_ASSERT_EQUAL(Result, 0xAA);
    
    return;
}

/*============================================================================*/
/*  Function : Bit_RotateLeft_Case                                            */
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
static void Bit_RotateLeft_Case(void *Para)
{
    UINT32 x = 0x12345678;
    UINT32 Result;

    Result = Bit_RotateLeft(x, 8);
    TD_ASSERT_EQUAL(Result, 0x34567812);
    
    return;
}

/*============================================================================*/
/*  Function : Bit_GetEdian_Case                                              */
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
static void Bit_GetEdian_Case(void *Para)
{
    TD_ASSERT_EQUAL(Bit_GetEdian(), BIT_MACHINE_LITTLE_EDAIN);
    
    return;
}

/*============================================================================*/
/*  Function : Bit_Mul_Case                                                   */
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
static void Bit_Mul_Case(void *Para)
{
    UINT32 a = 12, b = 34;
    UINT32 Result = 408;

    TD_ASSERT_EQUAL(Result, Bit_Mul(a, b));
    
    return;
}

/*============================================================================*/
/*  Function : Bit_OddEven_Case                                               */
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
static void Bit_OddEven_Case(void *Para)
{
    TD_ASSERT_EQUAL(BIT_NUM_EVEN, Bit_OddEven(7));

    return;
}

/*============================================================================*/
/*  Function : Bit_CountBit1_Case                                             */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.21                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Bit_CountBit1_Case(void *Para)
{
    TD_ASSERT_EQUAL(3, Bit_CountBit1(7));
    TD_ASSERT_EQUAL(9, Bit_CountBit1(0xA0888843));
    
    return;
}
