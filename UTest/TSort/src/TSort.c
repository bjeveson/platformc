/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TSort.c                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.31                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>

#include "VarType.h"
#include "ErrorCode.h"
#include "TestDrv.h"
#include "Log.h"
#include "Sort.h"
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
static void Sort_Bubble_Case(void *Para);

static UINT32 StringCompare(void *p, void *q);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
UINT32 TSort_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Sort_Bubble_Case",
            Sort_Bubble_Case,
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
/*  Function : Sort_Bubble_Case                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.31                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Sort_Bubble_Case(void *Para)
{
    UINT8 *pucStr[5];
    void *pResult[5];
    UINT32 i = 0;
    UINT32 ulRet;
    
    pResult[0] = pucStr[0] = (UINT8 *)"I Want Money";    // 4
    pResult[1] = pucStr[1] = (UINT8 *)"I Want Journey";  // 3
    pResult[2] = pucStr[2] = (UINT8 *)"I Want Time";     // 5
    pResult[3] = pucStr[3] = (UINT8 *)"I Want Healthy";  // 2
    pResult[4] = pucStr[4] = (UINT8 *)"I Want Girl";     // 1
    
    Sort_Bubble(pResult, 5, StringCompare);

    ulRet = strcmp(pResult[0], pucStr[4]);
    TD_ASSERT_EQUAL(ulRet, 0);

    ulRet = strcmp(pResult[1], pucStr[3]);
    TD_ASSERT_EQUAL(ulRet, 0);    

    ulRet = strcmp(pResult[2], pucStr[1]);
    TD_ASSERT_EQUAL(ulRet, 0);

    ulRet = strcmp(pResult[3], pucStr[0]);
    TD_ASSERT_EQUAL(ulRet, 0);

    ulRet = strcmp(pResult[4], pucStr[2]);
    TD_ASSERT_EQUAL(ulRet, 0);    
    
    return;
}

/*============================================================================*/
/*  Function : StringCompare                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.31                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 StringCompare(void *p, void *q)
{   
	if (strcmp((UINT8 *)p, (UINT8 *)q) > 0)
	{
		return COMPARE_MORE;
    }
	else if (strcmp((UINT8 *)p, (UINT8 *)q) <0)
	{
		return COMPARE_LESS;
	}
	else
	{
		return COMPARE_EQUAL;
    }
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


