/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TSearch.c                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
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

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static UINT32 g_NumArr[] = {2, 4, 7, 8, 10, 16, 20, 22, 24, 30, 37, 38, 39};
/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void Search_Binary_Case(void *Para);

static UINT32 Search_Compare(void *pArray, void *pItem);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
UINT32 TSearch_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Search_Binary_Case",
            Search_Binary_Case,
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
/*  Function : Search_Binary_Case                                             */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Search_Binary_Case(void *Para)
{
    UINT32 Item;
    void *pItem = (void *)&Item;
    UINT32 MidPos;
    UINT32 index;
    UINT32 *pArray[100];
    UINT32 ArrayLen = sizeof(g_NumArr) / sizeof(g_NumArr[0]);
    
    for (index = 0; index < ArrayLen; ++index)
    {
        pArray[index] = &g_NumArr[index];
    }

    for (index = 0; index < ArrayLen; ++index)
    {
        Item = g_NumArr[index];
        MidPos = Search_Binary(pArray, 
                               pItem, 
                               ArrayLen, 
                               Search_Compare);
        TD_ASSERT_EQUAL(MidPos, index);
    }
    
    Item = 1;
    MidPos = Search_Binary(pArray, 
                           pItem, 
                           ArrayLen, 
                           Search_Compare);
    TD_ASSERT_EQUAL(MidPos, 0);

    Item = 21;
    MidPos = Search_Binary(pArray, 
                           pItem, 
                           ArrayLen, 
                           Search_Compare);
    TD_ASSERT_EQUAL(MidPos, 7);    

    Item = 35;
    MidPos = Search_Binary(pArray, 
                           pItem, 
                           ArrayLen, 
                           Search_Compare);
    TD_ASSERT_EQUAL(MidPos, 10);

    Item = 38;
    MidPos = Search_Binary(pArray, 
                           pItem, 
                           ArrayLen, 
                           Search_Compare);
    TD_ASSERT_EQUAL(MidPos, 11);
}

/*============================================================================*/
/*  Function : Search_Compare                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 Search_Compare(void *pArray, void *pItem)
{
    UINT32 *pMid = (UINT32 *)pArray;
    UINT32 *pFind = (UINT32 *)pItem;

    if (*pMid > *pFind)
    {
        return COMPARE_MORE;
    }
    else if (*pMid < *pFind)
    {
        return COMPARE_LESS;
    }
    else
    {
        return COMPARE_EQUAL;
    }
}

