/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Log.h                                                          */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _LOG_H
#define _LOG_H

#include "OSI.h"

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define LOG_FILE_MAX_SIZE   (10 * 1024 * 1024)  //10M

#define LOG_OUTPOS_DISPLAY  0
#define LOG_OUTPOS_FILE     1
#define LOG_OUTPOS_NETWORK  2

#define LOG_LEVEL_OFF       0
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_NOTICE    2
#define LOG_LEVEL_WARNNING  3
#define LOG_LEVEL_ERROR     4

#define LOG_FILE_PATH_NAME_MAX_LEN      128

#define LOG_MSG_MAX_LEN     1024

#define LOG_MSGTYPE_DEFAULT     1

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#define LOG_PRINT(loglevel, fmt, args...) Log_SendMsg(loglevel,\
                                                      __FILE__, \
                                                      __LINE__,\
                                                      fmt,\
                                                      ##args)

#define LOG_STREAM_PRINT(loglevel,StreamBuff, StreamLen) \
    Log_StreamPrint(loglevel,StreamBuff, StreamLen)

#elif (COMPILE_ON_PLATFORM == WINDOWS32)
#define LOG_PRINT(loglevel,fmt,...) Log_SendMsg(loglevel,\
                                                      __FILE__, \
                                                      __LINE__,\
													  fmt)

#else
#error OSI_LACK_OS_ENVIRMENT
#endif
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef struct
{
    UINT32 m_ulLogFileSize;
    UINT32 m_ulCurOutPos;
    UINT32 m_ulCurOutLevel;
    UINT32 m_ulLogServerIp;
    UINT32 m_ulLogServerPort;
    UINT32 m_ulFlushFileEveryNLogs;
    UINT8 m_ucFileNamePrefix[LOG_FILE_PATH_NAME_MAX_LEN];
    UINT8 m_ucLogFilePath[LOG_FILE_PATH_NAME_MAX_LEN];
}TLogRegInfo;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 Log_Init(TLogRegInfo tLogInfo);

void Log_SendMsg(UINT32 ulLogLevel, 
                 UINT8 *ucFileName, 
                 UINT32 ulLineNum,
                 INT8 *fmt,
                 ...);

void Log_Destroy(void);

void Log_StreamPrint(UINT32 loglevel,UINT8 *PStreamBuff, UINT32 StreamLen);

#endif

