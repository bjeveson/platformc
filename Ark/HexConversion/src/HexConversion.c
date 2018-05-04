/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : HexConversion.c                                                */
/*  Descript : Hex Conversion                                                 */
/*  API Pref : Hexcon_                                                        */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>

#include "Arch.h"
#include "VarType.h"
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

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Hexcon_StringToDec                                             */
/*  Input    : pdec --Pointer Of String                                       */
/*  Output   :                                                                */
/*  Return   : Decimal System Num                                             */
/*  Desc     : String Conversion To Decimal System Num                        */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Hexcon_StringToDec(const UINT8 *pdec)
{ 
    UINT32 i = 0;
    UINT32 num = 0; 

    if (NULL == pdec)
    {
        return 0;
    }
    
    while (pdec[i]) 
    { 
        num *= 10; 
        
        if ((pdec[i] >= '0') && (pdec[i] <= '9'))
        {
            num += pdec[i] - '0'; 
        }
        i++;
    }
    
    return num; 
}

/*============================================================================*/
/*  Function : Hexcon_DecToAny                                                */
/*  Input    : %1 -- dec Num                                                  */
/*             %2 -- Change To Num                                            */
/*  Output   : %3 -- Store Result                                             */
/*  Return   : Error Code                                                     */
/*  Desc     : Conversion decimal number to any number less 10                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 *Hexcon_DecToAnyIn10(UINT32 dec, UINT32 bottom, UINT8 *any)
{
	UINT32 i, bit = 0, oldnum = dec;
	UINT8 c;

	if (bottom >= 10)
	{
		any = NULL;
	}
	else
	{
		do
		{
			bit++;
		}while (dec /= bottom);	

		i = 0;
		do 
		{
			any[i] = oldnum % bottom + '0';
			i++;
		}while (oldnum /= bottom);

		any[i] = '\0';
		bit = bit - 1;

		for (i = 0; i < bit; i++, bit--)
		{
			c = any[i];
			any[i] = any[bit];
			any[bit] = c;
		}
	}

	return any;
}

/*============================================================================*/
/*  Function : Hexcon_DecToBinDisplay                                         */
/*  Input    : %1 -- dec num                                                  */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Display Binary Num                                             */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Hexcon_DecToBinDisplay(UINT32 x)
{
	UINT32 i;
	
	for (i = 0; i < 8; i++)
	{
		printf("%d", x >> (7 - i) & 0x01);
    }
    
	printf("\n");
}

/*============================================================================*/
/*  Function : Hexcon_DecToHex                                                */
/*  Input    : %1 -- dec num                                                  */
/*             %2 -- num length                                               */
/*  Output   : %3 -- hex                                                      */
/*  Return   : Error Code                                                     */
/*  Desc     : dec to hex                                                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 *Hexcon_DecToHex(UINT32 dec, UINT32 len, UINT8 *hex) 
{    
   UINT8 ch, *left = hex, *right = hex, num[]="0123456789ABCDEF"; 
   UINT32 i = 0;

   if (hex == NULL)
   {
       return NULL;
   }

   for (i = 0;i < len;i++)
   {
       *right++=num[dec & 15]; 
       dec >>= 4;
   }
   *right-- = '\0'; 

   while (left < right) 
   { 
       ch = *left; 
       *left++ = *right; 
       *right-- = ch; 
   } 
   
   return hex; 
}                    

/*============================================================================*/
/*  Function : Hexcon_HexToDec                                                */
/*  Input    : %1 -- string of hex                                            */
/*  Output   :                                                                */
/*  Return   : dec num                                                        */
/*  Desc     : Conversion hex number to decimal number                        */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Hexcon_HexToDec(const UINT8 *hex) 
{ 
   UINT32 i=0, num=0; 

   if (hex == NULL)
   {
       return 0;
   }
   
   while (hex[i]) 
   { 
      num <<= 4; 
      if (hex[i] >= '0' && hex[i] <= '9') 
      {
         num += hex[i] - '0'; 
      }
      else if (hex[i] >= 'A' && hex[i] <= 'F') 
      {
         num += hex[i] - 55; 
      }
      else if (hex[i] >= 'a' && hex[i] <= 'f') 
      {
         num += hex[i] - 87;
      }
      else
      {
        num = 0;
      }
         
      i++; 
   } 
   
   return num; 
}

/*============================================================================*/
/*  Function : Hexcon_DecNumToStr                                             */
/*  Input    : %1 -- DEC NUM Array                                            */
/*             %2 -- Array Element Count                                      */
/*  Output   : %3 -- dest string                                              */
/*  Return   : Error Code                                                     */
/*  Desc     : Conversion dec number to number string,And Blank Separation    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 *Hexcon_DecNumFormateToStr(UINT32 Num[], UINT32 NumCount, UINT8 DesStr[])
{
	UINT32 ch[32];
	INT32 i = 0, j = 0, k = 0, index = 0;

	for (index = 0; index < (INT32)NumCount; index++)
	{
	    i = 0;
    	do
    	{
    		ch[i++] = Num[index] % 10 + '0';
    	}while (Num[index] /= 10);

    	k = i - 1;

    	for (; k >= 0; j++, k--)
    	{
    		DesStr[j] = ch[k];
        }
        
    	DesStr[j++] = ' ';
    }

    DesStr[j - 1] = '\0';
    
	return DesStr;
}

/*============================================================================*/
/*  Function : Hexcon_DecStrFormateToNum                                      */
/*  Input    : %1 -- Des String One Blank Separation                          */
/*  Output   : %2 -- Num Array                                                */
/*  Return   : Array Element Count                                            */
/*  Desc     : Dec String To Dec Num                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.19                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Hexcon_DecStrFormateToNum(const UINT8 *Str, UINT32 Num[])
{
    UINT8 tmp[32];
    UINT32 j = 0, i = 0, index = 0;
    
    for (i = 0; Str[i] != '\0'; ++i)
    {
        if (' ' != Str[i])
        {
            tmp[j++] = Str[i];
        }
        else
        {
            tmp[j] = '\0';
            j = 0;
            Num[index++] = atoi(tmp);
        }
    }

    tmp[j] = '\0';
    Num[index++] = atoi(tmp);
    
    return index;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

