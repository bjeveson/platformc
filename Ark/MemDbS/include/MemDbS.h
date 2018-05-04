/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : MemDbS.h                                                       */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _MEMDBS_H
#define _MEMDBS_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define MMDB_BUFFER_LINE_NUM    100

#define MMDB_STORE_FILENAME_LEN 256
#define MMDB_COL_MAX_NUM        64
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef struct
{
    void *Data;
    UINT32 CommitFlag;
}TRowInfo;

typedef struct TTable
{
    UINT32 TotalRowNum;
    UINT32 CurRowNum;
    UINT8 StoreFile[MMDB_STORE_FILENAME_LEN];
    UINT8 *ColName[MMDB_COL_MAX_NUM];
    UINT32 ColNum;
    TRowInfo *pRow;
}TTableInfo;

typedef void * TableHandle;
/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

#endif

