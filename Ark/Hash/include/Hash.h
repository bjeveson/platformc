/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Hash.h                                                         */
/*  Descript :                                                                */
/*  API Pref : Hash_                                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _HASH_H
#define _HASH_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
enum
{
    HASH_RS,     /* 0 -- RSHash   */
    HASH_JS,     /* 1 -- JSHash   */
    HASH_PJW,    /* 2 -- PJWHash  */
    HASH_ELF,    /* 3 -- ELFHash  */
    HASH_AP,     /* 4 -- APHash   */
    HASH_DJB,    /* 5 -- DJBHash  */
    HASH_SDBM,   /* 6 -- SDBMHash */
    HASH_BKDR,   /* 7 -- BKDRHash */
    HASH_MAX     /* 8 -- Hash Function Num */
};

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

typedef UINT32 (*FP_HashFunction)(const void *pStr);

typedef struct TConflictNode
{
    void *pData;
    struct TConflictNode *Prior;
    struct TConflictNode *Next;
}TConflictNode, *PLConflictNode;

typedef struct
{
    TConflictNode *pNode;
    UINT32 ConflictCount;
}THashNode;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

FP_HashFunction Hash_GetFunc(UINT8 ucHashFuncNo);

UINT32 MergeDataToString(const UINT8 *pucKey, UINT8 ucLength, UINT8 *pucBuffer);

THashNode *Hash_InitTable(UINT32 HashTableLength);

UINT32 Hash_AddNodeToTable(THashNode *pHashTable, 
                           UINT32 HashTableLength,
                           void *pHashData,
                           void *pHashInfo,
                           FP_HashFunction KeyFunction);

void *Hash_Search(THashNode *pHashTable, 
                  UINT32 HashTableLength,
                  void *pHashData,
                  FP_HashFunction KeyFunction,
                  List_Compare CompareFunc);

void Hash_Destroy(THashNode *pHash);

#endif

