/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Search.c                                                       */
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
#include "ErrorCode.h"

#include "Arch.h"
#include "VarType.h"

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
/*  Function : Search_Binary                                                  */
/*  Input    : %1 -- Pointer Array Which Point To The Data Array              */
/*             %2 -- Data To Search                                           */
/*             %3 -- Data Array Length                                        */
/*             %4 -- Compare Function Call Back                               */
/*  Output   :                                                                */
/*  Return   : The Index Of Array                                             */
/*  Desc     : less or equal the frst element , return 0                      */
/*             more than the last element return arraylen                     */
/*             equal the last element return arraylen - 1                     */
/*             equal the element return index of element in array             */
/*             more than the index element and less than the index + 1 element*/
/*                                      return index + 1                      */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Search_Binary(void *pArray[], void *pItem, UINT32 ArrayLen, Compare fp)
{
    UINT32 High, Low, Mid;

    High = ArrayLen - 1;
    Low = 0;
    Mid = (High + Low) / 2;

    if (COMPARE_MORE == fp(pArray[0], pItem)
        || COMPARE_EQUAL == fp(pArray[0], pItem))
    {
        return 0;
    }

    if (COMPARE_LESS == fp(pArray[ArrayLen - 1], pItem))
    {
        return ArrayLen;
    }

    if (COMPARE_EQUAL == fp(pArray[ArrayLen - 1], pItem))
    {
        return ArrayLen - 1;
    }
    
    while (Low <= High)
    {
        if (COMPARE_MORE == fp(pArray[Mid], pItem))
        {
            High = Mid;
            Mid = (High + Low) / 2;
        }
        else if (COMPARE_LESS == fp(pArray[Mid], pItem))
        {
            Low = Mid;
            Mid = (High + Low) / 2;
        }
        else
        {
            return Mid;
        }

        if (COMPARE_MORE == fp(pArray[Mid + 1], pItem)
            && COMPARE_LESS == fp(pArray[Mid], pItem))
        {
            return Mid + 1;
        }
    }

	return -1;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


