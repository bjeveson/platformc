/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Stack.c                                                        */
/*  Descript : Link Stack                                                     */
/*  API Pref : Stack_                                                         */
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

#include "Arch.h"
#include "VarType.h"
#include "ErrorCode.h"
#include "Stack.h"
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
/*  Function : Stack *Stack_Init(void)                                        */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
Stack *Stack_Init(void)
{
    Stack *ps = (Stack *)MemPool_Malloc(sizeof(Stack));
    
    if (ps != NULL)                                
    {
        ps->top = NULL;
        ps->size = 0;
    }
    
    return ps;                                  
}                                               

/*============================================================================*/
/*  Function : Stack_IsEmpty                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Stack_IsEmpty(Stack *ps)
{
    if (ps->top == NULL && ps->size == 0)
    {
        return STACK_EMPTY;
    }
    else
    {
        return STACK_NOT_EMPTY;
    }
}

/*============================================================================*/
/*  Function : Stack_GetSize                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Stack_GetSize(Stack *ps)
{
    return ps->size;
}

/*============================================================================*/
/*  Function : Stack_Push                                                     */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
PSNode Stack_Push(Stack *ps, void *Data)
{   
    PSNode Node = (PSNode)MemPool_Malloc(sizeof(SNode));

    if (NULL != Node)
    {
        Node->Data = Data;
        Node->down = Stack_GetTop(ps, NULL);
        ps->size++;
        ps->top = Node;

        return Node;
    }
    else
    {
        return NULL;
    }
}

/*============================================================================*/
/*  Function : Stack_GetTop                                                   */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
PSNode Stack_GetTop(Stack *ps, void *Data)
{
    if (Stack_IsEmpty(ps) != STACK_EMPTY && Data != NULL)
    {
        Data = ps->top->Data;
    }

    return ps->top;
}

/*============================================================================*/
/*  Function : Stack_Pop                                                      */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
PSNode Stack_Pop(Stack *ps, void **Data)
{
    SNode *Node = ps->top;

    if (Stack_IsEmpty(ps) != STACK_EMPTY && Node != NULL)
    {
        *Data = Node->Data;
        ps->size--;
        ps->top = ps->top->down;
        MemPool_Free(Node);
    }

    return ps->top;
}

/*============================================================================*/
/*  Function : Stack_Destroy                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Stack_Destroy(Stack **ps)
{
    if (Stack_IsEmpty(*ps) != STACK_EMPTY)
    {
        Stack_Clear(*ps);
    }

    MemPool_Free(*ps);

    *ps = NULL;

    return;
}

/*============================================================================*/
/*  Function : Stack_Clear                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Stack_Clear(Stack *ps)
{
    while (Stack_IsEmpty(ps) != STACK_EMPTY)
    {
        Stack_Pop(ps, NULL);
    }

    return;
}

/*============================================================================*/
/*  Function : Stack_Traverse                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.6                                                       */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Stack_Traverse(Stack *ps, Stack_PrintData visit)
{
    PSNode Node = ps->top;
    UINT32 i = ps->size;

    while (i--)
    {
        visit(Node->Data);
        Node = Node->down;
    }

    return;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


