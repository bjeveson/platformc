/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : String.c                                                       */
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Arch.h"
#include "VarType.h"
#include "ErrorCode.h"
#include "Strings.h"
#include "Log.h"
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
/*  Function : Str_MaxSameSubStr                                              */
/*  Input    : %1 -- String1                                                  */
/*             %2 -- String2                                                  */
/*  Output   : %3 -- OutPut Same Max Sub-String                               */
/*  Return   : Error Code                                                     */
/*  Desc     : Accept two strings,find out the longest commonality            */
/*             sub-string of str1 and str2                                    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_MaxSameSubStr(const UINT8 *str1, const UINT8 *str2, UINT8 *pucSubStr) 
{
	UINT32 num, max, len;
	const UINT8 *p, *q, *p1, *q1;
	UINT8 *tmp;

    if (NULL == str1 || NULL == str2 || NULL == pucSubStr)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "String Pointer Is NULL\n");
        return;
    }
    
	if (strlen(str1) >= strlen(str2))
	{
		len = strlen(str1);
    }
	else
	{
	    len = strlen(str2);
    }
    
	tmp = (UINT8 *)MemPool_Malloc(sizeof(UINT8) * len + 1);
	p = str1; 
	q = str2;
	max = 0;

	while (*p != '\0' && *q != '\0') 
	{
		q1 = q; 
		p1 = str1;

		while (*p1 == *q1 && *p1 != '\0' && *q1 != '\0') 
		{
			num = 0;

			while (*p1 == *q1) 
			{
				tmp[num] = *p1;
				p1++; 
				q1++; 
				num++;

				if (num > max) 
				{
					max = num;
					memcpy(pucSubStr, tmp, max);
				}
			}
			p1 = p1 - num + 1;
			q1 = q;
		}
		p++;
		q++;
	}

	pucSubStr[max] = '\0';
	MemPool_Free(tmp);

	return;
}

/*============================================================================*/
/*  Function : Str_MaxSameCharSubStr                                          */
/*  Input    : %1 -- String                                                   */
/*  Output   : %2 -- OutPut String                                            */
/*  Return   : Error Code                                                     */
/*  Desc     : Accept one strings,find out the longest Same sub-string of str */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_MaxSameCharSubStr(const INT8 *str, UINT8 *pucOutStr)
{
	UINT32 len,max;
	const UINT8 *p,*q,*t;
	p = str;

	if (p == NULL)
	{
		return;
    }
    
	q = p; 
	max = 0;

	while (*p != '\0')
	{

		len = 0;
		t = p;

		while (*t == *p) 
		{
			p++; 
			len++;
		}

		if (len > max)
		{
			max = len;
			q = t;
		}
	}

	strcpy(pucOutStr, q);
	pucOutStr[max] = '\0';

	return;
}

/*============================================================================*/
/*  Function : Str_StrNCat                                                    */
/*  Input    : %2 -- Dest String                                              */
/*             %3 -- String Length Of %2                                      */
/*  Output   : %1 -- OutPut String                                            */
/*  Return   : void                                                           */
/*  Desc     : Copy n chars from *src to the end of *des                      */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_StrNCat(UINT8 *des, const UINT8 *src, UINT32 n) 
{
	UINT8 *p1 = des;
	const UINT8 *p2 = src;
	UINT32 i = 0;

	while (*p1 != '\0') 
	{
		p1++;
	}

	while (*p2 != '\0' && i < n) 
	{
		*p1++ = *p2++;
		i++;
	}

	*p1 = '\0';
}

/*============================================================================*/
/*  Function : Str_IndexOfSubstr                                              */
/*  Input    : %1 -- The Long String                                          */
/*             %2 -- Sub String                                               */
/*  Output   : %3 -- First Pos Of SubString                                   */
/*  Return   : Error Code                                                     */
/*  Desc     : Find out the sub-string in string,                             */
/*             and return the index of Sub-String                             */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_IndexOfSubstr(const UINT8 *str, const UINT8 *substr, UINT32 *Index)
{
	const UINT8 *p = str, *q = substr;
	UINT32 index = 0,len = 0,flag = 0;

	while (*q != '\0' && *p != '\0') 
	{
		len++;

		if(*p == *q) 
		{
			p++;
			q++;
			flag = 1;
			index++;
		} 
		else
		{
			q = substr;
			index = 0;
			flag = 0;
			p++;
		}
	}

	if (flag == 0)
	{
		*Index = 0;
    }
	else
	{
		*Index =  len - index;
    }

    return;
}

/*============================================================================*/
/*  Function : Str_StrCpy                                                     */
/*  Input    : %1 -- Dest String                                              */
/*             %2 -- Src String                                               */
/*  Output   :                                                                */
/*  Return   : Dest String Pointer                                            */
/*  Desc     : Completly copy ch from *src to *des                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
const UINT8 *Str_StrCpy(UINT8 *des, const UINT8 *src)
{
    const UINT8 *p = des;
	if (des == NULL || src == NULL)
	{
		return NULL ;
    }
    
	while (*src != '\0')
	{
		*des++ = *src++;
    }
	*des = '\0';

	return p;
}

/*============================================================================*/
/*  Function : Str_StrNCmp                                                    */
/*  Input    : %1 -- destination string                                       */
/*             %2 -- source string                                            */
/*             %3 -- the length which will be compared string                 */
/*  Output   :                                                                */
/*  Return   : 0  *des = *src                                                 */
/*             1  *des > *src                                                 */
/*             -1 *des < *src                                                 */
/*  Desc     : Compare Two String                                             */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Str_StrNCmp(const UINT8 *des, const UINT8 *src, UINT32 len)
{
	const UINT8 *p = des, *q = src;
	UINT32 flag, i = 0;

	if (len == 0)
	{
		return 0;
    }
    
	if (des == NULL && src == NULL)
	{
		return 0;
    }
    
	if (des == NULL)
	{
		return -1;
    }
    
	if (src == NULL)
	{
		return 1;
    }
    
	do
	{
		if (i > len) 
		{
			break;
		}
		else
		{
			if (*p > *q) 
			{
				flag = 1;
				break;
			}
			else if (*p < *q)
			{
				flag = -1;
				break;
			} 
			else if (*p == *q)
			{
				flag = 0;
            }
            
			p++;
			q++;
		}
	}while (*p != '\0' || *q != '\0');

	return flag;
}

/*============================================================================*/
/*  Function : Str_StrCmp                                                     */
/*  Input    : %1 -- string1                                                  */
/*             %2 -- string2                                                  */
/*  Output   :                                                                */
/*  Return   : 0 *p1 = *p2                                                    */
/*             1 *p1 > *p2                                                    */
/*             -1 *p1 < *p2                                                   */
/*  Desc     : Compare To String                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 Str_StrCmp(const UINT8 *p1, const UINT8 *p2)
{
	if(p1 == NULL && p2 == NULL)
	{
		return 0;
    }

	if(p1 == NULL)
	{
		return -1;
    }
    
	if(p2 == NULL)
	{
		return 1;
    }
    
	while(*p1 != '\0' && *p2 != '\0') 
	{
		if(*p1 > *p2)
		{
			return 1;
        }
        
		if(*p1 < *p2)
		{
			return -1;
        }
		p1++;
		p2++;
	}

	if(*p1 == '\0' && *p2 == '\0')
	{
		return 0;
    }
    
	if( *p1 == '\0')
	{
		return -1;
    }
    
	return 1;
}

/*============================================================================*/
/*  Function : Str_StrLen                                                     */
/*  Input    : %1 -- String                                                   */
/*  Output   :                                                                */
/*  Return   : String Len                                                     */
/*  Desc     : Return String Len                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Str_StrLen(const UINT8 *pch)
{
	UINT32 i;

	if(pch == NULL)
	{
		return 0;
    }
    
	i = 0;
	while(*pch != '\0') 
	{
		pch++;
		i++;
	}

	return i;
}

/*============================================================================*/
/*  Function : Str_StrNCpy                                                    */
/*  Input    : %1 -- destination string                                       */
/*             %2 -- source string                                            */
/*             %3 -- number of char which copy                                */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Copy count char to dest                                        */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 *Str_StrNCpy(UINT8 *dest,	const UINT8 *source , UINT32 count)
{
	UINT8 *start = dest;

	while (count && (*dest++ = *source++))    /* copy string */
		count--;

	if (count)                              /* pad out with zeroes */
	{
		while (--count)
		{
			*dest++ = '\0';
        }
    }
    
	return(start);
}

/*============================================================================*/
/*  Function : Str_ShrinkBySpace                                              */
/*  Input    : %2 -- source string                                            */
/*             %3 -- number of char which copy                                */
/*  Output   : %1 -- destination string                                       */
/*  Return   : the pointer of the dest string                                 */
/*  Desc     : press the string,instead the char \r \n \t blank of one blank  */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 *Str_ShrinkBySpace(UINT8 *dest, const UINT8 *src, UINT32 len)
{
	UINT8 *p = dest;
	const UINT8 *q = src;
	UINT32 i = 0;

	if (len <= 0 || q == NULL)
	{
		return NULL;
    }
    
	while (*q != '\0') 
	{
		if (*q == ' ' || *q == '\t' || *q == '\n' || *q == '\r') 
		{
			*p++ = ' ';
			q++;
			len--;

			while (*q == ' ' || *q == '\t' || *q == '\n' || *q == '\r') 
			{
				q++;
				len--;
			}
		} 
		else
		{
			*p++ = *q++;
			len--;
		}

		if (len == 0)
			break;
	}

	*p = '\0';

	return dest;
}

/*============================================================================*/
/*  Function : Str_MaxSubStrInRange                                           */
/*  Input    : %1 -- src String                                               */
/*             %2 -- from ch1                                                 */
/*             %3 -- to ch2                                                   */
/*  Output   : %4 -- dest string                                              */
/*             %5 -- sub-string length                                        */
/*  Return   : Error Code                                                     */
/*  Desc     : search the sub-string,                                         */
/*             sub-string contains chars which from ch1 to ch2                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_MaxSubStrInRange(const UINT8 *str, 
                          UINT8 ch1,
                          UINT8 ch2, 
                          UINT8 *dest,
                          UINT32 *length)
{
	UINT32 len,max = 0;
	const UINT8 *p = str, *q;

	if (p == NULL)
    {
		return;
    }
    
	while (*p != '\0')
	{
		if (*p >= ch1 && *p <= ch2)
		{
			p++;
			len++;

			if (len > max) 
			{
				max = len;
				q = p - len;
			}
		} 
		else
		{
			p++;
			len = 0;
		}
	}

	strcpy(dest, q);
	dest[max] = '\0';
	*length = max;

	return;
}

/*============================================================================*/
/*  Function : Str_LongestWordLen                                             */
/*  Input    : %1 -- Src String                                               */
/*  Output   :                                                                */
/*  Return   : Longest World Len                                              */
/*  Desc     : Return Longest Word Len In String, The Word sprate by Space    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Str_LongestWordLen(const UINT8 *src)
{
	UINT32 len = 0, max = -1;
	const UINT8 *p = src;

	while (*p !='\0') 
	{
		if (*p != ' ')
		{
			len++;
		}
		else
		{
			if (max < len)
			{
				max = len;
            }
            
			len = 0;
		}
		p++;
	}

	return max;
}

/*============================================================================*/
/*  Function : Str_SearchFirstSingleCh                                        */
/*  Input    : %1 -- --the whole string                                       */
/*  Output   :                                                                */
/*  Return   : ch is the first single char                                    */
/*  Desc     : search a char of first exist                                   */
/*             and the char is difference from others                         */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT8 Str_SearchFirstSingleCh(const UINT8 *str)
{
	const UINT8 *q, *begin, *p = str;
	UINT8 ch;
	UINT32 bflag = 1, fflag = 1;

	while (*p != '\0')
	{
		q = p + 1;

		if (*(q) == '\0')
		{
			ch = *p;
        }
        
		while (*q != '\0') 
		{
			if (*p == *q) 
			{
				bflag = 0;
				break;
			} 
			else 
			{
				begin = str;

				while (begin != p)
				{
					if (*begin == *p)
					{
						fflag = 0;
						break;
					} 
					else 
					{
						fflag = 1;
					}

					begin++;
				}
				bflag = 1;
			}
			q++;
		}

		if (bflag == 1 && fflag == 1)
		{
			ch = *p;
			break;
		}
		p++;
	}

	return ch;
}

/*============================================================================*/
/*  Function : Str_DeleteChar                                                 */
/*  Input    : %1 -- src String                                               */
/*             %2 -- delete ch                                                */
/*  Output   : %3 -- dest string                                              */
/*  Return   : Error Code                                                     */
/*  Desc     : delete ch char, then result store in dest                      */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_DeleteChar(const UINT8 *str, UINT8 ch, UINT8 *dest)
{
	UINT32 i, j = 0, len = strlen(str);

	for (i = 0; i < len; i++) 
	{
		if (str[i] != ch) 
        {
            dest[j++] = str[i];
        }
	}

	return;
}

/*============================================================================*/
/*  Function : Str_ShiftLeft                                                  */
/*  Input    : %1 -- the string will be shifted                               */
/*             %2 -- move number of char                                      */
/*  Output   : %3 -- result of rotate shift left                              */
/*  Return   : Error Code                                                     */
/*  Desc     : rotate shift left move the char                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_RotateShiftLeft(const UINT8 *src, UINT32 k, UINT8 *dest)
{
	UINT32 i;
	UINT32 srclen;

	if (src == NULL || k == 0)
	{
	    LOG_PRINT(LOG_LEVEL_ERROR, "Para Invalid.\n");
		return;
    }

    srclen = strlen(src);
    memcpy(dest, &src[k], srclen - k);

	for (i = 0; i < k; i++) 
	{
        dest[srclen - k + i] = src[i];
	}

    dest[srclen] = '\0';
    
	return;
}

/*============================================================================*/
/*  Function : Str_RotateShiftRight                                           */
/*  Input    : %1 -- the string will be shifted                               */
/*             %2 -- move number of char                                      */
/*  Output   : %3 -- result of rotate shift right                             */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Str_RotateShiftRight(const UINT8 *src, UINT32 k, UINT8 *dest)
{
	UINT32 i, len;

	if (src == NULL || k == 0)
	{
	    LOG_PRINT(LOG_LEVEL_ERROR, "Para Invalid.\n");
		return;
    }
    
	len = strlen(src);

    memcpy(&dest[k], src, len - k);
    
	for (i = 0; i < k; i++) 
	{
        dest[i] = src[len - k + i];
	}

	dest[len] = '\0';

	return;
}

/*============================================================================*/
/*  Function : Str_IsSubstr                                                   */
/*  Input    : %1 -- whole string                                             */
/*             %2 -- sub string                                               */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Judge substr is sub-string or not                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Str_IsSubstr(const UINT8 *str, const UINT8 *substr)
{
	UINT32 flag = STRING_ISNOT_SUBSTRING;
	const UINT8 *p,*q;
	
	p = str;
	q = substr;

	while (*p != '\0')
	{
		while (*q != '\0')
		{
			if (*p != *q) 
			{
				if (flag == STRING_IS_SUBSTRING)
				{
					flag = STRING_ISNOT_SUBSTRING;
					break;
				}
				p++;
			}
			else
			{
				flag = STRING_IS_SUBSTRING;
				q++;
				p++;
				if ((flag == STRING_IS_SUBSTRING) && (*q == '\0'))
					break;
			}
		}
		p++;
		
		if (flag == STRING_ISNOT_SUBSTRING)
		{
			break;
		}
	}
	
	return flag;
}

/*============================================================================*/
/*  Function : Str_CmdToVector                                                */
/*  Input    : %1 -- Cmd String                                               */
/*  Output   : %2 -- Cmd To Vector                                            */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.23                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Str_CmdToVector(UINT8 *pucCmdLine, UINT32 VectorMax, UINT8 *pucParaPos[])
{
    UINT32 VectorIndex = 0;
    UINT32 ParaCharIndex = 0;
    UINT8 *pStr = pucCmdLine;
   
    while (*pStr == ' ')
    {
        pStr++;
    }

    if (*pStr == '\n')
    {
        return 0;
    }
    
    while (*pStr != '\0' && *pStr != '\n')
    {
        if (*pStr == ' ' && *(pStr + 1) == ' ')
        {
            pStr++;
            continue;
        }
        else if (*pStr == ' ' && *(pStr + 1) == '\0')
        {
            break;
        }
        else if (*pStr == ' ' && *(pStr + 1) != ' ')
        {
            pucParaPos[VectorIndex][ParaCharIndex] = '\0';
            ParaCharIndex = 0;
            VectorIndex++;
        }
        else
        {
            pucParaPos[VectorIndex][ParaCharIndex++] = *pStr;
        }

        if (VectorIndex == VectorMax)
        {
            return VectorMax + 1;
        }

        pStr++;
    }

    pucParaPos[VectorIndex][ParaCharIndex] = '\0';

    return VectorIndex + 1;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

