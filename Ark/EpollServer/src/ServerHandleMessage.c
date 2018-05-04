/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : HandleMessage.c                                                */
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
#include <string.h>
#include <pthread.h>  

#include "Arch.h"
#include "VarType.h"
#include "ThreadPool.h"
#include "ServerHandleMessage.h"
#include "MemoryPool.h"
#include "Dbug.h"
#include "Log.h"
#include "ProtocalParse.h"
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
static void *VoidDeal(void * arg);

static TMsgFuncMap g_MsgFuncMap[CALLBACK_MSGCODE_MAX] = {VoidDeal};

/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
/*============================================================================*/
/*  函数名 : HandleMessage                                                    */
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
void HandleMsg(INT32 fd, INT8 *pRecvMsg, INT32 RecvMsgLen, INT32 PoolId)
{
    TSimpleMsgBodyFormate *pMsg = NULL;
    void *pBuff = NULL;
    TMsgHead MsgHead;
    TMemNode *pNode;
    UINT8 *pUndefineAction = "Undefined Operation!\n";
    TProtocalParse pProtocal;

    DBUG_PRINT(DEBUG_OFF, "MsgCode:%d\n", pMsg->MsgCode);

    /* Protocol Parse */
    pProtocal = ProtocalParse(fd, pRecvMsg);
    pMsg = (TSimpleMsgBodyFormate *)pProtocal.pServiceMsgBody;
    
    if (pMsg->MsgCode < CALLBACK_MSGCODE_MAX && g_MsgFuncMap[pMsg->MsgCode].pFunc != NULL)
    {
        MsgHead.ClientFd = fd;
        MsgHead.RecvMsgLen = RecvMsgLen;
        MsgHead.ProtocalType = pProtocal.ProtocalType;
        
        if ((pBuff = (void *)MemPool_Malloc(RecvMsgLen + sizeof(TMsgHead))) == NULL)
        {
            printf("Malloc Failed!%s:%d\n", __FILE__, __LINE__);
            return;
        }

        memcpy((void *)pBuff, (void *)&MsgHead, sizeof(MsgHead));
        memcpy((INT8 *)pBuff + sizeof(MsgHead), pRecvMsg, RecvMsgLen);
        ThreadPool_AddWorker(g_MsgFuncMap[pMsg->MsgCode].pFunc, pBuff, PoolId);
    }
    else
    {
        pRecvMsg[CALLBACK_MSGCODE_MAX] = '\0';
        LOG_PRINT(LOG_LEVEL_ERROR, "%s Msg:\n%s\n", pUndefineAction, pRecvMsg);
        SentReplyMsg(pProtocal.ProtocalType, NULL,  fd, pUndefineAction, strlen(pUndefineAction), -1);
    }
    
    return;
}

/*============================================================================*/
/*  函数名 : AddMsgFuncMap                                                    */
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
INT32 AddMsgFuncMap(INT32 MsgCode, process pFunc)
{
    if (MsgCode > CALLBACK_MSGCODE_MAX || MsgCode < 0)
    {
        return -1;
    }
    
    g_MsgFuncMap[MsgCode].pFunc = pFunc;
    
    return 0;
}

/*============================================================================*/
/*  函数名 : SentReplyMsg                                                     */
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
void SentReplyMsg(UINT32 ProtocalType, INT8 *pRecvBuff, INT32 CliengFd, INT8 *pSendBuff, INT32 SendMsglen, INT32 ExecResult)
{
    if (NULL != pRecvBuff)
    {
        MemPool_Free(pRecvBuff);
    }

    if (NULL != pSendBuff)
    {
        write(CliengFd, pSendBuff, SendMsglen);
    }
    
    return;
}

/*============================================================================*/
/*  函数名 : process                                                          */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.12                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static void *VoidDeal(void * arg)
{
    TMsgHead *pMsgHead = (TMsgHead *)arg;
    UINT8 *pSendMsg = "No Defined Action\n";

    SentReplyMsg(pMsgHead->ProtocalType, arg,  pMsgHead->ClientFd, pSendMsg, strlen(pSendMsg), -1);
    
    return;
}
