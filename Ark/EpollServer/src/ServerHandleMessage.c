/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾���䱱����������� */
/*  ϵͳ���� : ZXONE 8600�⴫��ϵͳ                                           */
/*  �� �� �� : HandleMessage.c                                                */
/*  �������� :                                                                */
/*  �� �� �� : ������                                                         */
/*  �ļ��汾 : V1.00.001                                                      */
/*  �������� : 2014.10.23                                                     */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/

/*============================================================================*/
/* �����ı�׼����������Ǳ�׼��ͷ�ļ�                                         */
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
/* ���ļ�ʹ�õĳ�������                                                       */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õĺ궨��                                                         */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õ���������                                                       */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֱ���                                                                   */
/*============================================================================*/
static void *VoidDeal(void * arg);

static TMsgFuncMap g_MsgFuncMap[CALLBACK_MSGCODE_MAX] = {VoidDeal};

/*============================================================================*/
/* ���ļ�ʹ�õı���                                                           */
/*============================================================================*/

/*============================================================================*/
/* ���ʵ��                                                                   */
/*============================================================================*/
/*============================================================================*/
/*  ������ : HandleMessage                                                    */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
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
/*  ������ : AddMsgFuncMap                                                    */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
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
/*  ������ : SentReplyMsg                                                     */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
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
/*  ������ : process                                                          */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : Huyongfu                                                         */
/*  ��  �� : V1.00.00                                                         */
/*  ��  �� : 2014.12.12                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
static void *VoidDeal(void * arg)
{
    TMsgHead *pMsgHead = (TMsgHead *)arg;
    UINT8 *pSendMsg = "No Defined Action\n";

    SentReplyMsg(pMsgHead->ProtocalType, arg,  pMsgHead->ClientFd, pSendMsg, strlen(pSendMsg), -1);
    
    return;
}
