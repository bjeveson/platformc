/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : ThreadPool.c                                                   */
/*  功能描述 :                                                                */
/*  创 建 人 : 胡永福                                                         */
/*  文件版本 : V1.00.001                                                      */
/*  创建日期 : 2014.10.21                                                     */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/

/*============================================================================*/
/* 包含的标准库或者其他非标准库头文件                                         */
/*============================================================================*/
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <pthread.h>  
#include <assert.h> 
#include <string.h>

#include "Arch.h"
#include "VarType.h"
#include "MemoryPool.h"
#include "ErrorCode.h"
#include "ThreadPool.h"
#include "Dbug.h"
#include "Log.h"
#include "Shell.h"
#include "ServerHandleMessage.h"
#include "Shell.h"
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
static CThread_pool *g_pool[THREAD_MAX_POOLS_NUM] = {NULL}; 
/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static void *thread_routine(void *arg);

static INT32 ThreadPool_GetIdleID(void);

static UINT32 ThreadQueueShow(UINT32 argc, UINT8 *argv[]);
/*============================================================================*/
/*  函数名 : ThreadPool_Init                                                  */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.21                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 ThreadPool_Init(INT32 max_thread_num, INT32 max_queue_size) 
{ 
	INT32 i = 0;
	INT32 poolid = ThreadPool_GetIdleID();
	TThreadInfo Thread;
	TShell ShellNode;

	if (poolid == -1)
	{
	    LOG_PRINT(LOG_LEVEL_ERROR, "Thread Pool Init Failed\n");
	    return -1;
	}
	
	g_pool[poolid] = (CThread_pool *)MemPool_Malloc(sizeof(CThread_pool)); 
    if (NULL == g_pool[poolid])
    {
        printf("Error Happend:%s:%d\n", __FILE__, __LINE__);
        return -1;
    }
    
	pthread_mutex_init(&(g_pool[poolid]->queue_lock), NULL); 
	pthread_cond_init(&(g_pool[poolid]->queue_ready), NULL); 
	g_pool[poolid]->queue_head = NULL; 
	g_pool[poolid]->max_thread_num = max_thread_num; 
	g_pool[poolid]->max_queue_size = max_queue_size;
	g_pool[poolid]->cur_queue_size = 0; 
	g_pool[poolid]->shutdown = 0;
	g_pool[poolid]->threadid = (pthread_t *)MemPool_Malloc(max_thread_num * sizeof(pthread_t)); 
    if (NULL == g_pool[poolid]->threadid)
    {
        printf("Error Happend:%s:%d\n", __FILE__, __LINE__);
        return -1;
    }

	for (i = 0; i < max_thread_num; i++) 
	{  
		pthread_create(&(g_pool[poolid]->threadid[i]), NULL, thread_routine, g_pool[poolid]);
		Thread.pid = g_pool[poolid]->threadid[i];
		sprintf(Thread.desc, "Thread Pool Thread No.%d", i);
		Sh_AddThreadInfo(Thread);
	}

    ShellNode.MaxParaNum = 1;
    ShellNode.pfunc = ThreadQueueShow;
    sprintf(ShellNode.ucCmdDesc, "Display Cur Wait Queue Length & Total Length");
    sprintf(ShellNode.ucCmdstr, "queue");
    Sh_AddCmd(ShellNode);
    
	return poolid;
} 

/*============================================================================*/
/*  函数名 : ThreadPool_AddWorker                                             */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.21                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 ThreadPool_AddWorker(void *(*process) (void *arg), void *arg, INT32 PollId)
{ 

	CThread_worker *newworker = NULL;
    INT32 retvalue = LIB_ERR_SUCCESS;
    //UINT8 *pSendMsg = "Server Busy Now.\n";
    TMsgHead *pMsgHead = (TMsgHead *)arg;

	pthread_mutex_lock(&(g_pool[PollId]->queue_lock));

	if (g_pool[PollId]->cur_queue_size < g_pool[PollId]->max_queue_size)
	{
	    newworker = (CThread_worker *)MemPool_Malloc(sizeof(CThread_worker));
	    if (NULL == newworker)
	    {
	        printf("Error Happened %s:%d\n", __FILE__, __LINE__);
	        pthread_mutex_unlock(&(g_pool[PollId]->queue_lock));
	        return LIB_ERR_POINTER_NULL;
	    }
	    
	    newworker->process = process; 
	    newworker->arg = arg; 
	    newworker->next = NULL; 
        //printf("%p", newworker->arg);
	    CThread_worker *member = g_pool[PollId]->queue_head; 

	    if (member != NULL) 
    	{ 
    		while (member->next != NULL) 
    			member = member->next; 
    		member->next = newworker; 
    	} 
    	else 
    	{ 
    		g_pool[PollId]->queue_head = newworker; 
    	} 

	    assert(g_pool[PollId]->queue_head != NULL); 

	    g_pool[PollId]->cur_queue_size++;
    }
    else
    {
        LOG_PRINT(LOG_LEVEL_OFF, "Thread Pool Queue Full,Give This Request\n");
        //SentReplyMsg(arg, pMsgHead->ClientFd, pSendMsg, strlen(pSendMsg));
        retvalue = LIB_ERR_QUEUE_FULL;
    }
    
	pthread_mutex_unlock(&(g_pool[PollId]->queue_lock)); 
	pthread_cond_signal(&(g_pool[PollId]->queue_ready)); 
	
	return retvalue; 
} 

/*============================================================================*/
/*  函数名 : ThreadPool_Destroy                                               */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.21                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 ThreadPool_Destroy(INT32 PoolId)
{ 
	INT32 i; 
    UINT32 loop = 0, begin, end;

    if (PoolId == -1)
    {
        begin = 0;
        end = THREAD_MAX_POOLS_NUM;
    }
    else
    {
        begin = PoolId;
        end = PoolId + 1;
    }
    
    for (loop = begin; loop < end; loop++)
    {
        if (g_pool[loop] == NULL)
        {
            continue;
        }
        
    	if (g_pool[loop]->shutdown) 
    		return -1; 

    	g_pool[loop]->shutdown = 1; 

    	pthread_cond_broadcast(&(g_pool[loop]->queue_ready)); 

    	for (i = 0; i < g_pool[loop]->max_thread_num; i++) 
    		pthread_join(g_pool[loop]->threadid[i], NULL); 

    	MemPool_Free(g_pool[loop]->threadid); 

    	CThread_worker *head = NULL; 
    	while(g_pool[loop]->queue_head != NULL) 
    	{ 
    		head = g_pool[loop]->queue_head; 
    		g_pool[loop]->queue_head = g_pool[loop]->queue_head->next; 
    		MemPool_Free(head); 
    	} 

    	pthread_mutex_destroy(&(g_pool[loop]->queue_lock)); 
    	pthread_cond_destroy(&(g_pool[loop]->queue_ready)); 

    	MemPool_Free(g_pool[loop]); 
    }
    
 	return 0; 
} 

/*============================================================================*/
/*  函数名 : thread_routine                                                   */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.21                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void *thread_routine(void *arg)
{ 
    CThread_pool *pool = (CThread_pool *)arg;
    DBUG_PRINT(DEBUG_OFF, "starting thread 0x%x\n", (UINT32)pthread_self()); 

	while (1) 
	{ 
		pthread_mutex_lock(&(pool->queue_lock)); 

		while(pool->cur_queue_size == 0 && !pool->shutdown) 
		{ 
			DBUG_PRINT(DEBUG_OFF, "thread 0x%x is waiting\n", pthread_self()); 
			pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock)); 
		} 

		if (pool->shutdown) 
		{ 

			pthread_mutex_unlock(&(pool->queue_lock)); 
			printf("thread 0x%x will exit\n", (UINT32)pthread_self()); 
			pthread_exit(NULL); 
		} 

		DBUG_PRINT(DEBUG_OFF, "thread 0x%x is starting to work\n", pthread_self()); 

		assert(pool->cur_queue_size != 0); 
		assert(pool->queue_head != NULL); 

		pool->cur_queue_size--; 
		CThread_worker *worker = pool->queue_head; 
		pool->queue_head = worker->next; 
		pthread_mutex_unlock(&(pool->queue_lock)); 

		(*(worker->process))(worker->arg);

		MemPool_Free(worker); 
	} 

	pthread_exit (NULL); 
} 

/*============================================================================*/
/*  函数名 : ThreadPool_GetIdleID                                             */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.6                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static INT32 ThreadPool_GetIdleID(void)
{
    UINT32 loop = 0;

    for (loop = 0; loop < THREAD_MAX_POOLS_NUM; ++loop)
    {
        if (g_pool[loop] == NULL)
        {
            return loop;
        }
    }

    return -1;
}

/*============================================================================*/
/*  函数名 : ThreadQueueShow                                                  */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.13                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 ThreadQueueShow(UINT32 argc, UINT8 *argv[])
{
    UINT32 poolid;
    UINT32 Index = 0;

    if (argc == 1)
    {
        for (Index = 0; Index < THREAD_MAX_POOLS_NUM; Index++)
        {
            if (g_pool[Index] != NULL)
            {
                printf("**************Thread Pool Id:%d*********************\n", Index);
                printf("Current ThreadPool Wait Queue Length:%d\n" , g_pool[Index]->cur_queue_size);
                printf("ThreadPool Wait Queue Total Length  :%d\n", g_pool[Index]->max_queue_size);
                printf("****************************************************\n");  
            }
        }
    }
    else if (argc == 2)
    {
        poolid = atoi(argv[1]);
        if (poolid >= THREAD_MAX_POOLS_NUM || g_pool[poolid] == NULL)
        {
            printf("Pool ID Error\n");
            return -1;
        }
        
        printf("**************Thread Pool Id:%d*********************\n", poolid);
        printf("Current ThreadPool Wait Queue Length:%d\n" , g_pool[poolid]->cur_queue_size);
        printf("ThreadPool Wait Queue Total Length  :%d\n", g_pool[poolid]->max_queue_size);
        printf("****************************************************\n");        
    }
    else
    {
        printf("Para NUm Error\n");
        return -1;
    }
    
    return 0;
}

