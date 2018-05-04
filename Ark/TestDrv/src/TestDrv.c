/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TestDrv.c                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <string.h>

#include "Arch.h"
#include "VarType.h"
#include "ErrorCode.h"
#include "TestDrv.h"
#include "Log.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static TTestCase g_TestCaseArray[TD_TESTCASE_MAX_NUM];

static UINT32 g_TestCaseCurrNum = 0;

static UINT32 g_TestCasePassNum = 0;

static UINT32 g_TestCaseFailedNum = 0;
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
/*  Function : TD_AddTestCase                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TD_AddTestCase(TTestCase *pTestCase)
{
    TTestCase *pTmpCase = NULL;
    UINT32 ulRet = LIB_ERR_SUCCESS;

    if (g_TestCaseCurrNum > TD_TESTCASE_MAX_NUM)
    {
        return LIB_ERR_ARRAY_OUT;
    }
    
    if (0 == g_TestCaseCurrNum)
    {
        memset((void *)g_TestCaseArray, 
               0, 
               sizeof(TTestCase) * TD_TESTCASE_MAX_NUM);
    }

    for (pTmpCase = pTestCase;
         NULL != pTmpCase->m_pFuncTestCase;
         pTmpCase++, g_TestCaseCurrNum++)
    {
        memcpy((void *)g_TestCaseArray[g_TestCaseCurrNum].m_ucTestCaseDesc, 
               (void *)pTmpCase->m_ucTestCaseDesc, 
               TD_TESTCASE_DESC_LEN);
        g_TestCaseArray[g_TestCaseCurrNum].m_pFuncTestCase = pTmpCase->m_pFuncTestCase;
        g_TestCaseArray[g_TestCaseCurrNum].Para = pTmpCase->Para;
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : TD_RunAllTestCase                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void TD_RunAllTestCase(void)
{
    UINT32 ulIndex = 0;
    UINT8 *pTitle = "\nCase Test Running->>>>>>>\n";
    UINT32 printwidth = 30;
    
    printf("%.*s", printwidth, pTitle);
    
    LOG_PRINT(LOG_LEVEL_INFO, "Test Case Run Begin!\n");

    for (ulIndex = 0; ulIndex < g_TestCaseCurrNum; ++ulIndex)
    {
        LOG_PRINT(LOG_LEVEL_INFO, 
                  "Case:%s\n",
                  g_TestCaseArray[ulIndex].m_ucTestCaseDesc);
        g_TestCaseArray[ulIndex].m_pFuncTestCase(g_TestCaseArray[ulIndex].Para);
        LOG_PRINT(LOG_LEVEL_ERROR, "-------------------------------------------\n");
    }
    
    LOG_PRINT(LOG_LEVEL_INFO, "\n");    
    LOG_PRINT(LOG_LEVEL_INFO, "Test Case Run End!\n");
    LOG_PRINT(LOG_LEVEL_INFO,\
              "\n               Total Case    :%d\n\
               Total Asserts :%d\n\
               Asserts Passed:%d\n\
               Asserts Failed:%d\n", 
              g_TestCaseCurrNum,
              g_TestCasePassNum + g_TestCaseFailedNum,
              g_TestCasePassNum, 
              g_TestCaseFailedNum);

    return;
}

/*============================================================================*/
/*  Function : TD_TestCasePass                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Passed Case Count                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void TD_TestCasePass(void)
{
    g_TestCasePassNum++;

    return;
}

/*============================================================================*/
/*  Function : TD_TestCaseFailed                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Failed Case Count                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void TD_TestCaseFailed(void)
{
    g_TestCaseFailedNum++;

    return;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

