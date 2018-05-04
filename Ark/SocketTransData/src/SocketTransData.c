/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : SocketTransData.c                                              */
/*  Descript : Translate Data Through Socket                                  */
/*           : Translate Protocol:                                            */
/*           : Operation_Flag(4)|Framer_Order(4)|Total_Framer(4)|Result(4)|   */
/*           : Current_Framer_Len(4)|Framer_Data                              */
/*           : Operation_Flag     : 1 -- Request Data                         */
/*           :                      2 -- Translate Data                       */
/*           :                      3 -- Translate Result                     */
/*           :                      4 -- Translate Data Over                  */
/*           : Framer_Order       : Current Index Framer                      */
/*           : Total_Framer       : Will Translate Framer Num Of Data         */
/*           : Result             : 0 -- Success                              */
/*           :                    : 1 -- Failed                               */
/*           : Current_Framer_Len : Framer_Data Len,Max SHOW_SOCKET_DATA_LEN  */
/*           : Framer_Data        : Payload                                   */
/*  API Pref : Trans                                                          */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include "Arch.h"
#include "VarType.h"
#include "OSI.h"
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
#include <winsock.h> 
#pragma comment(lib,"ws2_32.lib")
#else
#error OSI_LACK_CPU_TYPE
#endif
#include <string.h>
#include <stdio.h>

#include "ErrorCode.h"
#include "VarType.h"
#include "SocketTransData.h"

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static PF_GetOmData pfusr_GetOmData = NULL; 

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static UINT32 DataDeal(Tprotocol tRecvProtocol, 
                       Tprotocol *pSendProtocol,
                       UINT8 *pBuffer);


static UINT32 parseprocotol(const UINT8 *pBuffer, Tprotocol *pProtocol);

static void StructMsgheader(const Tprotocol *protocol, UINT8 *pBuffer);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Trans_InitDrvApi                                               */
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
UINT32 Trans_InitDrvApi(PF_GetOmData pfn)
{
    if (0 != pfn)
    {
        pfusr_GetOmData = (PF_GetOmData)pfn;
    }
    else
    {
        return LIB_ERR_POINTER_NULL;
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Trans_Server                                                   */
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
UINT32 Trans_Server(void)
{
    struct sockaddr_in servaddr, cliaddr;
    INT32 listenfd, connfd, cliaddr_len, msg_len;
    UINT8 buffer[SHOW_SOCKET_DATA_LEN + SHOW_PROTOCOL_HEADER];
    Tprotocol tRecvProtocol, tSendProtocol;
    
    memset(&servaddr, 0, sizeof(servaddr));
    cliaddr_len = sizeof(cliaddr);
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenfd)
    {
        printf("Create Socket Error!\n");
        return LIB_ERR_NETWORK_FAIL;
    }
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SHOW_SOCKET_LISTEN_PORT);

    if (-1 == bind((INT32)listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))/*lint !e64 !e740*/
    {
        printf("Bind Socket Error!\n");
        return LIB_ERR_NETWORK_FAIL;
    }

    if (-1 == listen(listenfd, 20))
    {
        printf("Listen Socket Error!\n");
        return LIB_ERR_NETWORK_FAIL;
    }

    while (1)
    {
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);/*lint !e740 !e64*/
        printf("%d Client Connect.\n", connfd);
        
        while (1)
        {
            //printf("Read Data Len:");
            msg_len = read(connfd, (INT8 *)buffer, SHOW_SOCKET_DATA_LEN);
            //printf("%d\n", msg_len);
            if (msg_len <= 0)
            {
                printf("%d Client DisConnect!\n", connfd);
                break;
            }
            
            parseprocotol(buffer, &tRecvProtocol);

            memset(buffer, 0, SHOW_SOCKET_DATA_LEN);
            DataDeal(tRecvProtocol, &tSendProtocol, buffer);

            write(connfd, (INT8 *)buffer, SHOW_PROTOCOL_HEADER + tSendProtocol.m_ulDataLen);
        }

        close(connfd);
        printf("Sample Process Quit!\n");
    }

}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : parseprocotol                                                  */
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
static UINT32 parseprocotol(const UINT8 *pBuffer, Tprotocol *pProtocol)
{
    pProtocol->m_ulOperFlag = BigLittleSwap32(*(UINT32 *)pBuffer);
    pProtocol->m_ulFrameOrder = BigLittleSwap32(*(UINT32 *)(pBuffer + 4));
    pProtocol->m_ulTotalFrame = BigLittleSwap32(*(UINT32 *)(pBuffer + 8));
    pProtocol->m_ulOperResult = BigLittleSwap32(*(UINT32 *)(pBuffer + 12));
    pProtocol->m_ulDataLen = BigLittleSwap32(*(UINT32 *)(pBuffer + 16));

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : DataDeal                                                       */
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
static UINT32 DataDeal(Tprotocol tRecvProtocol, 
                       Tprotocol *pSendProtocol,
                       UINT8 *pBuffer)
{
    UINT32 ulRet = LIB_ERR_SUCCESS;
    UINT8 *pModData = NULL;
    UINT32 ulDataLen = 0;
    
    switch (tRecvProtocol.m_ulOperFlag)
    {
        case SHOW_OPER_BEGIN_FLAG:
        {
            if (NULL != pfusr_GetOmData)
            {
                ulRet = pfusr_GetOmData(&ulDataLen, &pModData);
                if (LIB_ERR_SUCCESS != ulRet) 
                {
                    printf("Sample Optical Module Data Error:%x!\n", ulRet);
                    return LIB_ERR_NETWORK_FAIL;
                }

                if (ulDataLen > SHOW_SOCKET_DATA_LEN)
                {
                    printf("Optical Module Retun Data Toooo Long:%x!\n",
                           ulDataLen);
                    return LIB_ERR_ARRAY_OUT;
                }
            }
            else
            {
                printf("Get Optical Module Api Not Init!\n");
                return LIB_ERR_POINTER_NULL;
            }

            pSendProtocol->m_ulOperFlag = SHOW_OPER_EXCHANGING_FLAG;
            pSendProtocol->m_ulFrameOrder = 1;
            pSendProtocol->m_ulTotalFrame = 1;
            pSendProtocol->m_ulOperResult = 0;
            pSendProtocol->m_ulDataLen = ulDataLen;

            StructMsgheader(pSendProtocol, pBuffer);
            
            memcpy((void *)(pBuffer + SHOW_PROTOCOL_HEADER), 
                   pModData,
                   ulDataLen);
            break;
        }
        case SHOW_OPER_EXCHANGING_FLAG:
        {
            break;
        }
        case SHOW_OPER_EXCHANGING_RESULT_FLAG:
        {
            break;
        }
        case SHOW_OPER_END_FALG:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : StructMsgheader                                                */
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
static void StructMsgheader(const Tprotocol *protocol, UINT8 *pBuffer)
{
    memcpy((void *)pBuffer, (void *)&protocol->m_ulOperFlag, 4);
    memcpy((void *)(pBuffer + 4), (void *)&protocol->m_ulFrameOrder, 4);
    memcpy((void *)(pBuffer + 8), (void *)&protocol->m_ulTotalFrame, 4);
    memcpy((void *)(pBuffer + 12), (void *)&protocol->m_ulOperResult, 4);
    memcpy((void *)(pBuffer + 16), (void *)&protocol->m_ulDataLen, 4);

    return;
}

