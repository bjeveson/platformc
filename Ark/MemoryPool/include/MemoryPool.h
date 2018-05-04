/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : MemoryPool.h                                                   */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _DYNMEMMAN_H
#define _DYNMEMMAN_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define DMM_LIST_TYPE_MAX_NUM           64
#define DMM_NODE_USED                   0xAAAAAAAA
#define DMM_NODE_UNUSED                 0xBBBBBBBB
#define DMM_NODE_SENTRY                 0xCCCCCCCC

#define DMM_SHOW_INFO_HEAD              1
#define DMM_SHOW_INFO_NODE              1 << 1
#define DMM_SHOW_INFO_BYTES             1 << 2
#define DMM_SHOW_INFO_USED              1 << 3

#define DMM_BACKTRACE_MAX_DEPTH         20
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef struct TMemNode
{
    UINT32 SentryBegin;
    void *Data; /* Flag(4) Data(N) Flag(4) */
    UINT32 UsedFlag;
    void *BackTraceBuff[DMM_BACKTRACE_MAX_DEPTH];
    UINT32 BackTraceDepth;
    UINT32 MemLen;
    struct TMemNode *Next;
    UINT32 SentryEnd;
}TMemNode;

typedef struct
{
    ULong pNodeAddr;
    ULong pNodeBlockAddr;
}THashData;

typedef struct
{
    UINT32 NodeSize;
    UINT32 NodeNum;
    UINT32 CurUsedBlockNum;
    UINT32 RunOutThreshold;
}TMemListInfo;

typedef struct
{
    TMemListInfo MemList;
    TMemNode *HeadNode;
}TListType;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 MemPool_Init(TMemListInfo *pList, UINT32 ListNum);

void *MemPool_Malloc(UINT32 MemSize);

void MemPool_Free(const void *pMem);

void MemPool_DestroyMem(void);

UINT32 MemPool_Show(UINT32 argc, UINT8 *argv[]);

UINT32 MemPool_GetTotalMemorySize(void);

#endif

