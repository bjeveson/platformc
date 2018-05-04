/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾���䱱����������� */
/*  ϵͳ���� : ZXONE 8600�⴫��ϵͳ                                           */
/*  �� �� �� : WebSocketMsgParse.c                                            */
/*  �������� :                                                                */
/*  �� �� �� : ������                                                         */
/*  �ļ��汾 : V1.00.00                                                       */
/*  �������� : 2014.12.25                                                     */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/

/*============================================================================*/
/* �����ı�׼����������Ǳ�׼��ͷ�ļ�                                         */
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
/* ���ļ�ʹ�õĳ�������                                                       */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õĺ궨��                                                         */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õ���������                                                       */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֱ���                                                                   */
/*============================================================================*/
static UINT32 *pWebSocketStatus = NULL;
/*============================================================================*/
/* ���ļ�ʹ�õı���                                                           */
/*============================================================================*/

/*============================================================================*/
/* ���ʵ��                                                                   */
/*============================================================================*/
static TWebSocketHandShake HandShakeValue(UINT8 *p);

static TWebSocketDataFrame DataFrameParse(UINT8 *pMsg);
/*============================================================================*/
/*  ������ : WS_init                                                          */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.00                                                         */
/*  ��  �� : 2014.12.27                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
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
/*  ������ : WS_MsgParse                                                      */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.00                                                         */
/*  ��  �� : 2014.12.27                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
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
/*  ������ : HandShakeValue                                                   */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.00                                                         */
/*  ��  �� : 2014.12.27                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
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
