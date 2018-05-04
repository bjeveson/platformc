/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : String.h                                                       */
/*  Descript : String Operation Function                                      */
/*  API Pref : Str_                                                           */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _STRINGS_H
#define _STRINGS_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define STRING_IS_SUBSTRING         1
#define STRING_ISNOT_SUBSTRING      0
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

void Str_MaxSameSubStr(const UINT8 *str1, const UINT8 *str2, UINT8 *pucSubStr);

void Str_MaxSameCharSubStr(const INT8 *str, UINT8 *pucOutStr);

void Str_StrNCat(UINT8 *des, const UINT8 *src, UINT32 n);

void Str_IndexOfSubstr(const UINT8 *str, const UINT8 *substr, UINT32 *Index);

const UINT8 *Str_StrCpy(UINT8 *des, const UINT8 *src);

INT32 Str_StrNCmp(const UINT8 *des, const UINT8 *src, UINT32 len);

INT32 Str_StrCmp(const UINT8 *p1, const UINT8 *p2);

UINT32 Str_StrLen(const UINT8 *pch);

UINT8 *Str_StrNCpy(UINT8 *dest,	const UINT8 *source , UINT32 count);

UINT8 *Str_ShrinkBySpace(UINT8 *dest, const UINT8 *src, UINT32 len);

void Str_MaxSubStrInRange(const UINT8 *str, 
                          UINT8 ch1,
                          UINT8 ch2, 
                          UINT8 *dest,
                          UINT32 *length);

UINT32 Str_IsSubStr(const UINT8 *str, const UINT8 *substr);

UINT32 Str_LongestWordLen(const UINT8 *src);

UINT8 Str_SearchFirstSingleCh(const UINT8 *str);

void Str_DeleteChar(const UINT8 *str, UINT8 ch, UINT8 *dest);

void Str_RotateShiftLeft(const UINT8 *src, UINT32 k, UINT8 *dest);

void Str_RotateShiftRight(const UINT8 *src, UINT32 k, UINT8 *dest);

UINT32 Str_CmdToVector(UINT8 *pucCmdLine, UINT32 VectorMax, UINT8 *pucParaPos[]);

#endif

