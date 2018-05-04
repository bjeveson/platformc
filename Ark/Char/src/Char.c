/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Char.c                                                         */
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
#include "Arch.h"
#include "VarType.h"

#include "Char.h"
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
/*  Function : Char_IsDigit                                                   */
/*  Input    : %1 -- Char                                                     */
/*  Output   :                                                                */
/*  Return   : CHAR_ISNOT_DIGIT --is not number of 0-9                        */
/*             CHAR_IS_DIGIT     --is number of 0-9                           */
/*  Desc     : Judge the char is digit type or not                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Char_IsDigit(UINT8 c)
{
	if ((c >= '0' ) && (c <= '9' ))
	{
		return CHAR_IS_DIGIT;
    }
	else 
    {
	    return CHAR_ISNOT_DIGIT;
	}
}

/*============================================================================*/
/*  Function : Char_IsSpace                                                   */
/*  Input    : %1 -- Char                                                     */
/*  Output   :                                                                */
/*  Return   : CHAR_ISNOT_SPACE -- Is not a space                             */
/*             CHAR_IS_SPACE     -- Is space                                  */
/*  Desc     : Judge the char is space ir not                                 */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Char_IsSpace(UINT8 c)
{
	if (c == ' ')
	{
		return CHAR_IS_SPACE;
    }
	else
	{
	    return CHAR_ISNOT_SPACE;
    }
}

/*============================================================================*/
/*  Function : Char_OfIndex                                                   */
/*  Input    : %1 -- the long string                                          */
/*             %2 -- the char needed to find out                              */
/*  Output   :                                                                */
/*  Return   : 0   --the char dos not exist in string                         */
/*             >=1 --the index of char in string                              */
/*  Desc     : Find out the ch in string,and return the index of First ch     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Char_OfIndex(UINT8 *str, UINT8 ch)
{
	UINT8 *p = str;
	UINT32 index = 0;

	while (*p != '\0') 
	{
		if (*p != ch) 
		{
			index++;
			p++;
		}
		else
		{
		    break;
		}
	}
    
	return index;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


