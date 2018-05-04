/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : ErrorCode.h                                                    */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _ERRORCODE_H
#define _ERRORCODE_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define LIB_SN               (0x0)
#define LIB_ERRCODE          (0x0 << 8)

#define LIB_ERR_SUCCESS                 (LIB_ERRCODE | 0x00)
#define LIB_ERR_ARRAY_OUT               (LIB_ERRCODE | 0x01)
#define LIB_ERR_PARA_INVALID            (LIB_ERRCODE | 0x02)
#define LIB_ERR_POINTER_NULL            (LIB_ERRCODE | 0x03)
#define LIB_ERR_NETWORK_FAIL            (LIB_ERRCODE | 0x04)
#define LIB_ERR_QUEUE_OPERFAIL          (LIB_ERRCODE | 0x05)
#define LIB_ERR_TASK_FAILED             (LIB_ERRCODE | 0x06)
#define LIB_ERR_FILE_OPEN_FAILED        (LIB_ERRCODE | 0x07)
#define LIB_MEMPAGE_ERROR               (LIB_ERRCODE | 0x08)
#define LIB_ERR_TASK_CREATE_FAILED		(LIB_ERRCODE | 0x09)
#define LIB_ERR_QUEUE_FULL              (LIB_ERRCODE | 0x0A)
#define LIB_ERR_SOCKET_SETATTR_FAILED   (LIB_ERRCODE | 0x0B)
#define LIB_ERR_SOCKET_ACCEPT_FAILED    (LIB_ERRCODE | 0x0C)
#define LIB_ERR_MUTEX_INIT_FAILED       (LIB_ERRCODE | 0x0D)
#define LIB_ERR_MUTEX_LOCK_FAILED       (LIB_ERRCODE | 0x0E)
#define LIB_ERR_MUTEX_UNLOCK_FAILED     (LIB_ERRCODE | 0x0F)
#define LIB_ERR_CONNECT_DB_FAILED       (LIB_ERRCODE | 0x10)
#define LIB_ERR_LIST_INIT_FAILED        (LIB_ERRCODE | 0x11)
#define LIB_ERR_QUEUE_INIT_FAILED       (LIB_ERRCODE | 0x12)
#define LIB_ERR_LIST_EMPTY              (LIB_ERRCODE | 0x13)
#define LIB_ERR_DBPOOL_EXCELSQL         (LIB_ERRCODE | 0x14)

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

#endif

