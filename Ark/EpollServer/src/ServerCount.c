/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : ClientCount.c                                                  */
/*  功能描述 :                                                                */
/*  创 建 人 : 胡永福                                                         */
/*  文件版本 : V1.00.001                                                      */
/*  创建日期 : 2014.10.27                                                     */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/

/*============================================================================*/
/* 包含的标准库或者其他非标准库头文件                                         */
/*============================================================================*/
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "VarType.h"
#include "ServerCount.h"
#include "TimeTag.h"
#include "Shell.h"
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

/*============================================================================*/
/* 全局变量                                                                   */
/*============================================================================*/
static TClientOperationRecord g_ClientRecord[CLIENT_RECORD_MAX_NUM] = {0};

static UINT32 g_RecordIndex = 0;

static pthread_mutex_t Record_Mutex;

/////////////////////////////////////////////
static TClientCount g_ClientCount = {0};

static pthread_mutex_t g_ClientCount_Mutex = {0};

/////////////////////////////////////////////
static TtimeTag g_TimeTag;

/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static UINT32 ShowClientCountInfo(UINT32 argc, UINT8 *argv[]);

static UINT32 ShowClientRecord(UINT32 argc, UINT8 *argv[]);

static void PrintRecord(INT32 index, TClientOperationRecord Record);
/*============================================================================*/
/*  函数名 : InitClientCount                                                  */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.27                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 InitClientCount(void)
{
    struct timeval tv;
    TShell ShellNode;
    UINT8 *pCountCmd = "count";
    UINT8 *pCountCmdDesc = "show client count info useage:count";
    UINT8 *pRecordCmd = "record";
    UINT8 *pRecordCmdDesc = "show client record usage:record [pos:0-all,1-befor,2-after] [fd:0 -all,fd]";

    if (pthread_mutex_init(&Record_Mutex, NULL) != 0)  
    {  
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }

    if (pthread_mutex_init(&g_ClientCount_Mutex, NULL) != 0)  
    {  
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }

    ShellNode.MaxParaNum = 0;
    ShellNode.pfunc = ShowClientCountInfo;
    strncpy(ShellNode.ucCmdDesc, pCountCmdDesc, strlen(pCountCmdDesc) + 1);
    strncpy(ShellNode.ucCmdstr, pCountCmd, strlen(pCountCmd) + 1);
    Sh_AddCmd(ShellNode);

    ShellNode.MaxParaNum = 2;
    ShellNode.pfunc = ShowClientRecord;
    strncpy(ShellNode.ucCmdDesc, pRecordCmdDesc, strlen(pRecordCmdDesc) + 1);
    strncpy(ShellNode.ucCmdstr, pRecordCmd, strlen(pRecordCmd) + 1);
    Sh_AddCmd(ShellNode);

    return 0;
}

/*============================================================================*/
/*  函数名 : AddClientRecord                                                  */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.27                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 AddClientRecord(TClientOperationRecord Record)
{
    INT32 ret;

    if ((ret = pthread_mutex_lock(&Record_Mutex)) != 0)
    {
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }
    
    g_ClientRecord[g_RecordIndex & (CLIENT_RECORD_MAX_NUM - 1)] = Record;
    g_RecordIndex++;
    
    pthread_mutex_unlock(&Record_Mutex);

    return 0;
}

/*============================================================================*/
/*  函数名 : ClientCount                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void ClientCount(INT32 CountOper)
{
    INT32 ret;

    if ((ret = pthread_mutex_lock(&g_ClientCount_Mutex)) != 0)
    {
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return;
    }

    switch (CountOper)
    {
        case CLIENT_RECORD_OPER_CONNECT:
        {
            g_ClientCount.OnlineClientNum++;
            break;
        }
        case CLIENT_RECORD_OPER_CLOSE:
        {
            g_ClientCount.OnlineClientNum--;
            break;
        }
        case CLIENT_RECORD_OPER_WRITE:
        {
            g_ClientCount.OnlineRequestNum++;
            break;
        }
        default:
        {
            printf("Input CountOper Error\n");
        }
    }

    pthread_mutex_unlock(&g_ClientCount_Mutex);
    
    return ;
}

/*============================================================================*/
/*  函数名 : ShowClientCountInfo                                              */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 ShowClientCountInfo(UINT32 argc, UINT8 *argv[])
{
    printf("OnlineUserNum OnlineRequestNum\n");

    printf("%8d    %8d\n",
           g_ClientCount.OnlineClientNum,
           g_ClientCount.OnlineRequestNum); 

    return 0;
}

/*============================================================================*/
/*  函数名 : ShowClientRecord                                                 */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 ShowClientRecord(UINT32 argc, UINT8 *argv[])
{
    INT32 pos = atoi(argv[1]);
    INT32 fd = atoi(argv[2]);
    INT32 loop = 0;

    printf("index SecondOfToday  fd      Oper\n");
    if (pos == 0)
    {
        if (fd == 0)
        {
            for (loop = 0; loop < CLIENT_RECORD_MAX_NUM; loop++)
            {
                PrintRecord(loop, g_ClientRecord[loop]);
            }
        }
        else
        {
            for (loop = 0; loop < CLIENT_RECORD_MAX_NUM; loop++)
            {
                if (g_ClientRecord[loop].fd == fd)
                {
                    PrintRecord(loop, g_ClientRecord[loop]);
                }
            }
        }
    }
    else if (pos == 1)
    {
        if (fd == 0)
        {
            for (loop = 0; loop < g_RecordIndex; loop++)
            {
                PrintRecord(loop, g_ClientRecord[loop]);
            }
        }
        else
        {
            for (loop = 0; loop < g_RecordIndex; loop++)
            {
                if (g_ClientRecord[loop].fd == fd)
                {
                    PrintRecord(loop, g_ClientRecord[loop]);
                }
            }
        }
    }
    else if (pos == 2)
    {
        if (fd == 0)
        {
            for (loop = 0; loop < g_RecordIndex; loop++)
            {
                PrintRecord(loop, g_ClientRecord[loop]);
            }
        }
        else
        {
            for (loop = g_RecordIndex; loop < CLIENT_RECORD_MAX_NUM; loop++)
            {
                if (g_ClientRecord[loop].fd == fd)
                {
                    PrintRecord(loop, g_ClientRecord[loop]);
                }
            }
        }
    }
    else
    {
        printf("Para pos Input Error\n");
        return -1;
    }
    
    return 0;
}

/*============================================================================*/
/*  函数名 : PrintRecord                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void PrintRecord(INT32 index, TClientOperationRecord Record)
{
    printf("%-8d %-8d %-8d %-8d\n", index, Record.SecondOfToday, Record.fd, Record.Oper);

    return;
}
