/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Hash.c                                                         */
/*  Descript : Hash                                                           */
/*  API Pref : Hash_                                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
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
#include "ErrorCode.h"
#include "Arch.h"
#include "VarType.h"

#include "Log.h"
#include "List.h"
#include "Hash.h"
#include "MemoryPool.h"
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
static UINT32 RSHash(const void *pData);

static UINT32 JSHash(const void *pData);

static UINT32 PJWHash(const void *pData);

static UINT32 ELFHash(const void *pData);

static UINT32 APHash(const void *pData);

static UINT32 DJBHash(const void *pData);

static UINT32 SDBMHash(const void *pData);

static UINT32 BKDRHash(const void *pData);

static FP_HashFunction g_HashFunc[] = {
                                        RSHash,  
                                        JSHash,
                                        PJWHash, 
                                        ELFHash,
                                        BKDRHash, 
                                        SDBMHash, 
                                        DJBHash, 
                                        APHash,
                                      };

static UINT32 CreateList(TConflictNode **list);

static UINT32 InsertNodeToListAtTail(TConflictNode *list, void *e);

static UINT32 FindCurNodeByData(TConflictNode *list, 
                                void *Cur_Key,
                                void **Cur_e, 
                                List_Compare fp_Compare);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : MergeDataToString                                              */
/*  Input    : %1 -- Pointer Of Key Word                                      */
/*             %2 -- Length Of Key Word                                       */
/*  Output   : %3 -- Key Word Be Deleted Char 0                               */
/*  Return   : Error Code                                                     */
/*  Desc     : Delete Char 0 In Key Word                                      */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 MergeDataToString(const UINT8 *pucKey, UINT8 ucLength, UINT8 *pucBuffer)
{
    UINT8 i = 0 ;
    UINT8 ucCount = 0 ;
    
    if (NULL == pucKey || NULL == pucBuffer)
    {
        return LIB_ERR_POINTER_NULL;
    }

    for (i = 0; i < ucLength; i++)
    {
        if (0x00 != *pucKey)
        {
            *(pucBuffer + ucCount) = *pucKey;
            ucCount++;
        }
        pucKey++;
    }
    
    *(pucBuffer + ucCount) = '\0';

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Hash Function Index                                      */
/*  Output   :                                                                */
/*  Return   : Hsh Function Pointer                                           */
/*  Desc     : R.S Hash Function                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
FP_HashFunction Hash_GetFunc(UINT8 ucHashFuncNo)
{
    return g_HashFunc[ucHashFuncNo];
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : R.S Hash Function                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 RSHash(const void *pData)
{
    UINT32 b = 378551;
    UINT32 a = 63689;
    UINT32 hash = 0;
    UINT8 *str = (UINT8 *)pData;
    
    while (0x00 != *str)
    {
        hash = hash * a + (*str);
        a *= b;
        str++;
    } 

    return (hash & 0x7FFFFFFF);
} 

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : J.S Hash Function                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 JSHash(const void *pData)
{
    UINT32 hash = 1315423911;
    UINT8 *str = (UINT8 *)pData;

    while (0x00 != *str)
    {
        hash ^= ((hash<<5) + (*str) + (hash>>2));
        str++;
    } 
 
    return (hash & 0x7FFFFFFF);
}

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : P.J.W Hash Function                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 PJWHash(const void *pData)
{
     UINT32 BitsInUnignedInt = (UINT32)(sizeof(UINT32) * 8);
     UINT32 ThreeQuarters = (UINT32)((BitsInUnignedInt * 3) / 4);
     UINT32 OneEighth = (UINT32)(BitsInUnignedInt / 8);
     UINT32 HighBits = (UINT32)(0xFFFFFFFF) << (BitsInUnignedInt - OneEighth);
     UINT32 hash = 0;
     UINT32 test = 0;
     UINT8 *str = (UINT8 *)pData;

     while (0x00 != *str)
     {
         hash = (hash << OneEighth) + (*str);
         if ((test = hash & HighBits) != 0)
         {
             hash = ((hash ^ (test >> ThreeQuarters)) & ( ~HighBits));
         }
         str++;
     } 
 
     return (hash & 0x7FFFFFFF);
}

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : E.L.F Hash Function                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 ELFHash(const void *pData)
{
    UINT32 hash = 0;
    UINT32 x = 0;
    UINT8 *str = (UINT8 *)pData;

    while (0x00 != *str)
    {
        hash = (hash << 4) + (*str);
        if ((x = hash & 0xF0000000L) != 0)
        {
             hash ^= (x >> 24);
             hash &= ~x;
        }
        str++;
    } 
 
    return (hash & 0x7FFFFFFF);
}

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : A.P Hash Function                                              */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 APHash(const void *pData)
{
    UINT32 hash = 0;
    UINT32 i = 0;
    UINT8 *str = (UINT8 *)pData;
    
    for (i = 0; *str; i++)
    {
        if((i & 1) == 0)
        {
            hash ^= ((hash<<7) ^ (*str) ^ (hash>>3));
        } 
        else 
        {
            hash ^= (~((hash<<11) ^ (*str) ^ (hash>>5)));
        }
        str++;
    } 
    
     return (hash & 0x7FFFFFFF);
} 

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : D.J.B Hash Function                                            */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 DJBHash(const void *pData)
{
    UINT32 hash = 5381;
    UINT8 *str = (UINT8 *)pData;

    while (0x00 != *str)
    {
        hash += (hash << 5 ) + (*str);
        str++;
    } 

    return (hash & 0x7FFFFFFF);
}

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : S.D.B.M Hash Function                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 SDBMHash(const void *pData)
{
    UINT32 hash = 0;
    UINT8 *str = (UINT8 *)pData;

    while (0x00 != *str)
    {
        hash = (*str) + (hash << 6) + (hash << 16) - hash;
        str++;
    } 

    return (hash & 0x7FFFFFFF);
}

/*============================================================================*/
/*  Function : RSHash                                                         */
/*  Input    : %1 -- Key Str                                                  */
/*  Output   :                                                                */
/*  Return   : Key Word                                                       */
/*  Desc     : B.K.D.R Hash Function                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.8.16                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 BKDRHash(const void *pData)
{
    UINT32 seed = 131;  //  31 131 1313 13131 131313 etc.. 
    UINT32 hash = 0;
    UINT8 *str = (UINT8 *)pData;

    while (0x00 != *str)
    {
        hash = hash * seed + (*str);
        str++;
    } 

    return (hash & 0x7FFFFFFF);
}

/*============================================================================*/
/*  函数名 : InitHashTable                                                    */
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
THashNode *Hash_InitTable(UINT32 HashTableLength)
{
    THashNode *pHashTable = NULL;

    pHashTable = (THashNode *)malloc(sizeof(THashNode) * HashTableLength);

    if (NULL == pHashTable)
    {
        return NULL;
    }
    
    memset(pHashTable, 0, sizeof(THashNode) * HashTableLength);
    
    return pHashTable;
}

/*============================================================================*/
/*  函数名 : AddNodeToHashTable                                               */
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
UINT32 Hash_AddNodeToTable(THashNode *pHashTable, 
                           UINT32 HashTableLength,
                           void *pHashDataToKey,
                           void *pHashInfo,
                           FP_HashFunction KeyFunction)
{
    UINT32 loop = 0;
    UINT32 HashKey;
    UINT32 Ret;
    
    if (NULL == pHashTable || NULL == KeyFunction)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Pointer NULL\n");
        return LIB_ERR_PARA_INVALID;
    }

    HashKey = KeyFunction(pHashDataToKey);

    if (HashKey > HashTableLength)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Hash Key Error\n");
        return LIB_ERR_ARRAY_OUT;
    }
    
    if (pHashTable[HashKey].ConflictCount == 0) //here is no data to stored
    {
        Ret = CreateList(&pHashTable[HashKey].pNode);
        if (LIB_ERR_SUCCESS != Ret)
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "Create List Failed\n");
            return Ret;
        }

        // used list head as data node
        pHashTable[HashKey].pNode->pData = pHashInfo;
        pHashTable[HashKey].ConflictCount = 1;
    }
    else  //Conflict > 1 Counthave conflict ,insert to conflict list
    {
        Ret = InsertNodeToListAtTail(pHashTable[HashKey].pNode, pHashInfo);
        if (Ret != LIB_ERR_SUCCESS)
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "Add List Node Failed\n");
            return Ret;
        }

        pHashTable[HashKey].ConflictCount++;
    }
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : Hash_Search                                                      */
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
void *Hash_Search(THashNode *pHashTable, 
                  UINT32 HashTableLength,
                  void *pHashDataToKey,
                  FP_HashFunction KeyFunction,
                  List_Compare CompareFunc)
{
    UINT32 HashKey, Ret;
    TConflictNode *PHead;
    void *eleData;
    
    if (NULL == pHashTable || NULL == KeyFunction || NULL == CompareFunc)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Pointer NULL\n");
        return NULL;
    }

    HashKey = KeyFunction((UINT8 *)pHashDataToKey);
    if (HashKey > HashTableLength)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Hash Key Too Large\n");
        return NULL;
    }

    PHead = pHashTable[HashKey].pNode;
    if (pHashTable[HashKey].ConflictCount == 1)
    {
        return PHead->pData;
    }
    else //search in conflict list
    {
        //used head node
        if (LIST_DATA_EQUAL == CompareFunc(PHead->pData, pHashDataToKey))
        {
            return PHead->pData;
        }
        else
        {
            Ret = FindCurNodeByData(PHead, pHashDataToKey, &eleData, CompareFunc);
            if (LIST_FIND_OK == Ret)
            {
                return eleData;
            }
            else
            {
                LOG_PRINT(LOG_LEVEL_ERROR, "Pointer NULL\n");
                return NULL;
            }
        }
    }
}

/*============================================================================*/
/*  函数名 : Hash_Destroy                                                     */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.11.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void Hash_Destroy(THashNode *pHash)
{
    if (NULL != pHash)
    {
        free(pHash);
    }

    return;
}

/*============================================================================*/
/*  函数名 : CreateList                                                       */
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
static UINT32 CreateList(PLConflictNode *list)
{
    *list = (PLConflictNode)malloc(sizeof(TConflictNode));

    if (NULL != *list)
    {
        (*list)->Next = (*list)->Prior = *list;
        (*list)->pData = NULL;
        return LIB_ERR_SUCCESS;
    }

    return LIB_ERR_POINTER_NULL;
}

/*============================================================================*/
/*  函数名 : InsertNodeToListAtTail                                           */
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
static UINT32 InsertNodeToListAtTail(PLConflictNode list, void *e)
{
    PLConflictNode p = list->Prior, s;

    s = (PLConflictNode)malloc(sizeof(TConflictNode));
    if (NULL == s)
    {
        return LIB_ERR_POINTER_NULL;
    }

    p->Next = s;
    s->Next = list;
    list->Prior= s;
    s->Prior = p;
    s->pData = e;
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : FindCurNodeByData                                                */
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
static UINT32 FindCurNodeByData(PLConflictNode list, 
                                void *Cur_Key,
                                void **Cur_e, 
                                List_Compare fp_Compare)
{
	PLConflictNode p = list->Next;   

    if (NULL == list)
    {
        printf("List NULL\n");
        return LIST_FIND_FAILED;
    }

    if (NULL == fp_Compare)
    {
        return LIST_FIND_FAILED;
    }

    while (p != list)
    {
        if (LIST_DATA_EQUAL == fp_Compare(p->pData, Cur_Key))
        {
            *Cur_e = p->pData;
            return LIST_FIND_OK;
        }

        p = p->Next;
    }
    
    return LIST_FIND_FAILED;
}

