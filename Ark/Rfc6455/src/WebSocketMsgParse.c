/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : WebSocketMsgParse.c                                            */
/*  功能描述 :                                                                */
/*  创 建 人 : 胡永福                                                         */
/*  文件版本 : V1.00.00                                                       */
/*  创建日期 : 2014.12.25                                                     */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/

/*============================================================================*/
/* 包含的标准库或者其他非标准库头文件                                         */
/*============================================================================*/
#include <stdio.h>
#include <string.h>

#include "VarType.h"
#include "ErrorCode.h"
#include "MemoryPool.h"
#include "Log.h"
#include "ProtocalParse.h"
#include "WebSocketMsgParse.h"
/*============================================================================*/
/* 本文件使用的常量定义                                                       */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的宏定义                                                         */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的数据类型                                                       */
/*============================================================================*/

/*============================================================================*/
/* 全局变量                                                                   */
/*============================================================================*/
static UINT32 *pWebSocketStatus = NULL;
/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
static TWebSocketHandShake HandShakeValue(UINT8 *p);

static TWebSocketDataFrame DataFrameParse(UINT8 *pMsg);
/*============================================================================*/
/*  函数名 : WS_init                                                          */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.27                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
UINT32 WS_init(UINT32 MaxClientNum)
{
    pWebSocketStatus = (UINT32 *)MemPool_Malloc(sizeof(UINT32) * MaxClientNum);
    if (NULL == pWebSocketStatus)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "Malloc Failed\n");
        return LIB_ERR_POINTER_NULL;
    }

    memset(pWebSocketStatus, WEBSOCKET_STATUS_IDLE, sizeof(UINT32) * MaxClientNum);
    
    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : WS_MsgParse                                                      */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.27                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
TProtocalParse WS_MsgParse(INT32 Fd, UINT8 *pWSMsg)
{
    TProtocalParse Parse;
    TWebSocketHandShake Shake;
    TWebSocketDataFrame DataFrame;

    switch (pWebSocketStatus[Fd])
    {
        case WEBSOCKET_STATUS_IDLE:
        {
            /* check hand shake msg */
            Shake = HandShakeValue(pWSMsg);
            break;
        }
        case WEBSOCKET_STATUS_SHAKE_OK:
        {
            /* check service data msg */
            DataFrame = DataFrameParse(pWSMsg);
            Parse.pServiceMsgBody = DataFrame.PayloadData;
            Parse.ServiceMsgBodyLen = DataFrame.PayloadLength;
            //Parse.MsgType = ;
            Parse.pProtocalHeader = pWSMsg;
            break;
        }
        default:
        {
            LOG_PRINT(LOG_LEVEL_ERROR, "Status Machine Error\n");
        }
    }

    Parse.ProtocalType = PROTOCAL_TYPE_WEBSOCKET;
    
    return Parse;
}

/*============================================================================*/
/*  函数名 : HandShakeValue                                                   */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.27                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static TWebSocketHandShake HandShakeValue(UINT8 *p)
{
    UINT8 *Line;
    UINT32 i = 0;
    UINT8 *sp = p;
    UINT32 index = 0;
    TWebSocketHandShake Shake;

    Line = sp;
    while (*sp != '\0')
    {
        if (*sp == '\n')
        {
            *(sp - 1) = '\0';

            switch (Line[0])
            {
                case 'G': Shake.Get.p = Line + 4; break;
                case 'H': Shake.Host.p = Line + 6; break;
                case 'U': Shake.Upgrade.p = Line + 9; break;
                case 'C': Shake.Connection.p = Line + 12; break;
                case 'O': Shake.Origin.p = Line + 8; break;
                case 'S': 
                    switch (Line[14])
                    {
                        case 'K': Shake.Sec_WebSocket_Key.p = Line + 19; break;
                        case 'P': Shake.Sec_WebSocket_Protocol.p = Line + 24; break;
                        case 'V': Shake.Sec_WebSocket_Version.p = Line + 23; break;
                        default: printf("Unknow Protocal Context\n");
                    }
                    break;
                default: printf("Unknow Protocal Context\n");
            }

            Line = sp + 1;  //next line
        }
        sp++;
    }

    return Shake;
}

static TWebSocketDataFrame DataFrameParse(UINT8 *pMsg)
{
    TWebSocketDataFrame DataFrame;
    UINT32 *pInt = (UINT32 *)pMsg;
    UINT64 ExtendedPayloadLength12, ExtendedPayloadLength3456, ExtendedPayloadLength78;
    UINT32 Masking_key12, Masking_key34;
    UINT32 PayloadLen;

    DataFrame.FIN = Bit_GetMNBits(*pInt, 0, 0);
    DataFrame.RSV1 = Bit_GetMNBits(*pInt, 1, 1);
    DataFrame.RSV2 = Bit_GetMNBits(*pInt, 2, 2);
    DataFrame.RSV3 = Bit_GetMNBits(*pInt, 3, 3);
    DataFrame.OpCode = Bit_GetMNBits(*pInt, 4, 7);
    DataFrame.MASK = Bit_GetMNBits(*pInt, 8, 8);
    PayloadLen = Bit_GetMNBits(*pInt, 9, 15);
    if (PayloadLen >= 0 && PayloadLen <= 125)
    {
        DataFrame.PayloadLength = PayloadLen;
    }
    else if (PayloadLen == 126)
    {
        DataFrame.PayloadLength = Bit_GetMNBits(*pInt, 16, 31);
    }
    else if (PayloadLen = 127)
    {
        ExtendedPayloadLength12 = Bit_GetMNBits(*pInt, 16, 31);

        pInt = (UINT32 *)(pMsg + 4);
        ExtendedPayloadLength3456 = *pInt;

        pInt = (UINT32 *)(pMsg + 8);
        ExtendedPayloadLength78 = Bit_GetMNBits(*pInt, 0, 15);
        DataFrame.PayloadLength = (ExtendedPayloadLength78 << 48) | (ExtendedPayloadLength3456 << 16) | ExtendedPayloadLength12;
    }
    else
    {
        memset(&DataFrame, 0, sizeof(TWebSocketDataFrame));
        return DataFrame;
    }

    pInt = (UINT32 *)(pMsg + 8);
    Masking_key12 = Bit_GetMNBits(*pInt, 16, 31);

    pInt = (UINT32 *)(pMsg + 12);
    Masking_key34 = Bit_GetMNBits(*pInt, 0, 15);
    DataFrame.Masking_key = (Masking_key34 << 16) | Masking_key12;
        
    DataFrame.PayloadData = pMsg + 14;
    
    return DataFrame;
}
