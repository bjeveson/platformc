/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : DBConPool.c                                                    */
/*  功能描述 :                                                                */
/*  创 建 人 : 胡永福                                                         */
/*  文件版本 : V1.00.001                                                      */
/*  创建日期 : 2014.11.24                                                     */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/

/*============================================================================*/
/* 包含的标准库或者其他非标准库头文件                                         */
/*============================================================================*/
#include "Arch.h"
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#include <unistd.h>
#include <pthread.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "VarType.h"
#include "OSI.h"
#include "ErrorCode.h"
#include "MemoryPool.h"
#include "Search.h"
#include "Sort.h"
#include "Shell.h"
#include "List.h"
#include "Hash.h"
#include "Log.h"
#include "DBConPool.h"
#include "Dbug.h"
/*============================================================================*/
/* 本文件使用的常量定义                                                       */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的宏定义                                                         */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的数据类型                                                       */
/*============================================================================*/
static pthread_mutex_t DBConPoolList_Mutex;
/*============================================================================*/
/* 全局变量                                                                   */
/*============================================================================*/
static TDBConInfo g_DBConInfo[DB_CONNECT_NUM_MAX];

static TDBConInfo *pDBConInfo[DB_CONNECT_NUM_MAX];

static UINT32 g_TotalDBConHandleNum;
/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static UINT32 DBConCompareHandleForSort(void *pArrayEle1, void *pArrayEle2);

static UINT32 DBConCompareHandleForSearch(void *pArrayEle1, void *pDBConHandle);

static void PrintListData(void *pData);

static UINT32 DBConPoolShow(UINT32 argc, UINT8 *argv[]);

/*============================================================================*/
/*  函数名 : DBConPool_Init                                                   */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 DBConPool_Init(UINT32 PoolSize, 
                      const UINT8 *Hostname,
                      const UINT8 *UserName,
                      const UINT8 *PassWord,
                      const UINT8 *DBName,
                      UINT32 DBPort)
{
    UINT32 Loop = 0;
    MYSQL *DBConn;
    UINT32 Ret;
    TShell ShellCmd;

    if (PoolSize > DB_CONNECT_NUM_MAX)
    {
        printf("DBConPool Too Large\n");
        return LIB_ERR_LIST_INIT_FAILED;
    }
    
    if (pthread_mutex_init(&DBConPoolList_Mutex, NULL) != 0)  
    {  
        LOG_PRINT(LOG_LEVEL_ERROR, "DBConPoolList_Mutex");
        return LIB_ERR_MUTEX_INIT_FAILED;
    }

    memset(g_DBConInfo, 0, sizeof(TDBConInfo) * DB_CONNECT_NUM_MAX);
    memset(pDBConInfo, 0, sizeof(TDBConInfo *) * DB_CONNECT_NUM_MAX);
    g_TotalDBConHandleNum = PoolSize;
    
    for (Loop = 0; Loop < g_TotalDBConHandleNum; Loop++)
    {
        DBConn = mysql_init(NULL);
        DBUG_PRINT(DEBUG_OFF, "Loop = %d, %p", Loop, DBConn);
        
        if (NULL == DBConn)
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "Init Mysql Error\n");
            return LIB_ERR_CONNECT_DB_FAILED;
        }

        if(!mysql_real_connect(DBConn, Hostname, UserName, PassWord, DBName, DBPort, NULL, 0))
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "Connect Mysql Error\n");
            return LIB_ERR_CONNECT_DB_FAILED;
        }

        g_DBConInfo[Loop].DbConnHandle = DBConn;
        g_DBConInfo[Loop].Status = DB_POOL_HANDLE_FREE;

        pDBConInfo[Loop] = &g_DBConInfo[Loop];
    }

    Sort_Bubble((void **)pDBConInfo, g_TotalDBConHandleNum, DBConCompareHandleForSort);
    
    ShellCmd.MaxParaNum = 0;
    ShellCmd.pfunc = DBConPoolShow;
    memcpy(ShellCmd.ucCmdDesc, "dbcon", SHELL_CMD_STR_MAX_LEN * 3);
    memcpy(ShellCmd.ucCmdstr, "dbcon", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);  
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : DBConCompareHandle                                               */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.10                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 DBConCompareHandleForSort(void *pArrayEle1, void *pArrayEle2)
{
    TDBConInfo *pHandle1 = pArrayEle1;
    TDBConInfo *pHandle2 = pArrayEle2;


    if (pHandle1->DbConnHandle > pHandle2->DbConnHandle)
    {
        return COMPARE_MORE;
    }
    else if (pHandle1->DbConnHandle < pHandle2->DbConnHandle)
    {
        return COMPARE_LESS;
    }
    else
    {
        return COMPARE_EQUAL;
    }
}

/*============================================================================*/
/*  函数名 : DBConPool_GetConnect                                             */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
MYSQL *DBConPool_GetConnect(void)
{
    MYSQL *DBConn;
    UINT32 loop = 0;
    UINT32 ret;

    if ((ret = pthread_mutex_lock(&DBConPoolList_Mutex)) != 0)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "DBConPoolList_Mutex\n");
        return NULL;
    }

    for (loop = 0; loop < g_TotalDBConHandleNum; ++loop)
    {
        if (g_DBConInfo[loop].Status == DB_POOL_HANDLE_FREE)
        {
            g_DBConInfo[loop].Status = DB_POOL_HANDLE_BUSY;
            pthread_mutex_unlock(&DBConPoolList_Mutex);
            return (MYSQL *)g_DBConInfo[loop].DbConnHandle;
        }
    }

    pthread_mutex_unlock(&DBConPoolList_Mutex);
    return NULL;
}

/*============================================================================*/
/*  函数名 : DBConPool_ReleaseConnect                                         */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 DBConPool_ReleaseConnect(MYSQL *pDBConn)
{
    UINT32 loop, ret;
    
    if ((ret = pthread_mutex_lock(&DBConPoolList_Mutex)) != 0)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "IdleList_Mutex\n");
        return LIB_ERR_MUTEX_LOCK_FAILED;
    }

    loop = Search_Binary((void **)pDBConInfo, 
                         (void *)pDBConn, 
                         g_TotalDBConHandleNum, 
                         DBConCompareHandleForSearch);

    if (-1 == loop)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Search DBConnHandle Failed\n");
        ret = LIB_ERR_ARRAY_OUT;
    }
    else
    {
        g_DBConInfo[loop].Status = DB_POOL_HANDLE_FREE;
        ret = LIB_ERR_SUCCESS;
    }
    
    pthread_mutex_unlock(&DBConPoolList_Mutex);    
    
    return ret;
}

/*============================================================================*/
/*  函数名 : DBConCompareHandleForSearch                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.10                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 DBConCompareHandleForSearch(void *pArrayEle1, void *pDBConHandle)
{
    TDBConInfo *pHandle = pArrayEle1;
    MYSQL *pDBConn = pDBConHandle;


    if (pHandle->DbConnHandle > pDBConn)
    {
        return COMPARE_MORE;
    }
    else if (pHandle->DbConnHandle < pDBConn)
    {
        return COMPARE_LESS;
    }
    else
    {
        return COMPARE_EQUAL;
    }
}

/*============================================================================*/
/*  函数名 : DBConPool_ExecuteSql                                             */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.2                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 DBConPool_ExecuteSql(MYSQL *pConHandle, const UINT8 *pSql, MYSQL_RES **pRes)
{  
    DBUG_PRINT(DEBUG_OFF, "pConHandle:%p pSql:%s", pConHandle, pSql);
    
    if (mysql_real_query(pConHandle, pSql, strlen(pSql)))
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Connect DB Failed\n");
        return -1;
    }
    else
    {
        *pRes = mysql_store_result(pConHandle);
        return 0; 
    }
}

/*============================================================================*/
/*  函数名 : DBConPool_GetRowNum                                              */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.2                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 DBConPool_GetRowNum(MYSQL_RES *pRes)
{
    return mysql_num_rows(pRes); 
}

/*============================================================================*/
/*  函数名 : DBConPool_GetColNum                                              */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.2                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 DBConPool_GetColNum(MYSQL_RES *pRes)
{
    return mysql_num_fields(pRes); 
}

/*============================================================================*/
/*  函数名 : DBConPool_FetchRow                                               */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.2                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
MYSQL_ROW DBConPool_FetchRow(MYSQL_RES *pRes)
{
    return mysql_fetch_row(pRes); 
}

/*============================================================================*/
/*  函数名 : DBConPool_FreeResult                                             */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.2                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void DBConPool_FreeResult(MYSQL_RES *pRes)
{
    return mysql_free_result(pRes); 
}

/*============================================================================*/
/*  函数名 : DBConPool_Destroy                                                */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 DBConPool_Destroy(void)
{
    MYSQL *DBConn;
    UINT32 try = 3;
    INT32 loop;
    UINT32 ret;

    if ((ret = pthread_mutex_lock(&DBConPoolList_Mutex)) != 0)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "BusyList_Mutex Mutes Error\n");
        return LIB_ERR_MUTEX_LOCK_FAILED;
    }

    for (loop = 0; loop < g_TotalDBConHandleNum; ++loop)
    {
        if (g_DBConInfo[loop].Status == DB_POOL_HANDLE_FREE)
        {
            mysql_close(g_DBConInfo[loop].DbConnHandle);
        }
        else
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "DB Handle:%p Connecting...\n", g_DBConInfo[loop].DbConnHandle);
        }
    }

    pthread_mutex_unlock(&DBConPoolList_Mutex);
    
    return;
}

/*============================================================================*/
/*  函数名 : DBConPool_Show                                                   */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 DBConPoolShow(UINT32 argc, UINT8 *argv[])
{   
    INT32 ret;
    UINT32 loop = 0;

    printf("=================DB Busy Connect List========================\n");
    if ((ret = pthread_mutex_lock(&DBConPoolList_Mutex)) != 0)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "BusyList_Mutex Error\n");
        return LIB_ERR_MUTEX_LOCK_FAILED;
    }

    printf("       Index     DBConHandle\n");
    for (loop = 0; loop < g_TotalDBConHandleNum; ++loop)
    {
        if (g_DBConInfo[loop].Status == DB_POOL_HANDLE_BUSY)
        {
            printf("%8d          %p\n", loop, g_DBConInfo[loop].DbConnHandle);
        }
    }

    printf("=================DB Idle Connect List========================\n");
    printf("       Index     DBConHandle\n");
    for (loop = 0; loop < g_TotalDBConHandleNum; ++loop)
    {
        if (g_DBConInfo[loop].Status == DB_POOL_HANDLE_FREE)
        {
            printf("%8d          %p\n", loop, g_DBConInfo[loop].DbConnHandle);
        }
    }

    pthread_mutex_unlock(&DBConPoolList_Mutex);

    return LIB_ERR_SUCCESS;
}

