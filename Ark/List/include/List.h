/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : List.h                                                         */
/*  Descript : Two-way circular linked list                                   */
/*  API Pref : List_                                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.29                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _LIST_H
#define _LIST_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define LIST_IS_EMPTY       1
#define LIST_NOT_EMPTY      0

#define LIST_FIND_OK        1
#define LIST_FIND_FAILED    0

#define LIST_DATA_EQUAL     1
#define LIST_DATA_NOT_EQUAL 0

#define LIST_INSERT_ATNEXT  1
#define LIST_INSERT_ATPRIOR 0

#define LIST_HAVE_LOOP      0
#define LIST_HAVE_NO_LOOP   1
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef struct LNode
{
     void *Data;
     struct LNode *Prior;
     struct LNode *Next;
}LNode, *PLNode;

typedef UINT32 (*List_Compare)(void *data, void *nextdata);

typedef void (*List_PrintData)(void *Data);
/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

UINT32 List_Init(PLNode *list);

void List_Destroy(PLNode list);

void List_Clear(PLNode list);

UINT32 List_IsEmpty(PLNode list);

UINT32 List_Length(PLNode list);

UINT32 List_GetDataByPosition(PLNode list, UINT32 n, void **e);

UINT32 List_GetNodeIndexByData(PLNode list, void *e, List_Compare fp_Compare);

UINT32 List_FindPriorNodeByData(PLNode list, 
                                void *cur_e, 
                                List_Compare fp_Compare, 
                                void **pre_e);

UINT32 List_FindNextNodeByData(PLNode list, 
                               void *cur_e,
                               List_Compare fp_Compare,
                               void **next_e);

UINT32 List_GetNodeAddrByPosition(PLNode list, UINT32 n, PLNode *node);

UINT32 List_InsertNodeByPosition(PLNode list, INT32 i, void *e);

UINT32 List_DeleteNodeByPosition(PLNode list, UINT32 i);

UINT32 List_Traverse(PLNode list, List_PrintData fp_PrintData);

void List_TraverseBack(PLNode list, List_PrintData fp_PrintData);

PLNode List_InsertNodeAtTail(PLNode list, void *e);

UINT32 List_DelNodeAtTail(PLNode list, void **e);

PLNode List_Unit(PLNode list1, PLNode list2);

void *List_ToArray(PLNode list);

UINT32 List_FindCurNodeByData(PLNode list, 
                            void *Cur_Key,
                            void **Cur_e, 
                            List_Compare fp_Compare);

void *List_GetMidNodeData(PLNode list);

UINT32 List_IsLoop(PLNode list);

#endif


