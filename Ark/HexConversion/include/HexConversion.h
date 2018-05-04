/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : HexConversion.h                                                */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _HEXCONVERSION_H
#define _HEXCONVERSION_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

UINT32 Hexcon_StringToDec(const UINT8 *pdec);

UINT8 *Hexcon_DecToAnyIn10(UINT32 dec, UINT32 bottom, UINT8 *any);

void Hexcon_DecToBinDisplay(UINT32 x);

UINT8 *Hexcon_DecToHex(UINT32 dec, UINT32 len, UINT8 *hex) ;

UINT32 Hexcon_HexToDec(const UINT8 *hex);

UINT8 *Hexcon_DecNumFormateToStr(UINT32 num, UINT8 des[]);

#endif

