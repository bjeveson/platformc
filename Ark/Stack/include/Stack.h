/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Stack.h                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _STACK_H
#define _STACK_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define STACK_EMPTY     1
#define STACK_NOT_EMPTY 0
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef struct SNode
{
     void *Data;
     struct SNode *down;
}SNode, *PSNode;

typedef struct Stack  
{
    PSNode top;
    UINT32 size;
}Stack;

typedef void (*Stack_PrintData)(void *Data);

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
Stack *Stack_Init(void);

UINT32 Stack_IsEmpty(Stack *ps);

UINT32 Stack_GetSize(Stack *ps);

PSNode Stack_Push(Stack *ps, void *Data);

PSNode Stack_GetTop(Stack *ps, void *Data);

PSNode Stack_Pop(Stack *ps, void **Data);

void Stack_Destroy(Stack **ps);

void Stack_Clear(Stack *ps);

void Stack_Traverse(Stack *ps, Stack_PrintData visit);

#endif

