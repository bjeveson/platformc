/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : TThreadPool.c                                                  */
/*  功能描述 :                                                                */
/*  创 建 人 : 胡永福                                                         */
/*  文件版本 : V1.00.001                                                      */
/*  创建日期 : 2014.10.23                                                     */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/

/*============================================================================*/
/* 包含的标准库或者其他非标准库头文件                                         */
/*============================================================================*/
#include <stdio.h>

#include "VarType.h"
#include "ErrorCode.h"
#include "ThreadPool.h"
#include "TThreadPool.h"
#include "TestDrv.h"
#include "Log.h"
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
static  INT32 poolId = 0;
static INT32 worker_id[TTHREAD_POOL_WORKER_NUM] = {0};
/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static void *process(void * arg);

void TThreadPool_Pool_Init_Case(void *arg);

void TThreadPool_pool_add_worker_Case(void *arg);

/*============================================================================*/
/*  函数名 : TThreadPool_AddTestCase                                          */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.23                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 TThreadPool_AddTestCase(void)
{
    TTestCase TestCase[] = 
    {
        {
            "TThreadPool_Pool_Init_Case",
            TThreadPool_Pool_Init_Case,
            NULL
        },      
        {
            "TThreadPool_pool_add_worker_Case",
            TThreadPool_pool_add_worker_Case,
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
/*  函数名 : TThreadPool_Pool_Init_Case                                       */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.23                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void TThreadPool_Pool_Init_Case(void *arg)
{
    poolId = ThreadPool_Init(100, 1000);
    TD_ASSERT_NOT_EQUAL(poolId, -1);
    
    return;
}

/*============================================================================*/
/*  函数名 : TThreadPool_pool_add_worker_Case                                 */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.23                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void TThreadPool_pool_add_worker_Case(void *arg)
{
    INT32 retvalue = 0;
    INT32 i = 0;
    INT32 sleeptime = 5;

    for (i = 0; i < TTHREAD_POOL_WORKER_NUM; i++)
    {
        worker_id[i] = i;
        retvalue = ThreadPool_AddWorker(process, (void *)&worker_id[i], poolId);
        if (retvalue == LIB_ERR_QUEUE_FULL)
        {
            printf("Work Queue Full! Wait:%d\n", sleeptime);
            i--;
            //sleep(sleeptime);
        }
    }

    retvalue = ThreadPool_Destroy(poolId);
    TD_ASSERT_EQUAL(retvalue, 0);
    
    return;
}

/*============================================================================*/
/*  函数名 : process                                                          */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.23                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void *process(void * arg)
{
    printf("I am Worker:%d\n",  *((INT32 *)arg));
    //sleep(3);
    
    return;
}

