/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TString.c                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
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
#include "Shell.h"
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
static void Str_MaxSameSubStr_Case(void *Para);

static void Str_MaxSameCharSubStr_Case(void *Para);

static void Str_IndexOfSubstr_Case(void *Para);

static void Str_RotateShiftLeft_Case(void *Para);

static void Str_RotateShiftRight_Case(void *Para);

static void Str_CmdToVector_Case(void *Para);
/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : TString_AddTestCase                                            */
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
UINT32 TString_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Str_MaxSameSubStr_Case",
            Str_MaxSameSubStr_Case,
            NULL
        },
        {
            "Str_MaxSameCharSubStr_Case",
            Str_MaxSameCharSubStr_Case,
            NULL
        },        
        {
            "Str_IndexOfSubstr_Case",
            Str_IndexOfSubstr_Case,
            NULL
        },        
        {
            "Str_RotateShiftLeft_Case",
            Str_RotateShiftLeft_Case,
            NULL
        },       
        {
            "Str_RotateShiftRight_Case",
            Str_RotateShiftRight_Case,
            NULL
        },       
        {
            "Str_CmdToVector_Case",
            Str_CmdToVector_Case,
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
/*  Function : Str_MaxSameSubStr_Case                                         */
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
static void Str_MaxSameSubStr_Case(void *Para)
{
    UINT8 *pucStr1 = "Hello World!\n";
    UINT8 *pucStr2 = "Hello MyLib!\n";
    UINT8 *pucSameStr = "Hello ";
    UINT8 ucSubStr[128] = {0};
    UINT32 ulSameResult;
    
    Str_MaxSameSubStr(pucStr1, pucStr2, ucSubStr);
    ulSameResult = strcmp(ucSubStr, pucSameStr);
    TD_ASSERT_EQUAL(ulSameResult, 0);
    
    return;
}

/*============================================================================*/
/*  Function : Str_MaxSameCharSubStr_Case                                     */
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
static void Str_MaxSameCharSubStr_Case(void *Para)
{
    UINT8 *pucStr1 = "Hello World!\n";
    UINT8 *pucStr2 = "Hello MyLib!\n";
    UINT8 *pucSameStr = "ll";
    UINT8 ucSubStr[128] = {0};
    UINT32 ulSameResult;

    Str_MaxSameCharSubStr(pucStr1, ucSubStr);
    ulSameResult = strcmp(ucSubStr, pucSameStr);
    TD_ASSERT_EQUAL(ulSameResult, 0);
    
    return;
}

/*============================================================================*/
/*  Function : Str_IndexOfSubstr_Case                                         */
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
static void Str_IndexOfSubstr_Case(void *Para)
{
    UINT8 *pucStr1 = "Hello World!";
    UINT8 *pucSubStr1 = "Wo";
    UINT8 *pucStr2 = "Hello Cruel World!";
    UINT8 *pucSubStr2 = "uel";    
    UINT32 Index = 0;

    Str_IndexOfSubstr(pucStr1, pucSubStr1, &Index);
    TD_ASSERT_EQUAL(Index, 6);

    Str_IndexOfSubstr(pucStr1, "Hel", &Index);
    TD_ASSERT_EQUAL(Index, 0);

    Str_IndexOfSubstr(pucStr1, "\0", &Index);
    TD_ASSERT_EQUAL(Index, 0);
    
    Str_IndexOfSubstr(pucStr2, pucSubStr2, &Index);
    TD_ASSERT_EQUAL(Index, 8);
    
    return;
}

/*============================================================================*/
/*  Function : Str_RotateShiftLeft_Case                                       */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Str_RotateShiftLeft_Case(void *Para)
{
    const UINT8 *pucStr = "abcdefghijklmnopqrstuvwxyz";
    UINT8 ucResult[30];
    UINT32 ulCmp;
    
    Str_RotateShiftLeft(pucStr, 5, ucResult);
    ulCmp = strcmp(ucResult, "fghijklmnopqrstuvwxyzabcde");
    TD_ASSERT_EQUAL(ulCmp, 0);

    return;
}

/*============================================================================*/
/*  Function : Str_RotateShiftRight_Case                                      */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Str_RotateShiftRight_Case(void *Para)
{
    const UINT8 *pucStr = "abcdefghijklmnopqrstuvwxyz";
    UINT8 ucResult[30];
    UINT32 ulCmp;
    
    Str_RotateShiftRight(pucStr, 5, ucResult);
    ulCmp = strcmp(ucResult, "vwxyzabcdefghijklmnopqrstu");
    TD_ASSERT_EQUAL(ulCmp, 0);

    return;
}

/*============================================================================*/
/*  Function : Str_CmdToVector_Case                                           */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.23                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Str_CmdToVector_Case(void *Para)
{
    UINT8 *pucCmdLine = "Hello The Cruel World";
    UINT8 ParaStr[SHELL_CMD_PARA_MAX_NUM][SHELL_CMD_STR_MAX_LEN];
    UINT8 *pPara[SHELL_CMD_PARA_MAX_NUM];
    UINT8 ucParaNum = 0;
    UINT32 VectorIndex = 0;
    UINT32 StrCmpResult;

    for (VectorIndex = 0; VectorIndex < SHELL_CMD_PARA_MAX_NUM; VectorIndex++)
    {
        pPara[VectorIndex] = ParaStr[VectorIndex];
    }
    
    ucParaNum = Str_CmdToVector(pucCmdLine, SHELL_CMD_PARA_MAX_NUM, pPara);

    TD_ASSERT_EQUAL(ucParaNum, 4);
    
    StrCmpResult = strcmp(pPara[0], "Hello");
    TD_ASSERT_EQUAL(StrCmpResult, 0);

    StrCmpResult = strcmp(pPara[1], "The");
    TD_ASSERT_EQUAL(StrCmpResult, 0);

    StrCmpResult = strcmp(pPara[2], "Cruel");
    TD_ASSERT_EQUAL(StrCmpResult, 0);

    StrCmpResult = strcmp(pPara[3], "World");
    TD_ASSERT_EQUAL(StrCmpResult, 0);

    pucCmdLine = (UINT8 *)"help";
    ucParaNum = Str_CmdToVector(pucCmdLine, SHELL_CMD_PARA_MAX_NUM, pPara);
    TD_ASSERT_EQUAL(ucParaNum, 1);
    StrCmpResult = strcmp(pPara[0], "help");
    TD_ASSERT_EQUAL(StrCmpResult, 0);

    pucCmdLine = (UINT8 *)"\n";
    ucParaNum = Str_CmdToVector(pucCmdLine, SHELL_CMD_PARA_MAX_NUM, pPara);
    TD_ASSERT_EQUAL(ucParaNum, 0);
    
    return;
}

