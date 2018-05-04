/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : FileSys.h                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.10                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _FILESYS_H
#define _FILESYS_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define STRING_FILE_TYPE_NULL       0
#define STRING_FILE_TYPE_PICTURE    1
#define STRING_FILE_TYPE_TEXT       2
#define STRING_FILE_TYPE_UNKNOW     UINT32_RESERVE

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 FS_ParseCfgFile(const UINT8 *filename ,
                       const UINT8 *key[],
                       UINT32 len,
                       UINT8 value[][255]);

UINT32 FS_FileType(const UINT8 *path, UINT8 *type);

UINT32 FS_GetCurDir(UINT8 *pucDir);

#endif

