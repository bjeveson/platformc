/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Sort.c                                                         */
/*  Descript : Common Sort                                                    */
/*  API Pref : Sort_                                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.31                                                      */
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
#include "Sort.h"
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
/*  Function : Sort_Bubble                                                    */
/*  Input    : %1 -- Pointer Array,Store Pointer                              */
/*             %2 -- Array Elemtype Num                                       */
/*             %3 -- Compare Call Back Function                               */
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
void Sort_Bubble(void *Array[], UINT32 ArrayLen, Compare fp)
{
	UINT32 i, j;
	void *tmp;

	for (i = 1; i < ArrayLen; i++)
	{
		for (j = 0; j < ArrayLen - i; j++)
		{
			if (COMPARE_MORE == fp(Array[j], Array[j + 1]))
			{
				tmp = Array[j];
			    Array[j] = Array[j + 1];
				Array[j + 1] = tmp;
			}
		}
	}

	return;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


