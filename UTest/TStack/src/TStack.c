/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TStack.c                                                       */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
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
#include "Stack.h"
#include "TStack.h"
#include "MemoryPool.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static Stack *ps = NULL;

static UINT32 ulAllChNum = 26;

static UINT32 ulNoneChNum = 0;
/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void Stack_Init_Case(void *Para);

static void Stack_Destroy_Case(void *Para);

static void Stack_Push_Case(void *Para);

static void Stack_Pop_Case(void *Para);

static void Stack_Traverse_Case(void *Para);

static void Print_StackData(void *Data);

static void Stack_GetSize_Case(void *Para);
/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : TStack_AddTestCase                                             */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TStack_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "Stack_Init_Case",
            Stack_Init_Case,
            NULL
        },  
        {
            "Stack_Push_Case",
            Stack_Push_Case,
            NULL
        },       
        {
            "Stack_Traverse_Case",
            Stack_Traverse_Case,
            NULL
        },    
        {
            "Stack_GetSize_Case",
            Stack_GetSize_Case,
            &ulAllChNum
        },         
        {
            "Stack_Pop_Case",
            Stack_Pop_Case,
            NULL
        },
        {
            "Stack_GetSize_Case",
            Stack_GetSize_Case,
            &ulNoneChNum
        },  
        {
            "Stack_Destroy_Case",
            Stack_Destroy_Case,
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
/*  Function : Stack_Init_Case                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Stack_Init_Case(void *Para)
{
    ps = Stack_Init();
    TD_ASSERT_NOT_EQUAL(ps, NULL);
    
    return;
}

/*============================================================================*/
/*  Function : Stack_Destroy_Case                                             */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Stack_Destroy_Case(void *Para)
{
    Stack_Destroy(&ps);
    TD_ASSERT_EQUAL(ps, NULL);
    
    return;
}

/*============================================================================*/
/*  Function : Stack_Push_Case                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Stack_Push_Case(void *Para)
{
    UINT8 ch;
    UINT8 *pCharBuff = NULL;
    
    for (ch = 'A'; ch <= 'Z'; ch++)
    {
        pCharBuff = (UINT8 *)MemPool_Malloc(sizeof(UINT8));
        *pCharBuff = ch;
        Stack_Push(ps, (void *)pCharBuff);
    }

    return;
}

/*============================================================================*/
/*  Function : Stack_Pop_Case                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Stack_Pop_Case(void *Para)
{
    UINT8 ch;
    UINT8 chIndex = 'Z';
    UINT8 *PData = NULL;

    while (Stack_IsEmpty(ps) != STACK_EMPTY)
    {
        Stack_Pop(ps, (void **)&PData);
        ch = *PData;
        TD_ASSERT_EQUAL(ch, chIndex);
        chIndex--;
    }
    
    return;
}

/*============================================================================*/
/*  Function : Print_StackData                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Print_StackData(void *Data)
{
    printf("%c\n", *(UINT8 *)Data);

    return;
}

/*============================================================================*/
/*  Function : Stack_Traverse_Case                                            */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Stack_Traverse_Case(void *Para)
{
    Stack_Traverse(ps, Print_StackData);
    
    return;
}

/*============================================================================*/
/*  Function : Stack_GetSize_Case                                             */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.7                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void Stack_GetSize_Case(void *Para)
{
    TD_ASSERT_EQUAL(Stack_GetSize(ps), *(UINT32 *)Para);
    
    return;
}
