/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : DynMemMan.c                                                    */
/*  Descript :                                                                */
/*  API Pref : DMM_                                                           */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include "Arch.h"
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#include <unistd.h>
#include <pthread.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <execinfo.h>

#include "VarType.h"
#include "OSI.h"
#include "ErrorCode.h"
#include "MemoryPool.h"
#include "Search.h"
#include "Sort.h"
#include "Shell.h"
#include "List.h"
#include "Hash.h"
#include "Log.h"
#include "Dbug.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/
static UINT32 g_ListTypeNum = 0;

static TListType *pBlockList[DMM_LIST_TYPE_MAX_NUM] = {NULL};

static pthread_mutex_t DynMemMan_Mutex;

static UINT32 HashTableLength = 0;

static THashNode *pHashTable = NULL;

static UINT32 g_TotalMemorySize = 0;
/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static UINT32 ListHeadSort(void *head1, void *head2);

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
static void MonitorMem(void *arg);
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
UINT32 __stdcall MonitorMem(void *arg);
#else
#error OSI_LACK_CPU_TYPE
#endif

static UINT32 CompareMemSize(void *pArrayEle, void *pSize);

static void AddShellDynCmd(void);

static UINT32 FindLastPower(UINT32 Num);

static UINT32 KeyFunction(const void *pStr);

static UINT32 CompareFunc(void *data, void *nextdata);

static UINT32 AddMemPoolNodeToHashTable(UINT32 TotalNodeNum);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : MemPool_Init                                                   */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : DMM Init To Create The whole memory will be used               */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MemPool_Init(TMemListInfo *pList, UINT32 ListNum)
{
    UINT32 ListManLen = sizeof(TListType) * ListNum;
    UINT32 ListNo = 0;
    UINT32 NodeNo = 0;
    TMemNode *pNode = NULL, *pTailNode = NULL;
    static TListType g_pListMan[DMM_LIST_TYPE_MAX_NUM];
    OSI_PID_T tid = 0;
    UINT32 MemBlockNodeNum = 0;
    TThreadInfo Thread;
    UINT8 *pdesc = "Memory Pool Monitor Thread";
    
    if (NULL == pList || ListNum > DMM_LIST_TYPE_MAX_NUM)
    {
        printf("Param Invalid!\n");
        return LIB_ERR_PARA_INVALID;
    }
    
    if (pthread_mutex_init(&DynMemMan_Mutex, NULL) != 0)  
    {  
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return LIB_ERR_MUTEX_INIT_FAILED;
    }

    g_ListTypeNum = ListNum;

    memset((void *)g_pListMan, 0, ListManLen);

    for (ListNo = 0; ListNo < ListNum; ++ListNo, ++pList)
    {
        g_pListMan[ListNo].MemList = *pList;

        for (NodeNo = 0; NodeNo < pList->NodeNum; ++NodeNo)
        {
            pNode = (TMemNode *)malloc(sizeof(TMemNode));
            if (NULL == pNode)
            {
                printf("Brk Failed.Block Roll Back\n");
                MemPool_DestroyMem();
                return LIB_ERR_POINTER_NULL;
            }

            MemBlockNodeNum++;
            
            memset(pNode, 0, sizeof(TMemNode));
            pNode->SentryBegin = DMM_NODE_SENTRY;
            pNode->SentryEnd = DMM_NODE_SENTRY;
            pNode->Data = (void *)malloc(pList->NodeSize + 8);

            g_TotalMemorySize += pList->NodeSize;
            
            if (NULL == pNode->Data)
            {
                printf("Brk Failed.Block Roll Back\n");
                MemPool_DestroyMem();
                return LIB_ERR_POINTER_NULL;
            }

            memset(pNode->Data, 0, pList->NodeSize + 8);
            *(UINT32 *)pNode->Data = DMM_NODE_SENTRY;
            *(UINT32 *)((UINT8 *)pNode->Data + pList->NodeSize + 4) = DMM_NODE_SENTRY;
            
            pNode->Next = NULL;
            pNode->UsedFlag = DMM_NODE_UNUSED;
            
            if (g_pListMan[ListNo].HeadNode == pTailNode
                && pTailNode == NULL)
            {
                g_pListMan[ListNo].HeadNode = pNode;
                pTailNode = pNode;
            }
            else
            {
                pTailNode->Next = pNode;
                pTailNode = pTailNode->Next;
            }
        }

        pTailNode = NULL;
        pBlockList[ListNo] = &g_pListMan[ListNo];
    }

    Sort_Bubble((void **)pBlockList, ListNum, ListHeadSort);

    if (LIB_ERR_SUCCESS != AddMemPoolNodeToHashTable(MemBlockNodeNum))
    {
        printf("Add Node To Hash Table Failed\n");
        return LIB_ERR_LIST_INIT_FAILED;
    }
    
    if (OSI_CreateThread(&tid, NULL, MonitorMem, NULL) != 0)
    {
        printf("Create DMM Task Failed!\n");
        return LIB_ERR_TASK_FAILED;
    }

    Thread.pid = tid;
    memcpy(Thread.desc, pdesc, strlen(pdesc) + 1);
    Sh_AddThreadInfo(Thread);
    
    AddShellDynCmd();

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : AddMemPoolNodeToHashTable                                        */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.10                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 AddMemPoolNodeToHashTable(UINT32 TotalNodeNum)
{
    UINT32 ListNo = 0;
    THashData *pHashData;
    UINT32 ret;    
    TMemNode *pNode = NULL;

    HashTableLength = FindLastPower(TotalNodeNum);
    
    pHashTable = Hash_InitTable(HashTableLength);
    if (NULL == pHashTable)
    {
        printf("Pointer NULL!%s:%d\n", __FILE__, __LINE__);
        return LIB_ERR_POINTER_NULL;
    }
    
    for (ListNo = 0; ListNo < g_ListTypeNum; ++ListNo)
    {
        pNode = pBlockList[ListNo]->HeadNode;
        while (NULL != pNode)
        {
            pHashData = (THashData *)malloc(sizeof(THashData));
            pHashData->pNodeAddr = (ULong)pNode;
            pHashData->pNodeBlockAddr = (ULong)pNode->Data;

            ret = Hash_AddNodeToTable(pHashTable, 
                                      HashTableLength,
                                      pNode->Data,
                                      pHashData, 
                                      KeyFunction);
            if (LIB_ERR_SUCCESS != ret)
            {
                LOG_PRINT(LOG_LEVEL_ERROR, "Add Hashe Node To Table Error\n");
                return ret;
            }

            pNode = pNode->Next;
        }
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : DMM_ShowAll                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Sholl All The MemInfo Currently                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MemPool_Show(UINT32 argc, UINT8 *argv[])
{
    UINT32 ListNo = 0;
    UINT32 NodeNo = 0;
    TMemNode *pNode = NULL;
    UINT32 ListBytes = 0, AllListBytes = 0;
    UINT32 ShowInfoMask = atoi(argv[1]);

    if (ShowInfoMask & DMM_SHOW_INFO_USED)
    {
        printf("ListNo  NodeSize     TotalNum     UsedNum   ThreasHold\n");
    }
    for (ListNo = 0; ListNo < g_ListTypeNum; ++ListNo)
    {
        printf("======================================================\n");
        if (ShowInfoMask & DMM_SHOW_INFO_HEAD)
        {
            printf("Memory List %d:\n", ListNo);
            printf("\tMemory Block Size:0x%x\n" , pBlockList[ListNo]->MemList.NodeSize);
            printf("\tMemory Block Num :%d\n", pBlockList[ListNo]->MemList.NodeNum);
            printf("\tList Head Addr   :%p\n", pBlockList[ListNo]->HeadNode);
        }

        ListBytes = pBlockList[ListNo]->MemList.NodeNum * pBlockList[ListNo]->MemList.NodeSize;
        AllListBytes += ListBytes;
        
        if (ShowInfoMask & DMM_SHOW_INFO_BYTES)
        {
            printf("\tValid Mem Sapce  :0x%x\n", ListBytes);
        }
        
        for (pNode = pBlockList[ListNo]->HeadNode; 
             pNode != NULL; 
             pNode = pNode->Next, ++NodeNo)
        {
            if (ShowInfoMask & DMM_SHOW_INFO_NODE)
            {
                printf("\t\t*=>Block %4d Info:\n", NodeNo);
                printf("\t\t\tBlock Addr      :%p\n", pNode->Data);
                printf("\t\t\tBlock Flag      :%d\n", pNode->UsedFlag);

                if (NULL == pNode->Next)
                {
                    printf("\t\t\tNext Block Addr :%p\n", NULL);
                }
                else
                {
                    printf("\t\t\tNext Block Addr:%p\n", pNode->Next->Data);
                }
            }
        }

        if (NodeNo != pBlockList[ListNo]->MemList.NodeNum)
        {
            printf("DMM List %d Node Num %d Error\n", ListNo, NodeNo);
        }

        NodeNo = 0;

        if (ShowInfoMask & DMM_SHOW_INFO_USED)
        {
            printf("%-10d 0x%-10x %-10d %-10d %d\n", 
                   ListNo, 
                   pBlockList[ListNo]->MemList.NodeSize, 
                   pBlockList[ListNo]->MemList.NodeNum,
                   pBlockList[ListNo]->MemList.CurUsedBlockNum,
                   pBlockList[ListNo]->MemList.RunOutThreshold);
        }

    }

    if (ShowInfoMask & DMM_SHOW_INFO_BYTES)
    {
        printf("====================================\n");
        printf("All Valid Memory Sapce:0x%x\n\n", AllListBytes);
    }
            
	return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : MemPool_Leak                                                     */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.8                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 MemPool_Leak(UINT32 argc, UINT8 *argv[])
{
    UINT32 ListNo = 0;
    TMemNode *pNode = NULL;
    UINT32 loop = 0;
    UINT32 Count = 0;
    INT8 **strings;
    
    for (ListNo = 0; ListNo < g_ListTypeNum; ++ListNo)
    {
        pNode = pBlockList[ListNo]->HeadNode;

        while (NULL != pNode)
        {
            if (pNode->BackTraceDepth != 0)
            {
                strings = backtrace_symbols(pNode->BackTraceBuff, pNode->BackTraceDepth);
                
                printf("---------------%d BlockSize:0x%x-------------\n", 
                       ++Count, (UINT32)pNode->MemLen);
                for (loop = 0; loop < pNode->BackTraceDepth; loop++)
                {
                    printf("%d:%s\n", loop, strings[loop]);
                }

                free(strings);
            }

            pNode = pNode->Next;
        }
    }

    return 0;
}

/*============================================================================*/
/*  Function : MemPool_Malloc                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void *MemPool_Malloc(UINT32 MemSize)
{
    UINT32 Index, ret;
    TMemNode *pNode = NULL;
    THashData *pHashData;
    void *pRetDataAddr = NULL;

    if ((ret = pthread_mutex_lock(&DynMemMan_Mutex)) != 0)
    {
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return NULL;
    }    
    
    Index = Search_Binary((void *)pBlockList, 
                         (void *)&MemSize, 
                         g_ListTypeNum, 
                         CompareMemSize);

    if (Index >= g_ListTypeNum)
    {
        printf("Index = %d MemSize = %d Too Large!%s:%d\n", Index , MemSize, __FILE__, __LINE__);
        return NULL;
    }

    pNode = pBlockList[Index]->HeadNode;    

    while (NULL != pNode)
    {
        if (DMM_NODE_UNUSED == pNode->UsedFlag)
        {
            pNode->UsedFlag = DMM_NODE_USED;
            
            pthread_mutex_unlock(&DynMemMan_Mutex);
            pRetDataAddr = (UINT8 *)pNode->Data + 4;
            pNode->BackTraceDepth = backtrace(pNode->BackTraceBuff, DMM_BACKTRACE_MAX_DEPTH);
            pNode->MemLen = MemSize;
            DBUG_PRINT(DEBUG_OFF, "Malloc:%p Stack Depth:%d", pRetDataAddr, pNode->BackTraceDepth);
            
            return pRetDataAddr;
        }
        else if (pNode->UsedFlag == DMM_NODE_USED)
        {
            ;
        }
        else
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "The Memory Pool Block Be Destroied\n");
        }
        
        pNode = pNode->Next;
    }

    pthread_mutex_unlock(&DynMemMan_Mutex);

    printf("No Malloc Can Be Used\n");
    
    return NULL;
}

/*============================================================================*/
/*  Function : MemPool_Free                                                   */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void MemPool_Free(const void *pMem)
{
    UINT32 index, ret;
    TMemNode *pNode = NULL;
    THashData *pAddr;
    THashData HashData;

    if (NULL == pMem)
    {
        printf("Pointer NULL\n");
        return;
    }      

    if ((ret = pthread_mutex_lock(&DynMemMan_Mutex)) != 0)
    {
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return;
    }    

    pAddr = Hash_Search(pHashTable, 
                        HashTableLength,
                        (void *)((UINT8 *)pMem - 4), 
                        KeyFunction, 
                        CompareFunc);

    pNode = (TMemNode *)pAddr->pNodeAddr;

    if (pNode->UsedFlag == DMM_NODE_UNUSED)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "This Block Is free already\n");
    }
    else if (pNode->UsedFlag == DMM_NODE_USED)
    {
        pNode->UsedFlag = DMM_NODE_UNUSED;
        pNode->BackTraceDepth = 0;
        DBUG_PRINT(DEBUG_OFF, "Free:Para = %p Find:%p\n", pMem, pNode->Data + 4);
    }
    else
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "This Block Is Be Destroied\n");
    }
    
    pthread_mutex_unlock(&DynMemMan_Mutex);
    
    return;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : ListHeadSort                                                   */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : List Head Sort                                                 */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 ListHeadSort(void *head1, void *head2)
{
    TListType *pListHead1 = head1, *pListHead2 = head2;

    if (NULL == head1 || NULL == head2)
    {
        printf("Null Pointer!\n");
        return COMPARE_EQUAL;
    }
    
    if (pListHead1->MemList.NodeSize > pListHead2->MemList.NodeSize)
    {
        return COMPARE_MORE;
    }
    else if (pListHead1->MemList.NodeSize < pListHead2->MemList.NodeSize)
    {
        return COMPARE_LESS;
    }
    else
    {
        return COMPARE_EQUAL;
    }
}

/*============================================================================*/
/*  Function : BlkRollBack                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : RollBack For malloc Failed                                     */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void MemPool_DestroyMem(void)
{
    UINT32 Index = 0;
    TMemNode *pNode = NULL, *pNextNode = NULL;

    for (Index = 0; Index < g_ListTypeNum; ++Index)
    {
        if (pBlockList[Index]->HeadNode != NULL)
        {
            pNode = pBlockList[Index]->HeadNode;

            while (pNode != NULL)
            {
                if (pNode->Data != NULL)
                {
                    free(pNode->Data);
                    pNode->Data = NULL;
                }

                pNextNode = pNode->Next;
                if (NULL != pNode)
                {
                    free(pNode);
                    pNode = NULL;
                }
                
                pNode = pNextNode;
            }

            pBlockList[Index]->HeadNode = NULL;
        }
    }
    
    return;
}

/*============================================================================*/
/*  Function : MonitorMem                                                     */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
static void MonitorMem(void *arg)
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
UINT32 __stdcall MonitorMem(void *arg)
#else
#error OSI_LACK_CPU_TYPE
#endif
{
    UINT32 index;
    TMemNode *pNode = NULL;
    INT32 ret;

    while (1)
    {
        if ((ret = pthread_mutex_lock(&DynMemMan_Mutex)) != 0)
        {
            perror("");
            printf("%s:%d\n", __FILE__, __LINE__);
            continue;
        }    

        for (index = 0; index < g_ListTypeNum; ++index)
        {
            pBlockList[index]->MemList.CurUsedBlockNum = 0;
        }        

        for (index = 0; index < g_ListTypeNum; ++index)
        {
            pNode = pBlockList[index]->HeadNode;
            while (NULL != pNode)
            {
                if (pNode->UsedFlag == DMM_NODE_USED)
                {
                    pBlockList[index]->MemList.CurUsedBlockNum++;
                }
                
                if (DMM_NODE_SENTRY != pNode->SentryBegin
                    || DMM_NODE_SENTRY != pNode->SentryEnd
                    || DMM_NODE_SENTRY != *(UINT32 *)pNode->Data
                    || DMM_NODE_SENTRY != *(UINT32 *)((UINT8 *)pNode->Data + pBlockList[index]->MemList.NodeSize+ 4))
                {
                    LOG_PRINT(LOG_LEVEL_WARNNING, "Memory Pool Ctl Info Error Node Addr:%p\n", pNode);
                    break;
                }

                pNode = pNode->Next;
            }
            
            if ((UINT32)(pBlockList[index]->MemList.CurUsedBlockNum / pBlockList[index]->MemList.NodeNum)
                 >= pBlockList[index]->MemList.RunOutThreshold)
            {
                printf("Warning:Pool %d Total:%d,have used %d ThresHold:%d\n",
                       index, 
                       pBlockList[index]->MemList.NodeNum, 
                       pBlockList[index]->MemList.CurUsedBlockNum,
                       pBlockList[index]->MemList.RunOutThreshold);
            }
        }

        pthread_mutex_unlock(&DynMemMan_Mutex);
        
        OSI_SLEEP(1);
    }

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)    
    return;
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
    return 0;
#else
#error OSI_LACK_CPU_TYPE
#endif    
}

/*============================================================================*/
/*  Function : CompareMemSize                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 CompareMemSize(void *pArrayEle, void *pSize)
{
    TListType *pListType = (TListType *)pArrayEle;
    UINT32 MemSize = *(UINT32 *)pSize;
    UINT32 ArrayEleMemSize = pListType->MemList.NodeSize;

    if (ArrayEleMemSize > MemSize)
    {
        return COMPARE_MORE;
    }
    else if (ArrayEleMemSize < MemSize)
    {
        return COMPARE_LESS;
    }
    else
    {
        return COMPARE_EQUAL;
    }
}

/*============================================================================*/
/*  Function : AddShellCmd                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void AddShellDynCmd(void)
{
    TShell ShellCmd;

    ShellCmd.MaxParaNum = 1;
    ShellCmd.pfunc = MemPool_Show;
    memcpy(ShellCmd.ucCmdDesc, 
           "display current dmm info usage:dmm [1/2/4/8]", 
           SHELL_CMD_STR_MAX_LEN + SHELL_CMD_STR_MAX_LEN);
    memcpy(ShellCmd.ucCmdstr, "dmm", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);

    ShellCmd.MaxParaNum = 0;
    ShellCmd.pfunc = MemPool_Leak;
    memcpy(ShellCmd.ucCmdDesc, 
           "display leak memory function call stack:leak", 
           SHELL_CMD_STR_MAX_LEN + SHELL_CMD_STR_MAX_LEN);
    memcpy(ShellCmd.ucCmdstr, "leak", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);    

    return;
}

/*============================================================================*/
/*  函数名 : FindLastPower                                                    */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.5                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 FindLastPower(UINT32 Num)
{
    UINT32 Power = 1;
    
    while (Power < Num)
    {
        Power <<= 1;
    }

    return Power;
}

/*============================================================================*/
/*  函数名 : KeyFunction                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 KeyFunction(const void *pStr)
{
    //THashData *pHashData = (THashData *)pStr;
    UINT32 Num = (ULong)pStr;//pHashData->pNodeBlockAddr;

    return (Num >> 4) & (HashTableLength - 1);
}

/*============================================================================*/
/*  函数名 : CompareFunc                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.4                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 CompareFunc(void *pHashData, void *pHashKey)
{
    THashData *pHashData1 = (THashData *)pHashData;
    UINT32 Data1 = pHashData1->pNodeBlockAddr;
    UINT32 Data2 = (ULong)pHashKey;

    if (Data1 == Data2)
    {
        return LIST_DATA_EQUAL;
    }
    else
    {
        return LIST_DATA_NOT_EQUAL;
    }
}

/*============================================================================*/
/*  函数名 : MemPool_GetTotalMemorySize                                       */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.10                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 MemPool_GetTotalMemorySize(void)
{
    return g_TotalMemorySize;
}

