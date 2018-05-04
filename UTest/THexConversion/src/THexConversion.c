/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : THexConversion.c                                               */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.19                                                      */
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
static void THexcon_DecNumFormateToStr_Case(void *Para);

static void THexcon_DecStrFormateToNum_Case(void *Para);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
UINT32 THexcon_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "THexcon_DecNumFormateToStr_Case",
            THexcon_DecNumFormateToStr_Case,
            NULL
        },
        {
            "THexcon_DecStrFormateToNum_Case",
            THexcon_DecStrFormateToNum_Case,
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
/*  Function : Hexcon_DecNumFormateToStr_Case                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.19                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void THexcon_DecNumFormateToStr_Case(void *Para)
{
    UINT32 Num[] = {123, 456, 789, 0};
    UINT8 NumStr[128] = {0};
    UINT8 *pucresult = (UINT8 *)"123 456 789 0";
    UINT8 cmpResult = 0;
    
    Hexcon_DecNumFormateToStr(Num, ARR_SIZE(Num), NumStr);

    cmpResult = strcmp(NumStr, pucresult);
    TD_ASSERT_EQUAL(cmpResult, 0);
    
    return;
}

/*============================================================================*/
/*  Function : Hexcon_DecStrFormateToNum_Case                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.19                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void THexcon_DecStrFormateToNum_Case(void *Para)
{
    UINT8 *pucStr = "123 456 789 0 888";
    UINT32 Num[5], NumCount;
    
    NumCount = Hexcon_DecStrFormateToNum(pucStr, Num);

    TD_ASSERT_EQUAL(NumCount, 5);
    TD_ASSERT_EQUAL(Num[0], 123);
    TD_ASSERT_EQUAL(Num[1], 456);
    TD_ASSERT_EQUAL(Num[2], 789);
    TD_ASSERT_EQUAL(Num[3], 0);
    TD_ASSERT_EQUAL(Num[4], 888);
    
    return;
}

