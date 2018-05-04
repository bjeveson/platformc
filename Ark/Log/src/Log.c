/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Log.c                                                          */
/*  Descript :                                                                */
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
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>

#include "ErrorCode.h"
#include "VarType.h"
#include "MsgQueue.h"
#include "Log.h"
#include "Shell.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define LOG_OK_INIT_FLAG    0
#define LOG_NO_INIT_FLAG    1

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
/* Register Log Module Info */
static TLogRegInfo g_LogInfo;   

/* Queue Index In Queue Module */
static INT32 g_LogQueueId = -1;

/* Current Log File Pointer */
static FILE *Fp = NULL;

static UINT32 g_LogModuleInitFlag = LOG_NO_INIT_FLAG;

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
static pthread_t tid = 0;
#endif
/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void *RecvLogMsg(void *arg);

static void CreateLogFile(void);

static void SignoDeal(INT32 sig);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Log_Init                                                       */
/*  Input    : tLogInfo -- Register Info                                      */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Register Log Module Info For Init                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Log_Init(TLogRegInfo tLogInfo)
{    
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    struct sigaction act; 
    TThreadInfo thread;
    UINT8 *pdesc = "Recv Log Msg Thread";
    
    if (tLogInfo.m_ulCurOutLevel > LOG_LEVEL_ERROR)
    {
        printf("Log Level Set Error!\n");

        return LIB_ERR_PARA_INVALID;
    }

    if (tLogInfo.m_ulCurOutPos > LOG_OUTPOS_NETWORK)
    {
        printf("Log OutPos Set Error!\n");

        return LIB_ERR_PARA_INVALID;
    }    

    if (tLogInfo.m_ulLogFileSize > LOG_FILE_MAX_SIZE)
    {
        printf("Log File Size Set Error!\n");

        return LIB_ERR_PARA_INVALID;
    }

    if (tLogInfo.m_ucFileNamePrefix == NULL)
    {
        printf("Log File Path And Name Error!\n");

        return LIB_ERR_PARA_INVALID;
    }
    
    g_LogInfo = tLogInfo;

    if (g_LogInfo.m_ulCurOutPos == LOG_OUTPOS_FILE)
    {
        g_LogQueueId = MsgQue_Create();
        
        if (pthread_create(&tid, NULL, RecvLogMsg, NULL) != 0)
        {
            printf("Create Log Task Failed!\n");
            return LIB_ERR_TASK_FAILED;
        }

        thread.pid = tid;
        memcpy(thread.desc, pdesc, strlen(pdesc) + 1);
        Sh_AddThreadInfo(thread);
        
        act.sa_handler = SignoDeal;
        sigemptyset(&act.sa_mask);    
        act.sa_flags = 0;
        
        //sigaction(SIGINT, &act, NULL);
        //sigaction(SIGQUIT, &act, NULL);
        //sigaction(SIGSEGV, &act, NULL);
        //sigaction(SIGCHLD, &act, NULL);
    }

    g_LogModuleInitFlag = LOG_OK_INIT_FLAG;
#endif    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Log_SendMsg                                                    */
/*  Input    : %1 -- Log Level                                                */
/*           : %2 -- Log File Name                                            */
/*           : %3 -- Log File Line No                                         */
/*           : %4 -- Var Parameter                                            */
/*  Output   :                                                                */
/*  Return   : void                                                           */
/*  Desc     : Send Log Msg                                                   */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Log_SendMsg(UINT32 ulLogLevel, 
                 UINT8 *ucFileName, 
                 UINT32 ulLineNum,
                 INT8 *fmt,
                 ...)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    TQueMsgBuff tSendMsgBuff;
    UINT8 ucParaStr[MSGQUE_MSG_MAX_LEN];
    va_list args;
    UINT32 ulDataLen;
    UINT32 MsgLen = 0;

    if (g_LogModuleInitFlag != LOG_OK_INIT_FLAG)
    {
        printf("Error:Log Module Is Not Init\n");
        return;
    }
    
    if (NULL == fmt)
    {
        printf("Send Log Msg NULL\n");
        return;
    }

    /* Current LogLevel %ulLogLevel Must More Than System Level, Then
       Log Will Record */
    if (g_LogInfo.m_ulCurOutLevel > ulLogLevel)
    {
        return;
    }
    
    va_start(args, fmt);
    ulDataLen = vsnprintf((INT8 *)ucParaStr, 
                          MSGQUE_MSG_MAX_LEN,
                          (INT8 *)fmt,
                          args);
    va_end(args);

    if (ulDataLen == 0)
    {        
        printf("\n%s:%d log Msg body is NULL!\n", __FILE__, __LINE__);
        return;
    }

    switch (g_LogInfo.m_ulCurOutPos)
    {
        case LOG_OUTPOS_DISPLAY:
        {
            printf("%s:%d %s", ucFileName, ulLineNum, ucParaStr);
            break;
        }
        case LOG_OUTPOS_FILE:
        {
            sprintf(tSendMsgBuff.m_ucMsgTxt, 
                    "%s:%d Msg:%s",
                    ucFileName, 
                    ulLineNum,
                    ucParaStr);
            tSendMsgBuff.m_ulMsgType = LOG_MSGTYPE_DEFAULT;

            MsgLen = strlen(tSendMsgBuff.m_ucMsgTxt) + 1;
            (void)MsgQue_SendMsg(g_LogQueueId, &tSendMsgBuff, MsgLen + 4);
            break;
        }
        case LOG_OUTPOS_NETWORK:
        {
            printf("Not Support.\n");
            break;
        }
        default:
        {
            printf("Error:Log Out Pos Error.\n");
        }
    }
#endif    
    return;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : RecvLogMsg                                                     */
/*  Input    : %1 --  No use Currently                                        */
/*  Output   :                                                                */
/*  Return   : void                                                           */
/*  Desc     : Recv Log Msg From Queue & Write To File                        */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void *RecvLogMsg(void *arg)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    TQueMsgBuff tRecvMsgBuff;
    static UINT32 ulWriteLogTimes = 0;

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    while (1)
    {
        CreateLogFile();
        if (NULL == Fp)
        {
            sleep(1);
            continue;
        }

        memset((void *)&tRecvMsgBuff, 0, sizeof(TQueMsgBuff));
        MsgQue_RecvMsg(g_LogQueueId, &tRecvMsgBuff);
        fwrite(tRecvMsgBuff.m_ucMsgTxt, 
               strlen(tRecvMsgBuff.m_ucMsgTxt) + 1,
               1,
               Fp);

        if (ulWriteLogTimes % g_LogInfo.m_ulFlushFileEveryNLogs == 0)
        {
            fflush(Fp);
        }

        ulWriteLogTimes++;
    }
#endif
    return NULL;
}

/*============================================================================*/
/*  Function : SignoDeal                                                      */
/*  Input    : %1 -- No use Currently                                         */
/*  Output   :                                                                */
/*  Return   : void                                                           */
/*  Desc     : Exception Signal Deal,Clean Resource Before Pragmme Quit       */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void SignoDeal(INT32 sig)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    MsgQue_DestroyById(g_LogQueueId);
    printf("del Queue Id:%d\n", g_LogQueueId);

    fclose(Fp);
    
    exit(0);
#endif
    return;
}

/*============================================================================*/
/*  Function : CreateLogFile                                                  */
/*  Input    : %1 -- void                                                     */
/*  Output   :                                                                */
/*  Return   : void                                                           */
/*  Desc     : Create Log File At Some Rules                                  */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void CreateLogFile(void)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    struct stat statbuf;
    static UINT8 ucLogFile[LOG_FILE_PATH_NAME_MAX_LEN];
    UINT8 ucCurTimeStr[LOG_FILE_PATH_NAME_MAX_LEN];
    
    if (NULL == Fp)
    {
        TC_TimeFormate(ucCurTimeStr, 
                       LOG_FILE_PATH_NAME_MAX_LEN,
                       "%Y-%m-%d-%H-%M-%S");
                       
        sprintf(ucLogFile,
                "%s%s-%s%s",
                g_LogInfo.m_ucLogFilePath,
                g_LogInfo.m_ucFileNamePrefix, 
                ucCurTimeStr,
                ".log");
                
        Fp = fopen(ucLogFile, "a+");
    
        if (NULL == Fp)
        {
            perror("Create Log File Failed\n");
            printf("Log File:%s\n", ucLogFile);
            return;
        }
    }
    
    if (-1 == stat(ucLogFile, &statbuf))
    {
        perror("Get File Attri Failed");
        printf("Log File:%s\n", ucLogFile);
        Fp = NULL;
        return;
    }
    else
    {
        if (statbuf.st_size >= g_LogInfo.m_ulLogFileSize)
        {
            fclose(Fp);
            Fp = NULL;
            return;
        }
    }
#endif
    return;
}

/*============================================================================*/
/*  Function : Log_DestroyQueue                                               */
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
void Log_Destroy(void)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    if (g_LogInfo.m_ulCurOutPos == LOG_OUTPOS_DISPLAY)
    {
        return;
    }

    while (1)
    {
        if (MsgQue_GetCurBytes(g_LogQueueId) == -1
            || MsgQue_GetCurMsgNum(g_LogQueueId) == -1)
        {
            break;
        }

        /* Check Msg Number In Queue, If = 0, then Programme Should Be Over */
        if (MsgQue_GetCurBytes(g_LogQueueId) == 0
            && MsgQue_GetCurMsgNum(g_LogQueueId) == 0)
        {
            fclose(Fp);
            //pthread_cancel(tid);
            MsgQue_DestroyById(g_LogQueueId);
            break;
        }
    }
#endif
    return;
}

/*============================================================================*/
/*  函数名 : Stream_LogPrint                                                  */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.12.5                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void Log_StreamPrint(UINT32 loglevel,UINT8 *PStreamBuff, UINT32 StreamLen)
{
    UINT8 StreamBuff[LOG_MSG_MAX_LEN];
    UINT32 loop;

    if (StreamLen >= LOG_MSG_MAX_LEN)
    {
        printf("Msg Too Long:Len = %d\n", StreamLen);
        return;
    }
    
    for (loop = 0; loop < StreamLen; loop++)
    {
        sprintf(&StreamBuff[loop], "%d ", PStreamBuff[loop]);
    }
    StreamBuff[loop] = '\0';
    
    LOG_PRINT(loglevel, "%s\n", StreamBuff);

    return;
}

