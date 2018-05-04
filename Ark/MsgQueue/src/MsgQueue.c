/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Queue.c                                                        */
/*  Descript : can't call macro log_print                                     */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include "Arch.h"

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h> 

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "ErrorCode.h"
#include "VarType.h"
#include "MsgQueue.h"
#include "Log.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static INT32 g_MsgQueueIdList[MSGQUE_MAX_NUM] = {0};

static UINT32 g_CurMsgQueueNum = 0;

/*============================================================================*/
/* Macro Define                                                               */
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
/*  Function : Que_Create                                                     */
/*  Input    : %1 -- void                                                     */
/*  Output   :                                                                */
/*  Return   : void                                                           */
/*  Desc     : Create Queue                                                   */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MsgQue_Create(void)
{   
    if (g_CurMsgQueueNum >= MSGQUE_MAX_NUM)
    {
        printf("Queue List Enough,Cant Create!\n");

        return 0;
    }
    
    if ((g_MsgQueueIdList[g_CurMsgQueueNum] = msgget(0, 0666)) == -1)
    {
        perror("msgget error!\n");
        return 0;     
    }

    g_CurMsgQueueNum++;

    return g_CurMsgQueueNum - 1;
}

/*============================================================================*/
/*  Function : Que_SendMsg                                                    */
/*  Input    : %1 -- Queue Index                                              */
/*           : %2 -- Msg Buffer Pointer                                       */
/*           : %3 -- Msg Length                                               */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Send Msg To Queue                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MsgQue_SendMsg(INT32 QueueIndex, 
                      const TQueMsgBuff *pMsgBuff,
                      UINT32 ulMsgLen)
{
    if (QueueIndex < 0 || QueueIndex >= MSGQUE_MAX_NUM)
    {
        printf("Queue ID Not Exist\n");
        return LIB_ERR_QUEUE_OPERFAIL;
    }

    if (NULL == pMsgBuff)
    {
        printf("Send Msg Buffer NULL.\n");
        return LIB_ERR_POINTER_NULL;
    }

    if (ulMsgLen > MSGQUE_MSG_MAX_LEN)
    {
        printf("Send Msg Too Large\n");
        return LIB_ERR_ARRAY_OUT;
    }

    if (msgsnd(g_MsgQueueIdList[QueueIndex], 
               (struct msgbuf *)pMsgBuff,
               ulMsgLen, 
               IPC_NOWAIT)
        == -1)     
    {     
        printf("Log Message Send Error!QueueIndex = %d g_MsgQueueIdList[QueueIndex] = %d ulMsgLen = %d\n",
               QueueIndex, g_MsgQueueIdList[QueueIndex], ulMsgLen);
        perror("");     
        return LIB_ERR_QUEUE_OPERFAIL;
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Que_RecvMsg                                                    */
/*  Input    : %1 -- Queue Index                                              */
/*           : %2 -- Recv Msg Buffer Pointer                                  */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Recv Msg From Queue                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MsgQue_RecvMsg(INT32 QueueIndex, TQueMsgBuff *pMsgBuff)
{
    //TQueMsgBuff tMsgBuff;
    
    if (QueueIndex < 0 || QueueIndex >= MSGQUE_MAX_NUM)
    {
        printf("Queue ID Not Exist\n");
        return LIB_ERR_QUEUE_OPERFAIL;
    }

    if (NULL == pMsgBuff)
    {
        printf("Recv Msg Buffer NULL.\n");
        return LIB_ERR_POINTER_NULL;
    }

    if (msgrcv(g_MsgQueueIdList[QueueIndex], 
               (struct msgbuf *)pMsgBuff, 
               MSGQUE_MSG_MAX_LEN,
               0,
               0)
        == -1)
    {
        printf("Log Message Recv Error!QueueIndex = %d g_MsgQueueIdList[QueueIndex] = %d\n",
               QueueIndex, g_MsgQueueIdList[QueueIndex]);\
        if (errno == EIDRM)
        {
            printf("QueueIndex = %d Removed\n", QueueIndex);
        }
        else if (errno == ENOMSG)
        {
            printf("QueueIndex = %d No Msg To Read\n", QueueIndex);
        }
        else
        {
            printf("QueueIndex = %d errno = %d\n", QueueIndex, errno);
        }
        
        perror("");
        return LIB_ERR_QUEUE_OPERFAIL;
    }
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Que_DestroyById                                                */
/*  Input    : %1 -- Queue Index                                              */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Remove Queue From System                                       */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MsgQue_DestroyById(INT32 QueueIndex)
{
    if (QueueIndex < 0 || QueueIndex >= MSGQUE_MAX_NUM)
    {
        printf("Queue ID Not Exist\n");
        return LIB_ERR_QUEUE_OPERFAIL;
    }

    if (msgctl(g_MsgQueueIdList[QueueIndex], IPC_RMID, 0) == -1)
    {
        printf("Queue %d ", g_MsgQueueIdList[QueueIndex]);
        perror("Destroy Failed\n");
        //g_MsgQueueIdList[QueueIndex] = 0;
        return LIB_ERR_QUEUE_OPERFAIL;
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : Que_AllDestroy                                                   */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.8                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 MsgQue_AllDestroy(void)
{
    UINT32 Index;

    for (Index = 0; Index < MSGQUE_MAX_NUM; ++Index)
    {
        if (g_MsgQueueIdList[Index] > 0)
        {
            if (msgctl(g_MsgQueueIdList[Index], IPC_RMID, 0) == -1)
            {
                printf("Queue %d ", g_MsgQueueIdList[Index]);
                perror("Destroy Failed\n");
                //g_MsgQueueIdList[QueueIndex] = 0;
                return LIB_ERR_QUEUE_OPERFAIL;
            }
        }
    }

     return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Que_GetLength                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.11                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MsgQue_GetCurMsgNum(INT32 QueueIndex)
{
    struct msqid_ds msg_info;
    
    if (msgctl(g_MsgQueueIdList[QueueIndex], IPC_STAT, &msg_info) == -1)
    {
        perror("Get Queue Stat Failed.\n");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }
    else
    {
        return msg_info.msg_qnum;
    }
}

/*============================================================================*/
/*  Function : Que_GetCurBytes                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.11                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MsgQue_GetCurBytes(INT32 QueueIndex)
{
    struct msqid_ds msg_info;
    
    if (msgctl(g_MsgQueueIdList[QueueIndex], IPC_STAT, &msg_info) == -1)
    {
        perror("Get Queue Stat Failed.\n");
        printf("%s:%d errno = %d Queue id:%d\n", __FILE__, __LINE__, errno, g_MsgQueueIdList[QueueIndex]);
        return -1;
    }
    else
    {
        return msg_info.msg_cbytes;
    }
}
#endif
/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

