/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : TDBConPool.c                                                   */
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
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "VarType.h"
#include "ErrorCode.h"
#include "TestDrv.h"
#include "Log.h"
#include "TDBConPool.h"
#include "DBConPool.h"
/*============================================================================*/
/* 本文件使用的常量定义                                                       */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的宏定义                                                         */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的数据类型                                                       */
/*============================================================================*/

/*============================================================================*/
/* 全局变量                                                                   */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static void DBConPool_Init_Case(void *Para);

static void *DBConPoolTestThread(void *arg);

static void DBConPool_GetRelease_Case(void *Para);

/*============================================================================*/
/*  函数名 : TDBConPool_AddTestCase                                           */
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
UINT32 TDBConPool_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "DBConPool_Init_Case",
            DBConPool_Init_Case,
            NULL
        },   
        {
            "DBConPool_GetRelease_Case",
            DBConPool_GetRelease_Case,
            NULL
        },          
        {
            "Over",
            NULL,
            NULL
        }
    };
    
    return TD_AddTestCase(TestCase);
}

/*============================================================================*/
/*  函数名 : DBConPool_Init_Case                                              */
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
static void DBConPool_Init_Case(void *Para)
{
    UINT32 ret;
    
    ret = DBConPool_Init(150, 
                         "10.85.159.218", 
                         "root", 
                         "root",
                         "ijiaotang",
                         3306);

    TD_ASSERT_EQUAL(ret, LIB_ERR_SUCCESS);
    
    return;
}

/*============================================================================*/
/*  函数名 : DBConPool_GetRelease_Case                                        */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.26                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void DBConPool_GetRelease_Case(void *Para)
{
    UINT32 loop = 0;
    UINT32 ret;
    pthread_t pid[TDBCONPOOL_TEST_THREAD_NUM_MAX];
    
    for (loop = 0; loop < TDBCONPOOL_TEST_THREAD_NUM_MAX; loop++)
    {
        if ((ret = pthread_create(&pid[loop], NULL, DBConPoolTestThread, NULL)) != 0)
        {
            printf("loop = %d At %s:%d\n",loop, __FILE__, __LINE__);
            perror("");
            
            return;
        }
    }
    
	for (loop = 0; loop < TDBCONPOOL_TEST_THREAD_NUM_MAX; loop++)
	{
		pthread_join(pid[loop], NULL);
    }

    printf("=====================================================");
    sleep(2);
    
    return;
}

/*============================================================================*/
/*  函数名 : DBConPoolTestThread                                              */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.26                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void *DBConPoolTestThread(void *arg)
{
    UINT32 loop = 1;
    MYSQL *pSqlHandle;
    MYSQL_RES *pRes = NULL;
    UINT8 SqlBuff[1024];
    INT32 Ret;

    while (loop--)
    {
        pSqlHandle = DBConPool_GetConnect();
        if (NULL == pSqlHandle)
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "Get DBConPool Connect Failed %p.\n", pSqlHandle);
            usleep(Math_Random((ULong)pSqlHandle) % 50);
            continue;
        }

        sprintf(SqlBuff, "select * from T_Test");
        
        Ret = DBConPool_ExecuteSql(pSqlHandle, SqlBuff, &pRes);
        TD_ASSERT_EQUAL(Ret, 0);
        DBConPool_FreeResult(pRes);
        DBConPool_ReleaseConnect(pSqlHandle);
        sleep(1);
    }
   

    return;
}

