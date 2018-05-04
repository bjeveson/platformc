/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : List.c                                                         */
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
#include <stdlib.h>
#include <stdio.h>

#include "ErrorCode.h"
#include "Arch.h"
#include "VarType.h"
#include "List.h"
#include "MemoryPool.h"

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

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : List_Init                                                      */
/*  Input    : --                                                             */
/*  Output   : %1 -- list pointer                                             */
/*  Return   : Error Code                                                     */
/*  Desc     : Init A list with head, head no real data                       */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.31                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_Init(PLNode *list)
{
    *list = (PLNode)MemPool_Malloc(sizeof(LNode));

    if (NULL != *list)
    {
        (*list)->Next = (*list)->Prior = *list;
        (*list)->Data = NULL;
        return LIB_ERR_SUCCESS;
    }

    return LIB_ERR_POINTER_NULL;
}

/*============================================================================*/
/*  Function : List_Destroy                                                   */
/*  Input    : %1 -- list pointer                                             */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Destroy A List                                                 */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void List_Destroy(PLNode list)
{
    PLNode q, p = list->Next; /* p point to the first node */

    while (p != list)
    {
        q = p->Next;
        MemPool_Free(p);
        p = q;
    }

    MemPool_Free(list);
    list = NULL;

    return;
}

/*============================================================================*/
/*  Function : List_Clear                                                     */
/*  Input    : %1 -- List Pointer                                             */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Clear A List                                                   */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void List_Clear(PLNode list)
{
    PLNode q, p = list->Next;
    
    while (p != list)
    {
        q = p->Next;
        MemPool_Free(p);
        p = q;
    }
    
    list->Next = list->Prior = list;
}

/*============================================================================*/
/*  Function : List_IsEmpty                                                   */
/*  Input    : %1 -- List Pointer                                             */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Judge A List Is Empty Or Not                                   */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_IsEmpty(PLNode list)
{
    if (list->Next == list && list->Prior == list)
    {
        return LIST_IS_EMPTY;
    }
    else
    {
        return LIST_NOT_EMPTY;
    }
}

/*============================================================================*/
/*  Function : List_Length                                                    */
/*  Input    : %1 -- List Pointer                                             */
/*  Output   :                                                                */
/*  Return   : Node Num of List                                               */
/*  Desc     : Get Length                                                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_Length(PLNode list)
{
    UINT32 i = 0;
    PLNode p = list->Next;

    while (p != list)
    {
        i++;
        p = p->Next;
    }

    return i;
}

/*============================================================================*/
/*  Function : List_GetNElem                                                  */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Position:1,2,3...N                                       */
/*  Output   : %3 -- The Node Data Of Position Node                           */
/*  Return   : Error Code                                                     */
/*  Desc     : Output Position Node Data Pointer                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_GetDataByPosition(PLNode list, UINT32 n, void **e)
{
    UINT32 j = 1;
    PLNode p = list->Next;

    while (p != list && j < n)
    {
        p = p->Next;
        j++;
    }
    
    if(p == list || j > n)
    {
        return LIST_FIND_FAILED;
    }

    *e = p->Data;

    return LIST_FIND_OK;
}

/*============================================================================*/
/*  Function : List_FindNodeByData                                            */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Data Pointer Of Finding                                  */
/*             %3 -- Call Back Function Of Compare Node Data                  */
/*  Output   :                                                                */
/*  Return   : Index                                                          */
/*  Desc     : Get Node Index By Data Node                                    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_GetNodeIndexByData(PLNode list, void *e, List_Compare fp_Compare)
{ 
    UINT32 i = 0;
    PLNode p = list->Next;   

    if (NULL == fp_Compare)
    {
        return LIST_FIND_FAILED;
    }

    while (p != list)
    {
        i++;
        if (LIST_DATA_EQUAL == fp_Compare(p->Data ,e))
        {
            return i;
        }
        p = p->Next;
    }
    
    return 0;
}

/*============================================================================*/
/*  Function : List_FindCurNodeData                                           */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_FindCurNodeByData(PLNode list, 
                            void *Cur_Key,
                            void **Cur_e, 
                            List_Compare fp_Compare)
{
	PLNode p = list->Next;   

    if (NULL == list)
    {
        printf("List NULL\n");
        return LIST_FIND_FAILED;
    }

    if (NULL == fp_Compare)
    {
        return LIST_FIND_FAILED;
    }

    while (p != list)
    {
        if (LIST_DATA_EQUAL == fp_Compare(p->Data, Cur_Key))
        {
            *Cur_e = p->Data;
            return LIST_FIND_OK;
        }

        p = p->Next;
    }
    
    return LIST_FIND_FAILED;
}

/*============================================================================*/
/*  Function : List_FindPriorNodeByData                                       */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Current Data Of Node                                     */
/*             %3 -- Call Back Fuction Of Compare                             */
/*  Output   : %4 -- Prior Node Pointer Of Current Node With Data             */
/*  Return   : LIST_FIND_OK/LIST_FIND_FAILED                                  */
/*  Desc     : Get Prior Node Data Pointer By Current Data                    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_FindPriorNodeByData(PLNode list, 
                                void *cur_e, 
                                List_Compare fp_Compare, 
                                void **pre_e)
{
    PLNode p = list->Next->Next;

    if (NULL == fp_Compare)
    {
        return LIST_FIND_FAILED;
    }
    
    while (p != list)
    {
        if (LIST_DATA_EQUAL == fp_Compare(p->Data, cur_e))
        {
            *pre_e = p->Prior->Data;
            return LIST_FIND_OK;
        }
        p = p->Next;
    }

    return LIST_FIND_FAILED;
}

/*============================================================================*/
/*  Function : List_FindNextNodeByData                                        */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Current Node Data Pointer                                */
/*             %3 -- Call Back Function Of Compare                            */
/*  Output   : %4 -- Next Node Data Pointer                                   */
/*  Return   : LIST_FIND_OK/LIST_FIND_FAILED                                  */
/*  Desc     : Find Next Node Data Pointer By Current Node Data               */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_FindNextNodeByData(PLNode list, 
                               void *cur_e,
                               List_Compare fp_Compare,
                               void **next_e)
{
    PLNode p = list->Next->Next;

    if (NULL == fp_Compare)
    {
        return LIST_FIND_FAILED;
    }
    
    while (p != list)   
    {
        if (LIST_DATA_EQUAL == fp_Compare(p->Prior->Data, cur_e))
        {
            *next_e = p->Data;
            return LIST_FIND_OK;
        }

        p = p->Next;
    }

    return LIST_FIND_FAILED;
}

/*============================================================================*/
/*  Function : List_GetNodeAddrByPosition                                     */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Index                                                    */
/*  Output   : %3 -- Address Of Node                                          */
/*  Return   : LIST_FIND_FAILED/LIST_FIND_OK                                  */
/*  Desc     : Get Node Address Of The Position                               */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_GetNodeAddrByPosition(PLNode list, UINT32 n, PLNode *node)
{
    UINT32 j = 1;
    PLNode p = list->Next;

    while (p != list && j < n)
    {
        p = p->Next;
        j++;
    }
    
    if(p == list || j > n)
    {
        return LIST_FIND_FAILED;
    }

    *node = p;

    return LIST_FIND_OK;
}

/*============================================================================*/
/*  Function : List_InsertByPosition                                          */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Position                                                 */
/*             %3 -- Insert Data                                              */
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
UINT32 List_InsertNodeByPosition(PLNode list, INT32 i, void *e)
{
    PLNode p = list->Next, s;
    UINT32 j = 1;

    while (p != list)
    {
        if (i == j)
        {
            break;
        }
        j++;
        p = p->Next;
    }

    if (p == list)
    {
        LIB_ERR_PARA_INVALID;
    }
    
    s = (PLNode)MemPool_Malloc(sizeof(LNode));
    if (NULL == s)
    {
        return LIB_ERR_POINTER_NULL;
    }

    s->Data = e;
    s->Prior = p;
    s->Next = p->Next;
    p->Next->Prior = s;
    p->Next = s;
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : List_DeleteNodeByPosition                                      */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Index                                                    */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Delete Node Of The Position                                    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_DeleteNodeByPosition(PLNode list, UINT32 i)
{
    PLNode p = list->Next;
    UINT32 j = 1;

    while (p != list)
    {
        if (i == j)
        {
            break;
        }
        j++;
        p = p->Next;
    }
    
    if (p == list)
    {
        LIB_ERR_PARA_INVALID;
    }

    p->Prior->Next = p->Next;
    p->Next->Prior = p->Prior;
    MemPool_Free(p);

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : List_Traverse                                                  */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Call Back Function Of Print Data                         */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Print Data Of Every Node From Head To Tail                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_Traverse(PLNode list, List_PrintData fp_PrintData)
{
    PLNode p = list->Next;
    UINT32 Count = 0;

    if (NULL == fp_PrintData)
    {
        return;
    }
    
    while (p != list)
    {
        fp_PrintData(p->Data);
        p = p->Next;
        Count++;
    }
    
    printf("\n");

    return Count;
}

/*============================================================================*/
/*  Function : List_TraverseBack                                              */
/*  Input    : %1 -- List Pointer                                             */
/*             %2 -- Call Back Function Of Print Data                         */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Print Data Of Every Node From Tail To Head                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void List_TraverseBack(PLNode list, List_PrintData fp_PrintData)
{
    PLNode p = list->Prior;

    if (NULL == fp_PrintData)
    {
        return;
    }
    
    while (p != list)
    {
        fp_PrintData(p->Data);
        p = p->Prior;
    }
    
    printf("\n");
}

/*============================================================================*/
/*  Function : List_InsertNodeAtTail                                          */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
PLNode List_InsertNodeAtTail(PLNode list, void *e)
{
    PLNode p = list->Prior, s;

    s = (PLNode)MemPool_Malloc(sizeof(LNode));
    if (NULL == s)
    {
        return NULL;
    }

    p->Next = s;
    s->Next = list;
    list->Prior= s;
    s->Prior = p;
    s->Data = e;
    
    return s;
}

/*============================================================================*/
/*  Function : List_DelNodeAtTail                                             */
/*  Input    : %1 -- list                                                     */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_DelNodeAtTail(PLNode list, void **e)
{
    PLNode p = list->Prior;

    if (p != list)
    {
        list->Prior = p->Prior;
        p->Prior->Next = list;
    }
    else
    {
        return LIB_ERR_LIST_EMPTY;
    }
    
    *e = p->Data;

    MemPool_Free(p);
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : List_Unit                                                      */
/*  Input    : %1 --  lis                                                     */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
PLNode List_Unit(PLNode list1, PLNode list2)
{
    PLNode P1Tail = list1->Prior;
    PLNode P1First = list1->Next;
    PLNode P2Tail = list1->Prior;
    PLNode P2First = list2->Next;

    P1Tail->Next = P2First;
    P2Tail->Next = P1First;
    P2First->Prior = P1Tail;
    P1First->Prior = P2Tail;
    
    return list1;
}

/*============================================================================*/
/*  Function : List_ToArray                                                   */
/*  Input    : %1 -- List                                                     */
/*  Output   :                                                                */
/*  Return   : Mid Node Data                                                  */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void *List_GetMidNodeData(PLNode list)
{
    LNode *p = list->Next;
    LNode *p2Step = list->Next;
    
    while (p != list)
    {
        p = p->Next;
        p2Step = p2Step->Next->Next;
    }
    
    return p->Data;
}

/*============================================================================*/
/*  Function : List_IsLoop                                                    */
/*  Input    : %1 -- list                                                     */
/*  Output   :                                                                */
/*  Return   : loop flag                                                      */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.10.9                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 List_IsLoop(PLNode list)
{
    LNode *p = list->Next;
    LNode *p2Step = list->Next;

    while (p != list)
    {
        p = p->Next;
        p2Step = p2Step->Next->Next;

        if (p == p2Step)
        {
            return LIST_HAVE_LOOP;
        }
    }

    return LIST_HAVE_NO_LOOP;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


