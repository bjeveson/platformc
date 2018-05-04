/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : OSI.h                                                          */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.10.15                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _OSI_H
#define _OSI_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

#define OSI_LACK_OS_ENVIRMENT "Not Define OS Envirment!"

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#include <sys/mman.h>
#include <sys/time.h>

#define OSI_PROT_R_W_E          (PROT_READ | PROT_WRITE | PROT_EXEC)
#define OSI_PROT_R_W            (PROT_READ | PROT_WRITE)
#define OSI_PROT_E              (PROT_EXEC)

#define OSI_PATH_MAX            PATH_MAX

#elif (COMPILE_ON_PLATFORM == WINDOWS32)
#include <windows.h>
#include <sys/timeb.h>
#include <winsock.h>

#define OSI_PROT_R_W_E          (PAGE_EXECUTE_READWRITE )
#define OSI_PROT_R_W            (PAGE_READWRITE)
#define OSI_PROT_E              (PAGE_EXECUTE)

#define OSI_PATH_MAX            1024

#define close(a) closesocket(a)
#define write(a, b, c) send(a, b, c, 0)
#define read(a, b, c) recv(a, b, c, 0)

union timemark
{
	long long ns100;
	FILETIME ft;
};

#else
#error OSI_LACK_OS_ENVIRMENT
#endif
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 OSI_Protect(void *pBase, 
                   UINT32 MemSize, 
                   ULong NewPermission, 
                   ULong *pOldPermission);

UINT32 OSI_GettimeOfDay(struct timeval *tv, void *p);

UINT32 OSI_CreateThread(OSI_PID_T *tid,
                        OSI_PTHREAD_ATTR *attr,
                        pThreadFunction pfunc,
                        void *arg);

void OSI_SLEEP(ULong MilliSeconds);

#endif

