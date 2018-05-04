/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : MemDbS.c                                                       */
/*  Descript : Memory DataBase                                                */
/*  API Pref : MMDB_                                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>

#include "ErrorCode.h"

#include "Arch.h"
#include "VarType.h"
#include "MemDbS.h"
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
/*  Function : MMDB_CreateTable                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.19                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
TableHandle MMDB_CreateTable(const UINT8 *pucStoreFile, INT8 *ParaName, ...)
{
    TTableInfo *pTableInfo = NULL;
    UINT32 ulFileNamelen = strlen(pucStoreFile);
    va_list args;
    UINT8 *pucColName = NULL;
    UINT32 ulColDescLen;

    pTableInfo = (TTableInfo *)MemPool_Malloc(sizeof(TTableInfo));

    if (NULL == pTableInfo)
    {
        return NULL;
    }
    
    memset(pTableInfo, 0, sizeof(TTableInfo));

    
    if (ulFileNamelen > MMDB_STORE_FILENAME_LEN)
    {
        return NULL;
    }
    
    memcpy((void *)pTableInfo->StoreFile, 
           (void *)pucStoreFile,
           strlen(pucStoreFile) + 1);

    va_start(args, ParaName);

    for (; NULL != ParaName; ParaName = va_arg(args, void *))
    {
        ulColDescLen = strlen(ParaName);
        pucColName = (UINT8 *)MemPool_Malloc(ulColDescLen + 1);
        memcpy(pucColName, ParaName, ulColDescLen + 1);
        pTableInfo->ColName[pTableInfo->ColNum] = pucColName;
        pTableInfo->ColNum++;
    }
    
    va_end(args);
    
    return (TableHandle)pTableInfo;
}

/*============================================================================*/
/*  Function : MMDB_Insert                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.19                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MMDB_Insert(TableHandle handle, INT8 *RowData, ...)
{
    TTableInfo *pTableInfo = NULL;
    TTableInfo *pTableInfoAppend = NULL;

    pTableInfo = (TTableInfo *)handle;
    
    if (NULL == pTableInfo)
    {
        return LIB_ERR_POINTER_NULL;
    }

    if (pTableInfo->CurRowNum == pTableInfo->TotalRowNum)
    {
        pTableInfoAppend = (TTableInfo *)realloc(handle, 
                                                 sizeof(TRowInfo) * MMDB_BUFFER_LINE_NUM);
        if (NULL == pTableInfo)
        {
            return LIB_ERR_POINTER_NULL;
        }
    }

	return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


