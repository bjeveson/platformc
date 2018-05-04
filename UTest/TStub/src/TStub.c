/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Stub_TestCase.c                                                */
/*  Descript : Test Stub Module                                               */
/*  API Pref : TStub                                                          */
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
#include <stdlib.h>

#include "Arch.h"
#include "VarType.h"
#include "ErrorCode.h"
#include "stub.h"
#include "TestDrv.h"
#include "Log.h"
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
static UINT32 stubAdd(UINT32 ulLeft, UINT32 ulRight);

static UINT32 myAdd(UINT32 ulLeft, UINT32 ulRight);

static void Add_Case(void *Para);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : TStub_AddTestCase                                              */
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
UINT32 TStub_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Add_Case",
            Add_Case,
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
/*  Function : AddCase                                                        */
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
static void Add_Case(void *Para)
{
    UINT32 ulResult;
    UINT32 ulRet = LIB_ERR_SUCCESS;
    
    ulResult = myAdd(1, 2);
    TD_ASSERT_EQUAL(ulResult, 3);
    
    ulRet = Stub_Install(myAdd, stubAdd);
    TD_ASSERT_EQUAL(ulRet, LIB_ERR_SUCCESS);

    ulResult = myAdd(1, 2);
    TD_ASSERT_EQUAL(ulResult, 103);

    ulRet = Stub_Remove(myAdd, stubAdd);
    TD_ASSERT_EQUAL(ulRet, LIB_ERR_SUCCESS);

    ulResult = myAdd(1, 2);
    TD_ASSERT_EQUAL(ulResult, 3);

    return;    
}

/*============================================================================*/
/*  Function : stubAdd                                                        */
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
static UINT32 stubAdd(UINT32 ulLeft, UINT32 ulRight)
{
    UINT32 ulResult = ulLeft + ulRight;
    
    return ulResult + 100;
}

/*============================================================================*/
/*  Function : myAdd                                                          */
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
static UINT32 myAdd(UINT32 ulLeft, UINT32 ulRight)
{
    return ulLeft + ulRight;
}

