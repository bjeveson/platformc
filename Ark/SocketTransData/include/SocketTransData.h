/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : SocketTransData.h                                              */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _SOCKETTRANSDATA_H
#define _SOCKETTRANSDATA_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define SHOW_SOCKET_LISTEN_PORT    4444
#define SHOW_SOCKET_DATA_LEN       2048

#define SHOW_PROTOCOL_HEADER       sizeof(Tprotocol)

#define SHOW_OPER_BEGIN_FLAG               1
#define SHOW_OPER_EXCHANGING_FLAG          2
#define SHOW_OPER_EXCHANGING_RESULT_FLAG   3
#define SHOW_OPER_END_FALG                 4

#define SHOW_CMD_LEN               8

#define ASCII2NUM(A)    (A - 0x20202020)

#define BigLittleSwap32(A) ((((UINT32)(A) & 0xff000000) >> 24) | \
                             (((UINT32)(A) & 0x00ff0000) >> 8) | \
                             (((UINT32)(A) & 0x0000ff00) << 8) | \
                             (((UINT32)(A) & 0x000000ff) << 24))

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
#pragma pack(1)
typedef struct
{
    UINT32 m_ulOperFlag;
    UINT32 m_ulFrameOrder;
    UINT32 m_ulTotalFrame;
    UINT32 m_ulOperResult;
    UINT32 m_ulDataLen;
}Tprotocol;
#pragma pack()

typedef UINT32 (* PF_GetOmData)(UINT32 *pulDataLen, UINT8 **pData);

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 Trans_Server(void);

UINT32 Trans_InitDrvApi(PF_GetOmData pfn);

#endif

