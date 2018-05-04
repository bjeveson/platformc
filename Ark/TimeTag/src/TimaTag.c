/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TimaTag.c                                                      */
/*  Descript : Test Time To Some Code, (us)                                   */
/*  API Pref : TC                                                               */
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
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Arch.h"
#include "VarType.h"
#include "ErrorCode.h"
#include "OSI.h"
#include "TimeTag.h"
#include "HexConversion.h"
#include "MemoryPool.h"
#include "TimeTag.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
/* Array Of Store Head Node */
static TCountHeadNode g_HeadNodeArray[TC_HEADNODE_MAXNUM];

/* Used Index Head Node */
static UINT32 g_HeadNodeUsedNo = 0;

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/

static UINT32 SetTagRunFlag(UINT32 ulTagTypeID, UINT32 ulTagRunFlag);

static UINT32 TC_PrintTimeTag(UINT32 ulTagTypeID);

static UINT32 GetTagRunFlag(UINT32 ulTagTypeID, UINT32 *pulTagRunFlag);

static UINT32 PrintTimeTagHeadinfo(UINT32 ulTagTypeID);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : TC_AddTimeTag                                                  */
/*  Input    : %1 -- Head Info                                                */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Add A Head Node                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TC_AddTimeTag(TRegHeadInfo tHeadNode)
{
    UINT32 ulRet = LIB_ERR_SUCCESS;
    
    if (g_HeadNodeUsedNo >= TC_HEADNODE_MAXNUM)
    {
        return LIB_ERR_ARRAY_OUT;
    }
    
    if (tHeadNode.m_ulHeadNodeID != g_HeadNodeUsedNo)
    {
        return (LIB_ERRCODE | 0X01);
    }
    
    if (tHeadNode.m_ulCountNodeNum > TC_COUNTERNODE_MAXNUM)
    {
        return LIB_ERR_ARRAY_OUT;
    }
    
    memcpy((void *)&g_HeadNodeArray[g_HeadNodeUsedNo].m_tHeadInfo, 
           (void *)&tHeadNode, 
           sizeof(TRegHeadInfo));

    g_HeadNodeArray[g_HeadNodeUsedNo].m_TagRecordTimes = 0;
    g_HeadNodeArray[g_HeadNodeUsedNo].m_TagRunFlag = TC_TAG_CLOSE_FLAG;
    g_HeadNodeArray[g_HeadNodeUsedNo].m_pTimeTag = 
          (TtimeTag *)MemPool_Malloc(sizeof(TtimeTag) * tHeadNode.m_ulCountNodeNum);

    g_HeadNodeUsedNo++;
    
    return ulRet;
}

/*============================================================================*/
/*  Function : TC_SetTimeTag                                                  */
/*  Input    : %1 -- Tag Id                                                   */
/*           : %2 -- Tag Begin Or End Flag                                    */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Set Time Tag                                                   */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TC_SetTimeTag(UINT32 ulTagTypeID, UINT32 ulBeginOrEndOfTag)
{
    struct timeval tv;/* lint !e86 */

    if (TC_TAG_CLOSE_FLAG == g_HeadNodeArray[ulTagTypeID].m_TagRunFlag)
    {
        return LIB_ERR_SUCCESS;
    }

    OSI_GettimeOfDay(&tv, (struct timezone *)0);
    
    if (ulTagTypeID >= TC_HEADNODE_MAXNUM)
    {
        printf("Tag ID:%d Out Of Range\n", ulTagTypeID);

        return LIB_ERR_ARRAY_OUT;
    }

    if (TC_TAG_RECORD_BEGIN == ulBeginOrEndOfTag)
    {
        g_HeadNodeArray[ulTagTypeID].m_pTimeTag[g_HeadNodeArray[ulTagTypeID].m_TagRecordTimes % g_HeadNodeArray[ulTagTypeID].m_tHeadInfo.m_ulCountNodeNum].begintime = tv;
    }
    else if (TC_TAG_RECORD_END == ulBeginOrEndOfTag)
    {
        g_HeadNodeArray[ulTagTypeID].m_pTimeTag[g_HeadNodeArray[ulTagTypeID].m_TagRecordTimes % g_HeadNodeArray[ulTagTypeID].m_tHeadInfo.m_ulCountNodeNum].endtime = tv;
        g_HeadNodeArray[ulTagTypeID].m_TagRecordTimes++;
    }
    else
    {
        printf("Tag Flag Error!\n");

        return LIB_ERR_PARA_INVALID;
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : TC_SetTimeTagStatus                                            */
/*  Input    : %1 -- Para Num                                                 */
/*           : %2 -- Para String                                              */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 TC_SetTimeTagStatus(INT32 argc, INT8** argv)
{
    UINT32 ulRunFlag = 0;
    UINT32 ulRet = LIB_ERR_SUCCESS;
    UINT32 ulTaId;
    UINT32 ulTagIndex;
    INT8 *pCmd = "settimetag";

    if (strcmp(pCmd, argv[0]) != 0)
    {
        printf("Input Cmd:%s String Error!\n", argv[0]);

        return LIB_ERR_PARA_INVALID;
    }
    
    if (3 == argc)
    {
        ulTaId = Hexcon_StringToDec((UINT8 *)argv[1]);
        if (ulTaId >= TC_HEADNODE_MAXNUM)
        {
            printf("Head Node Index Out Of Range!\n");
            return LIB_ERR_PARA_INVALID;
        }
        
        if (strcmp("open", (INT8 *)argv[2]) == 0)
        {
            ulRunFlag = TC_TAG_OPEN_FLAG;
        }
        else if (strcmp("close", (INT8 *)argv[2]) == 0)
        {
            ulRunFlag = TC_TAG_CLOSE_FLAG;
        }
        else
        {
            printf("Para:%d = %sError!\n", argc, argv[2]);
            return LIB_ERR_PARA_INVALID;
        }

        ulRet = SetTagRunFlag(ulTaId, ulRunFlag);
    }
    else if (2 == argc)
    {
        if (strcmp("open", (INT8 *)argv[1]) == 0)
        {
            ulRunFlag = TC_TAG_OPEN_FLAG;
        }
        else if (strcmp("close", (INT8 *)argv[1]) == 0)
        {
            ulRunFlag = TC_TAG_CLOSE_FLAG;
        }
        else
        {
            printf("Para:%d = %sError!\n", argc, argv[1]);
            return LIB_ERR_PARA_INVALID;
        }

        for (ulTagIndex = 0; ulTagIndex < TC_HEADNODE_MAXNUM; ulTagIndex++)
        {
            ulRet += SetTagRunFlag(ulTagIndex, ulRunFlag);
        }
    }
    else
    {
        printf("Input Para num = %d Error!\n", argc);
        return LIB_ERR_PARA_INVALID;
    }
    
    return ulRet;

}

/*============================================================================*/
/*  Function : TC_PrintTimeTagInfo                                            */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
INT32 TC_PrintTimeTagInfo(INT32 argc, INT8** argv)
{
    UINT32 ulTaId;
    UINT32 ulRet = LIB_ERR_SUCCESS;
    INT8 *pCmd = "gettimetag";
    UINT32 ulTagTypeIndex = 0;

    if (strcmp(pCmd, argv[0]) != 0)
    {
        printf("Input Cmd:%s String Error!\n", argv[0]);
    
        return LIB_ERR_PARA_INVALID;
    }

    if (argc == 1)
    {
        for (ulTagTypeIndex = 0; 
             ulTagTypeIndex < TC_HEADNODE_MAXNUM;
             ulTagTypeIndex++)
        {
            ulRet += PrintTimeTagHeadinfo(ulTagTypeIndex);
        }
    }
    else if (argc == 2)
    {
        ulTaId = Hexcon_StringToDec((UINT8 *)argv[1]);

        PrintTimeTagHeadinfo(ulTaId);
        TC_PrintTimeTag(ulTaId);
    }
    else
    {
        printf("Para Error!\n");
        ulRet = LIB_ERR_PARA_INVALID;
    }

    return (INT32)ulRet;
}

/*============================================================================*/
/*  Function : TC_TimeFormate                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void TC_TimeFormate(UINT8 *pucStrTimeBuff, UINT8 ucStrLen, UINT8 *pucTimeFormate)
{   
    time_t timep;  
    struct tm *p_tm;
    
    timep = time(NULL);  
    p_tm = localtime(&timep);  
    strftime(pucStrTimeBuff, ucStrLen, pucTimeFormate, p_tm);  

    return;  
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : TC_PrintTimeTag                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 TC_PrintTimeTag(UINT32 ulTagTypeID)
{
    UINT32 ulTagNodeIndex = 0;
    
    if (ulTagTypeID >= TC_HEADNODE_MAXNUM)
    {
        printf("Tag ID:%d Out Of Range\n", ulTagTypeID);

        return LIB_ERR_ARRAY_OUT;
    }
    
    printf("     Index     BeginTime  EndTime         Spend(us)\n");
    for (ulTagNodeIndex = 0;
         ulTagNodeIndex < g_HeadNodeArray[ulTagTypeID].m_tHeadInfo.m_ulCountNodeNum;
         ulTagNodeIndex++)
    {
        printf("%8u     %8u     %8u     %8u\n", 
               ulTagNodeIndex,
               (UINT32)(g_HeadNodeArray[ulTagTypeID].m_pTimeTag[ulTagNodeIndex].begintime.tv_sec * 1000000
                        + g_HeadNodeArray[ulTagTypeID].m_pTimeTag[ulTagNodeIndex].begintime.tv_usec),
               (UINT32)(g_HeadNodeArray[ulTagTypeID].m_pTimeTag[ulTagNodeIndex].endtime.tv_sec * 1000000
                        + g_HeadNodeArray[ulTagTypeID].m_pTimeTag[ulTagNodeIndex].endtime.tv_usec),
               TC_CalcUsOfTime(g_HeadNodeArray[ulTagTypeID].m_pTimeTag[ulTagNodeIndex]));
        
    }
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : PrintTimeTagHeadinfo                                           */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 PrintTimeTagHeadinfo(UINT32 ulTagTypeID)
{
    UINT32 ulRet = LIB_ERR_SUCCESS;
    UINT32 ulFlag = 0;

    printf("\n---------------------------------------------------\n");
    printf("The %d Head Node Info:\n", ulTagTypeID);

    ulRet = GetTagRunFlag(ulTagTypeID, &ulFlag);

    if (TC_TAG_CLOSE_FLAG == ulFlag)
    {
        printf("Time Tag Record Is Closed.\n");
    }
    else if (TC_TAG_OPEN_FLAG == ulFlag)
    {
        printf("Time Tag Record Is Running.\n");
    }
    else
    {
        printf("Para Error!\n");
        return LIB_ERR_PARA_INVALID;
    }

    printf("TagDesc:%s\n", 
            g_HeadNodeArray[ulTagTypeID].m_tHeadInfo.m_ucHEADNodeDesc);
    printf("Time Tag Record Times:%d\n", 
            g_HeadNodeArray[ulTagTypeID].m_TagRecordTimes);
    printf("Time Tag Current Index:%d\n",
            g_HeadNodeArray[ulTagTypeID].m_TagRecordTimes % g_HeadNodeArray[ulTagTypeID].m_tHeadInfo.m_ulCountNodeNum);

    return ulRet;
}

/*============================================================================*/
/*  Function : SetTagRunFlag                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 SetTagRunFlag(UINT32 ulTagTypeID, UINT32 ulTagRunFlag)
{
    if (ulTagTypeID >= TC_HEADNODE_MAXNUM)
    {
        printf("Tag ID:%d Out Of Range\n", ulTagTypeID);

        return LIB_ERR_SUCCESS;
    }
    
    if ((TC_TAG_OPEN_FLAG != ulTagRunFlag)
        && (TC_TAG_CLOSE_FLAG != ulTagRunFlag))
    {
        printf("Tag Run Flag Error!\n");
        return LIB_ERR_PARA_INVALID;
    }

    if (ulTagTypeID >= g_HeadNodeUsedNo)
    {
        /* 没有用到的头结点不能设置成open状态，防止被未初始化就使用 */
        printf("%d Head Node Not Init, Can Not Set!\n", ulTagTypeID);
        return LIB_ERR_SUCCESS;
    }
    
    if (TC_TAG_OPEN_FLAG == ulTagRunFlag)
    {
        (void)memset((void *)g_HeadNodeArray[ulTagTypeID].m_pTimeTag, 
                     0,
                     sizeof(TtimeTag) * g_HeadNodeArray[ulTagTypeID].m_tHeadInfo.m_ulCountNodeNum);

        g_HeadNodeArray[ulTagTypeID].m_TagRecordTimes = 0;
    }

    g_HeadNodeArray[ulTagTypeID].m_TagRunFlag = ulTagRunFlag;
    printf("%d Head Node Set Ok!\n", ulTagTypeID);
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : GetTagRunFlag                                                  */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 GetTagRunFlag(UINT32 ulTagTypeID, UINT32 *pulTagRunFlag)
{
    if (ulTagTypeID >= TC_HEADNODE_MAXNUM)
    {
        printf("Tag ID:%d Out Of Range\n", ulTagTypeID);

        return LIB_ERR_ARRAY_OUT;
    }

    *pulTagRunFlag = g_HeadNodeArray[ulTagTypeID].m_TagRunFlag;

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : TC_CalcUsOfTime                                                */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 TC_CalcUsOfTime(TtimeTag tTimeTag)
{
    return (UINT32)((tTimeTag.endtime.tv_sec - tTimeTag.begintime.tv_sec) * 1000000 
            + tTimeTag.endtime.tv_usec - tTimeTag.begintime.tv_usec);/* lint -e115 -e40 */
}/* lint -e115 -e40 !e550 */

/*============================================================================*/
/*  函数名 : GetCurrentSecondSOfToday                                         */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.001                                                        */
/*  日  期 : 2014.10.28                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 GetCurrentSecondSOfToday(void)
{	
    struct tm curTime;	
    time_t t = time(NULL);	
    curTime = *localtime(&t);
    
    return 3600 * curTime.tm_hour + 60 * curTime.tm_min + curTime.tm_sec;
}

