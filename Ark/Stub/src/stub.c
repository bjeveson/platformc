/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : stub.c                                                         */
/*  Descript : Dynamic Stub,Support Linux And Win32 Envirment                 */
/*  API Pref : Stub                                                           */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arch.h"
#include "ErrorCode.h"
#include "VarType.h"
#include "OSI.h"
#include "stub.h"
#include "MemoryPool.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static TStubinfo *s_pStubInfoList = NULL;

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static UINT32 RemoveStubNode(TStubinfo tStubInfo);

static UINT32 ApendStubNode(TStubinfo tStubInfo);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Stub_Install                                                   */
/*  Input    : %1 -- Dest Function Addr                                       */
/*           : %2 -- Stub Function Addr                                       */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Install Stub                                                   */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Stub_Install(void *dest_func, void *stub_func)
{
    UINT8 *page = NULL;
    UINT32 ulRet = LIB_ERR_SUCCESS;
    TStubinfo tStubInfo;
	ULong ulOldProtect;
	ULong ulNewProtect;
	UINT32 ulMemSize = 1024;
	
    page = (UINT8 *)((ULong)dest_func & 0xFFFFF000);
    ulNewProtect = OSI_PROT_R_W_E;
    
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL)
    (void)(page);
    /* Modify Page Property At Kernel State */
#else
    ulRet = OSI_Protect(page, ulMemSize, ulNewProtect, &ulOldProtect);
    if (LIB_ERR_SUCCESS != ulRet)
    {
        perror("set page privilege failed!\n");
        return LIB_ERR_PARA_INVALID;
    }
#endif

    tStubInfo.dest_func = dest_func;
    tStubInfo.stub_func = stub_func;
    
    ulRet = ApendStubNode(tStubInfo);
    if (LIB_ERR_SUCCESS != ulRet)
    {
       return LIB_MEMPAGE_ERROR;
    }

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL)
    /* Do Not Modify Page Property At Kernel State */
#else
    ulRet = OSI_Protect(page, ulMemSize, ulNewProtect, &ulOldProtect);
    if (LIB_ERR_SUCCESS != ulRet)
    {
        perror("set page privilege failed!\n");
        return LIB_ERR_POINTER_NULL;
    }   
#endif

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Stub_Remove                                                    */
/*  Input    : %1 -- Dest Function Addr                                       */
/*           : %2 -- Stub Function Addr                                       */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Remove Stub                                                    */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Stub_Remove(void *dest_func, void *stub_func)
{
    UINT8 *page = NULL;
    UINT32 ulRet = LIB_ERR_SUCCESS;
    TStubinfo tStubInfo;
	ULong ulOldProtect;
	ULong ulNewProtect;
	UINT32 ulMemSize = 1024;

    page = (UINT8 *)((ULong)dest_func & 0xFFFFF000);

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL)
    (void)(page);
    /* Do Not Modify Page Property At Kernel State */
#else
    ulNewProtect = OSI_PROT_R_W_E;
    ulRet = OSI_Protect(page, ulMemSize, ulNewProtect, &ulOldProtect);
    if (LIB_ERR_SUCCESS != ulRet)
    {
        perror("set page privilege failed!\n");
        return LIB_ERR_PARA_INVALID;
    } 
#endif

    tStubInfo.dest_func = dest_func;
    tStubInfo.stub_func = stub_func;

    ulRet = RemoveStubNode(tStubInfo);
    if (LIB_ERR_SUCCESS != ulRet)
    {
       return LIB_ERR_POINTER_NULL;
    }

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL)
    /* Do Not Modify Page Property At Kernel State */
#else
    ulNewProtect = OSI_PROT_E;
    ulRet = OSI_Protect(page, 1024, ulNewProtect, &ulOldProtect);
    if (LIB_ERR_SUCCESS != ulRet)
    {
        perror("set page privilege failed!\n");
        return LIB_ERR_PARA_INVALID;
    }      
#endif

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : ApendStubNode                                                  */
/*  Input    : %1 -- Stub Info                                                */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Add Stub Info To List                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 ApendStubNode(TStubinfo tStubInfo)
{
    TStubinfo *pStubNode = NULL, *pPreStubNuode = NULL;
    UINT8 ucCommand[JUMP_INSTRUCTION_LEN];
    UINT8 *pucCmd = NULL;
    INT32 ulOffset = 0;
    
    (void)memset((void *)ucCommand, 0, JUMP_INSTRUCTION_LEN);
    pucCmd = ucCommand;
    
#if (CPU_STRUCTURE == CPU_X86)
    ulOffset = (ULong)tStubInfo.stub_func 
                - (ULong)tStubInfo.dest_func - JUMP_INSTRUCTION_LEN;
    *pucCmd = 0xE9;
    (void)memcpy((void *)(pucCmd + 1), (void *)&ulOffset, 4);
#elif (CPU_STRUCTURE == CPU_POWERPC603)
    ulOffset = (INT32)(tStubInfo.stub_func - tStubInfo.dest_func);
    if (ulOffset > 0)
    {
        *pucCmd = 0x48;
    }
    else 
    {
        *pucCmd = 0x4b;
    }
    
    *(pucCmd + 1) = Bit_GetNByte((UINT32)ulOffset, 3);
    *(pucCmd + 2) = Bit_GetNByte((UINT32)ulOffset, 2);
    *(pucCmd + 3) = Bit_GetNByte((UINT32)ulOffset, 1);
#else
#error OSI_LACK_CPU_TYPE
#endif

    if (NULL == s_pStubInfoList)
    {
        pStubNode = (TStubinfo *)MemPool_Malloc(sizeof(TStubinfo));
        if (NULL == pStubNode)
        {
            printf("Memory Alloc Failed.\n");
            return LIB_ERR_POINTER_NULL;
        }

        s_pStubInfoList = pStubNode;
        
        pStubNode->next = NULL;
        pStubNode->stub_func = tStubInfo.stub_func;
        pStubNode->dest_func = tStubInfo.dest_func;
        (void)memcpy((void *)pStubNode->ucContext, 
                     (void *)tStubInfo.dest_func,
                     JUMP_INSTRUCTION_LEN);
        (void)memcpy((void *)tStubInfo.dest_func, 
                     (void *)ucCommand, 
                     JUMP_INSTRUCTION_LEN);
    }
    else
    {
        pStubNode = s_pStubInfoList;
        while (NULL != pStubNode)
        {
            if (pStubNode->dest_func == tStubInfo.dest_func)
            {
                break;
            }

            pPreStubNuode = pStubNode;
            pStubNode = (TStubinfo *)(pStubNode->next);/*lint !e740*/
        }

        if (NULL != pStubNode)
        {
            return LIB_ERR_POINTER_NULL;
        }

        pStubNode = (TStubinfo *)MemPool_Malloc(sizeof(TStubinfo));
        if (NULL == pStubNode)
        {
            return LIB_ERR_POINTER_NULL;
        }

        if (NULL == pPreStubNuode)
        {
            MemPool_Free(pStubNode);
            pStubNode = NULL;
            return LIB_ERR_POINTER_NULL;
        }
        
        pPreStubNuode->next = (struct TStubinfo *)pStubNode;/*lint !e740*/
        pStubNode->next = NULL;
        pStubNode->stub_func = tStubInfo.stub_func;
        pStubNode->dest_func = tStubInfo.dest_func;
        (void)memcpy((void *)pStubNode->ucContext, 
                     (void *)tStubInfo.dest_func, 
                     JUMP_INSTRUCTION_LEN);
        (void)memcpy((void *)tStubInfo.dest_func, 
                     (void *)ucCommand,
                     JUMP_INSTRUCTION_LEN);
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : RemoveStubNode                                                 */
/*  Input    : %1 -- Stub Info                                                */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Remove Stub Info From List                                     */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 RemoveStubNode(TStubinfo tStubInfo)
{
    TStubinfo *pStubNode, *pPreStubNuode;

    if (NULL == s_pStubInfoList)
    {
        printf("List Empty!\n");
        return LIB_ERR_POINTER_NULL;
    }

    pStubNode = s_pStubInfoList;
    pPreStubNuode = s_pStubInfoList;

    while (NULL != pStubNode)
    {
        if ((pStubNode->dest_func == tStubInfo.dest_func)
            && (pStubNode->stub_func == tStubInfo.stub_func))
        {
            break;
        }

        pPreStubNuode = pStubNode;
        pStubNode = (TStubinfo *)pStubNode->next;/*lint !e740*/
    }

    if (NULL == pStubNode)
    {
        printf("Stub Not Exist.\n");
        return LIB_ERR_POINTER_NULL;
    }

    if (s_pStubInfoList == pStubNode)   /*  First Node   */
    {
        s_pStubInfoList = (TStubinfo *)pStubNode->next;/*lint !e740*/
    }
    else
    {
        pPreStubNuode->next = pStubNode->next;
        pStubNode->next = NULL;
    }
    
    (void)memcpy((void *)pStubNode->dest_func, 
                 (void *)pStubNode->ucContext,
                 JUMP_INSTRUCTION_LEN);

    MemPool_Free(pStubNode);

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : PrintStubInfo                                                  */
/*  Input    : %1 -- void                                                     */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Print Stub Info All List                                       */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void PrintStubInfo(void)
{
    TStubinfo *pStubNode = NULL;
    
    pStubNode = s_pStubInfoList;

    if (NULL == pStubNode)
    {
        printf("List Empty\n");
    }
    
    while (NULL != pStubNode)
    {
        printf("dest_func:%p\tstub_func:%p\n", pStubNode->dest_func,
                                                   pStubNode->stub_func);      
        pStubNode = (TStubinfo *)pStubNode->next;/*lint !e740*/
    }

    printf("\n");
    
    return;
}

