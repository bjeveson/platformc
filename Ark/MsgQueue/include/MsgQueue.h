/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Queue.h                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _MSGQUEUE_H
#define _MSGQUEUE_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define MSGQUE_MAX_NUM         10
#define MSGQUE_MSG_MAX_LEN     1024 * 4

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef struct
{
    long m_ulMsgType;
    UINT8 m_ucMsgTxt[MSGQUE_MSG_MAX_LEN];
}TQueMsgBuff;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 MsgQue_Create(void);

UINT32 MsgQue_SendMsg(INT32 QueueIndex, const TQueMsgBuff *pMsgBuff, UINT32 ulMsgLen);

UINT32 MsgQue_RecvMsg(INT32 QueueIndex, TQueMsgBuff *pMsgBuff);

UINT32 MsgQue_DestroyById(INT32 QueueIndex);

UINT32 MsgQue_GetCurMsgNum(INT32 QueueIndex);

UINT32 MsgQue_GetCurBytes(INT32 QueueIndex);

UINT32 MsgQue_AllDestroy(void);

#endif

