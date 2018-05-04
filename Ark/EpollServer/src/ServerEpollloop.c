/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : epollwhile.c                                                   */
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
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>/*不加此头文件 print inet_ntoa(ClientAddr.sin_addr)报错 */
#include <errno.h>
#include <pthread.h>

#include "Arch.h"
#include "VarType.h"
#include "ThreadPool.h"
#include "ServerEpollloop.h"
#include "MemoryPool.h"
#include "ServerCount.h"
#include "Log.h"
#include "Dbug.h"
#include "ErrorCode.h"
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

static struct epoll_event *events = NULL;;

/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static void SetNonBlocking(INT32 SockFd);

static INT32 AddClientfdToWaitSetFromListener(INT32 index, INT32 epfd, INT32 Listener);

static void DealClientEvents(INT32 epfd, INT32 index, INT32 fd, INT32 PoolId);

static INT32 do_read(INT32 fd, INT8 *buff, INT32 len);
/*============================================================================*/
/*  函数名 : DealClientRequest                                                */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void *DealClientRequest(void *arg)
{
    INT32 nfds;
    INT32 i = 0;
    INT32 ClientLen = sizeof(struct sockaddr_in), NewFd;
    struct sockaddr_in ClientAddr;
    TpthreadArg *parg = arg;
    TClientOperationRecord Record;

    events = (struct epoll_event *)MemPool_Malloc(sizeof(struct epoll_event) * parg->ClientMaxNum);
    if (NULL == events)
    {
        printf("Malloc Memory For events Failed\n");
        return NULL;
    }
    
    while (1)
    {
        if ((nfds = epoll_wait(parg->epfd, events, parg->ClientMaxNum, -1)) == -1)
        {
            perror("poll_wait:");
            continue;
        }

        for (i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == parg->Listener)
            {
                do
                {
                    if ((NewFd = accept(parg->Listener, 
                                        (struct sockaddr *)&ClientAddr,
                                        &ClientLen)) <= 0)
                    {
                        if (errno == EAGAIN) /* all connect dealed */
                        {
                            break;
                        }
                        DBUG_PRINT(DEBUG_ON, "accept errno = %d\n", errno);
                    }

                    Record.SecondOfToday = GetCurrentSecondSOfToday();
                    Record.fd = NewFd;
                    Record.Oper = CLIENT_RECORD_OPER_CONNECT;
                    AddClientRecord(Record);
                    ClientCount(CLIENT_RECORD_OPER_CONNECT);
                    DBUG_PRINT(DEBUG_OFF, "Socket:%d Connect from %s:%d\n", 
                               NewFd,
                               inet_ntoa(ClientAddr.sin_addr), 
                               htons(ClientAddr.sin_port));
            
                    SetNonBlocking(NewFd);
                    if (EpollCtl(parg->epfd, EPOLL_CTL_ADD, NewFd, EPOLLIN | EPOLLET) < 0)
                    {
                        LOG_PRINT(LOG_LEVEL_ERROR, "epfd = %d NewFd = %d\n", parg->epfd, NewFd);
                        break;
                    }
                }while(1);
            }
            else
            {
                DealClientEvents(parg->epfd, i, events[i].data.fd, parg->PoolId);
            }
        }
    }

    return;
}

/*============================================================================*/
/*  函数名 : DealClientEvents                                                 */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void DealClientEvents(INT32 epfd, INT32 index, INT32 fd, INT32 PoolId)
{
    INT8 RecvBuff[SERVER_TRANSFORDATA_LEN];
    INT32 RecvLen;
    TClientOperationRecord Record;
    
    if (events[index].events & EPOLLIN)
    {
        RecvLen = read(fd, RecvBuff, SERVER_TRANSFORDATA_LEN);
        
        if (RecvLen <= 0)
        {
            if (EpollCtl(epfd, EPOLL_CTL_DEL, fd, EPOLLIN | EPOLLET) < 0)
            {
                printf("At %s:%d\n",__FILE__, __LINE__);
            }

            Record.fd = fd;
            Record.Oper = CLIENT_RECORD_OPER_CLOSE;
            close(fd);
            DBUG_PRINT(DEBUG_OFF, "Client Socket Close:%d\n", fd);
            AddClientRecord(Record);
            ClientCount(CLIENT_RECORD_OPER_CLOSE);
        }
        else
        {
            LOG_STREAM_PRINT(LOG_LEVEL_INFO, RecvBuff, RecvLen);
            LOG_PRINT(LOG_LEVEL_INFO, "%s\n", RecvBuff);
            Record.SecondOfToday = GetCurrentSecondSOfToday();
            Record.fd = fd;
            Record.Oper = CLIENT_RECORD_OPER_WRITE;
            AddClientRecord(Record);
            ClientCount(CLIENT_RECORD_OPER_WRITE);            
            
            HandleMsg(fd, RecvBuff, RecvLen, PoolId);
        }
    }
    else
    {
        printf("Undefined Event\n");
    }
}

/*
static INT32 do_read(INT32 fd, char *buff, INT32 len)
{
    INT32 RecvLen;

    RecvLen = read(fd, buff, SHELFDRIVER_TRANSFORDATA_LEN);

    if (RecvLen == 0)
    {
        AddFdToEpollSet(int epfd, int fd, int events)
    }
    
    if (() == -1)
    {
        if (errno == EAGAIN)
        {
            // 由于是非阻塞的模式,所以当errno为EAGAIN时,表示当前缓冲区已无数据可读
            printf("No data to read\n");
        }
        else if (errno == ECONNRESET)
        {
            // 对方发送了RST
            close(fd);
            printf("Client Send RST\n");
        }
        else if (errno == EINTR)
        {
            printf("Interrupt\n");
        }
        else
        {
            printf("Unknow error:%d\n", errno);
        }
    }

    return RecvLen;
}
*/
/*============================================================================*/
/*  函数名 : SetNonBlocking                                                   */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void SetNonBlocking(INT32 SockFd) 
{ 
    INT32 opt = fcntl(SockFd, F_GETFL);
  
    if (opt < 0) 
    { 
        printf("fcntl(F_GETFL) fail.\n");
        return; 
    }
  
    opt |= O_NONBLOCK; 
    if (fcntl(SockFd, F_SETFL, opt) < 0)
    { 
        printf("fcntl(F_SETFL) fail."); 
        return; 
    }

    return;
} 

/*============================================================================*/
/*  函数名 : InitServerSocket                                                 */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 InitServerSocket(void)
{
    INT32 Listener;
    struct sockaddr_in ServerAddr;
    INT32 ListenerLen = sizeof(struct sockaddr_in);
    INT32 opt = 1;
    UINT8 keyvalue[256];
    UINT32 app_port;
    UINT32 listen_max_num;

    if (-1 == GetProfileString("./server.cfg", "server", "app_port", keyvalue))
    {
        return LIB_ERR_TASK_CREATE_FAILED;
    }   

    app_port = atoi(keyvalue);

    if (-1 == GetProfileString("./server.cfg", "server", "listen_max_num", keyvalue))
    {
        return LIB_ERR_TASK_CREATE_FAILED;
    }   

    listen_max_num = atoi(keyvalue);
    
    if ((Listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket:");
        return -1;
    }

    SetNonBlocking(Listener);

    bzero(&ServerAddr, ListenerLen);
    ServerAddr.sin_family = PF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(app_port);
    
    setsockopt(Listener, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(Listener, (struct sockaddr *)&ServerAddr, ListenerLen) == -1) 
    {
        perror("bind() fail.\n");
        return -1;
    }

    if (listen(Listener, listen_max_num) == -1)
    {
        perror("listen:");
        return -1;
    }
    
    return Listener;
}

/*============================================================================*/
/*  函数名 : AddFdToEpollSet                                                  */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.24                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 EpollCtl(INT32 epfd, INT32 op, INT32 fd, INT32 events)
{
    struct epoll_event ev;

    ev.events = events;
    ev.data.fd = fd;

    //printf("fd = %d\n", fd);
    if (epoll_ctl(epfd, op, fd, &ev) < 0)
    {
        perror("epoll_ctl:");
        fprintf(stderr, "epoll set insertion error: fd=%d\n", fd);
        return -1;
    }
    
    return 0;
}

