/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TList.c                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
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
#include "List.h"
#include "TList.h"
#include "MemoryPool.h"

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static PLNode list = NULL;

static TNodeData NodeData[] = {
                                  {
                                      1,
                                      'A',
                                      0
                                  },
                                  {
                                      2,
                                      'B',
                                      0
                                  },
                                  {
                                      3,
                                      'C',
                                      0
                                  },
                                  {
                                      4,
                                      'D',
                                      0
                                  },
                                  {
                                      5,
                                      'E',
                                      LIB_ERR_SUCCESS
                                  },
                                  {
                                      6,
                                      'F',
                                      0
                                  },
                                  {
                                      7,
                                      'G',
                                      0
                                  },
                                  {
                                      8,
                                      'H',
                                      0
                                  },
                                  {
                                      9,
                                      'I',
                                      0
                                  },
                                  {
                                      10,
                                      'J',
                                      0
                                  }
                              };
/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void List_Init_Case(void *Para);

static void List_IsEmpty_Case(void *Para);

static void List_InsertNodeByPosition_Case(void *Para);

static void PrinData(void *pData);

static void List_Destroy_Case(void *Para);

static void List_Length_Case(void *Para);

static void List_DeleteNodeByPosition_Case(void *Para);

static UINT32 CompareData(void *cur, void *next);

static void List_FindNextNodeByData_Case(void *Para);

static void List_Traverse_Case(void *Para);

static void List_GetDataByPosition_Case(void *Para);

static void List_FindCurNodeData_Case(void *Para);

static UINT32 ChCompare(void *Node, void *pucKey);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : TList_AddTestCase                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TList_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "List_Init_Case",
            List_Init_Case,
            NULL
        },   
        {
            "List_IsEmpty_Case",
            List_IsEmpty_Case,
            NULL
        },    
        {
            "List_InsertNodeByPosition_Case",
            List_InsertNodeByPosition_Case,
            &NodeData[9]
        },    
        {
            "List_Length_Case",
            List_Length_Case,
            &NodeData[9]
        },    
        {
            "List_DeleteNodeByPosition_Case",
            List_DeleteNodeByPosition_Case,
            &NodeData[4]
        },
        {
            "List_Length_Case",
            List_Length_Case,
            &NodeData[8]
        },    
        {
            "List_Traverse_Case",
            List_Traverse_Case,
            NULL
        },           
        {
            "List_FindNextNodeByData_Case",
            List_FindNextNodeByData_Case,
            &NodeData[2]
        },
        {
            "List_GetDataByPosition_Case",
            List_GetDataByPosition_Case,
            &NodeData[2]
        },
        {
            "List_FindCurNodeData_Case",
            List_FindCurNodeData_Case,
            &NodeData[2]
        },                
        {
            "List_Destroy_Case",
            List_Destroy_Case,
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
/*  Function : List_Init_Case                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_Init_Case(void *Para)
{
    UINT32 ulRet;
    
    ulRet = List_Init(&list);
    TD_ASSERT_EQUAL(ulRet, LIB_ERR_SUCCESS);
    TD_ASSERT_EQUAL(list->Next, list->Prior);
    TD_ASSERT_EQUAL(list->Next, list);
    TD_ASSERT_EQUAL(list->Data, NULL);
    
    return;
}

/*============================================================================*/
/*  Function : List_IsEmpty_Case                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_IsEmpty_Case(void *Para)
{
    UINT32 ulRet;
    
    ulRet = List_IsEmpty(list);
    TD_ASSERT_EQUAL(ulRet, LIST_IS_EMPTY);

    return;
}

/*============================================================================*/
/*  Function : List_InsertNodeByPosition_Case                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_InsertNodeByPosition_Case(void *Para)
{
    UINT32 ulRet;
    TNodeData *pData = NULL;
    TNodeData *pa = Para;
    UINT32 i = 0;

    for(i = 0; i < pa->Pos; i++)
    {
        pData = (TNodeData *)MemPool_Malloc(sizeof(TNodeData));
        if (NULL == pData)
        {
            return;
        }

        *pData = NodeData[i];
        
        List_InsertNodeByPosition(list, i, (void *)pData);
    }
    
    return;
}

/*============================================================================*/
/*  Function : PrinData                                                       */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void PrinData(void *pData)
{
    TNodeData *pNode = pData;
    
    if (NULL == pData)
    {
        return;
    }

    printf("%d    %c     %d\n", pNode->Pos, pNode->ch, pNode->Result);
}

/*============================================================================*/
/*  Function : List_Destroy_Case                                              */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_Destroy_Case(void *Para)
{
    List_Destroy(list);

    return;
}

/*============================================================================*/
/*  Function : List_Length_Case                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_Length_Case(void *Para)
{
    UINT32 ulListLen;
    TNodeData *pa = Para;
    
    ulListLen = List_Length(list);
    TD_ASSERT_EQUAL(ulListLen, pa->Pos);
}

/*============================================================================*/
/*  Function : List_DeleteNodeByPosition_Case                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_DeleteNodeByPosition_Case(void *Para)
{
    UINT32 ulRet;
    TNodeData *pa = Para;
    
    ulRet = List_DeleteNodeByPosition(list, pa->Pos);
    TD_ASSERT_EQUAL(ulRet, pa->Result);

    return;
}

/*============================================================================*/
/*  Function : List_GetDataByPosition_Case                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_GetDataByPosition_Case(void *Para)
{
    TNodeData *pa = Para;
    TNodeData *Pdata = NULL;
    UINT32 ulRet;
    
    ulRet = List_GetDataByPosition(list, pa->Pos, (void **)&Pdata);
    TD_ASSERT_EQUAL(ulRet, LIST_FIND_OK);
    TD_ASSERT_EQUAL(Pdata->Pos, 3);
    TD_ASSERT_EQUAL(Pdata->ch, 'C');
    TD_ASSERT_EQUAL(Pdata->Result, 0);
    
    return;
}

/*============================================================================*/
/*  Function : CompareData                                                    */
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
static UINT32 CompareData(void *cur, void *next)
{
    TNodeData *pcur_data = (TNodeData *)cur;
    TNodeData *pnext_data = (TNodeData *)next;

    if (pcur_data->ch == pnext_data->ch
        && pcur_data->Pos == pnext_data->Pos
        && pcur_data->Result == pnext_data->Result)
    {
        return LIST_DATA_EQUAL;
    }
    else
    {
        return LIST_DATA_NOT_EQUAL;
    }
}

/*============================================================================*/
/*  Function : List_FindNextNodeByData_Case                                   */
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
static void List_FindNextNodeByData_Case(void *Para)
{
    TNodeData *next_data = NULL;
    UINT32 ulRet = LIST_FIND_OK;
    
    ulRet = List_FindNextNodeByData(list, Para, CompareData, (void **)&next_data);
    TD_ASSERT_EQUAL(ulRet, LIST_FIND_OK);
    TD_ASSERT_NOT_EQUAL(next_data, NULL);
    TD_ASSERT_EQUAL(next_data->ch, 'D');
    TD_ASSERT_EQUAL(next_data->Pos, 4);
}

/*============================================================================*/
/*  Function : List_Traverse_Case                                             */
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
static void List_Traverse_Case(void *Para)
{
    return (void)List_Traverse(list, PrinData);
}

/*============================================================================*/
/*  Function : List_FindCurNodeData_Case                                      */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void List_FindCurNodeData_Case(void *Para)
{
    UINT8 pucKey = 'A';
    TNodeData *pData = NULL;
    UINT32 ulRet = LIB_ERR_SUCCESS;
    
    ulRet = List_FindCurNodeByData(list, 
                                (void *)&pucKey,
                                (void **)&pData, 
                                ChCompare);
    TD_ASSERT_EQUAL(ulRet, LIST_FIND_OK);
    TD_ASSERT_EQUAL(pData->ch, pucKey);

    pucKey = 'C';
    ulRet = List_FindCurNodeByData(list, 
                                (void *)&pucKey,
                                (void **)&pData, 
                                ChCompare);
    TD_ASSERT_EQUAL(ulRet, LIST_FIND_OK);
    TD_ASSERT_EQUAL(pData->ch, pucKey);

    pucKey = 'J';
    ulRet = List_FindCurNodeByData(list, 
                                (void *)&pucKey,
                                (void **)&pData, 
                                ChCompare);
    TD_ASSERT_EQUAL(ulRet, LIST_FIND_OK);
    TD_ASSERT_EQUAL(pData->ch, pucKey);
    
    return;
}

/*============================================================================*/
/*  Function : StringCompare                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 ChCompare(void *Node, void *pucKey)
{
    TNodeData *pNodeData = (TNodeData *)Node;
    UINT8 *pKey = (UINT8 *)pucKey;

    if (pNodeData->ch == *pKey)
    {
        return LIST_DATA_EQUAL;
    }
    else
    {
        return LIST_DATA_NOT_EQUAL;
    }
}
