/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : OSI.c                                                          */
/*  Descript :                                                                */
/*  API Pref : OSI_                                                           */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.10.15                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include "ErrorCode.h"

#include "Arch.h"
#include "VarType.h"
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)

#elif (COMPILE_ON_PLATFORM == WINDOWS32)
#include <minwindef.h>
#include <process.h>
#else
#error OSI_LACK_OS_ENVIRMENT
#endif
#include "OSI.h"

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
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
/*  Function : OSI_Protect                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : Hu Yongfu                                                      */
/*  Version  : Sword                                                          */
/*  Date     : 2013.10.18                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 OSI_Protect(void *pBase, 
                   UINT32 MemSize, 
                   ULong NewPermission, 
                   ULong *pOldPermission)
{
    UINT32 ulRet = LIB_ERR_SUCCESS;
    
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    (void)pOldPermission;
    ulRet = mprotect(pBase, MemSize, NewPermission);
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
    ulRet = VirtualProtect(pBase, MemSize, NewPermission, pOldPermission);
#else
#error OSI_LACK_OS_ENVIRMENT
#endif
    return ulRet;
}

/*============================================================================*/
/*  Function : OSI_GettimeOfDay                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : Hu Yongfu                                                      */
/*  Version  : Sword                                                          */
/*  Date     : 2013.10.18                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 OSI_GettimeOfDay(struct timeval *tv, void *p)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
	(void)p;
    gettimeofday(tv, (struct timezone *)0);
#elif (COMPILE_ON_PLATFORM == WINDOWS32)

    union timemark now;
    GetSystemTimeAsFileTime(&now.ft);
    tv->tv_usec = (long) ((now.ns100 / 10LL) % 1000000LL);
    tv->tv_sec = (long) ((now.ns100 - 116444736000000000LL) / 10000000LL);
#else
#error OSI_LACK_OS_ENVIRMENT
#endif
    return (0);
}

/*============================================================================*/
/*  Function : OSI_CreateThread                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : Hu Yongfu                                                      */
/*  Version  : Sword                                                          */
/*  Date     : 2013.10.18                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 OSI_CreateThread(OSI_PID_T *tid,
                               OSI_PTHREAD_ATTR *attr,
                               pThreadFunction pfunc,
                               void *arg)
{
    UINT32 ulRet = LIB_ERR_SUCCESS;
	OSI_THREADRET iRet = 0;
    
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    iRet = pthread_create(tid, attr, pfunc, arg);
	ulRet = (UINT32)iRet;
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
    iRet = (HANDLE)_beginthreadex(attr,
                                  0,            // stack size  
                                  pfunc,  
                                  arg,           // arg list  
                                  0,    // so we can later call ResumeThread()  
                                  tid);  
	if (iRet == 0)
	{
		ulRet = LIB_ERR_TASK_CREATE_FAILED;
	}
	else
	{
		ulRet = LIB_ERR_SUCCESS;
	}
#else
#error OSI_LACK_OS_ENVIRMENT
#endif
    return ulRet;
}

/*============================================================================*/
/*  Function : OSI_CreateThread                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : Hu Yongfu                                                      */
/*  Version  : Sword                                                          */
/*  Date     : 2013.10.18                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void OSI_SLEEP(ULong MilliSeconds)
{
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    sleep(MilliSeconds);
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
    Sleep(MilliSeconds);
#else
#error OSI_LACK_OS_ENVIRMENT
#endif
}
/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


