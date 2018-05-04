/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾���䱱����������� */
/*  ϵͳ���� : ZXONE 8600�⴫��ϵͳ                                           */
/*  �� �� �� : ClientCount.c                                                  */
/*  �������� :                                                                */
/*  �� �� �� : ������                                                         */
/*  �ļ��汾 : V1.00.001                                                      */
/*  �������� : 2014.10.27                                                     */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/

/*============================================================================*/
/* �����ı�׼����������Ǳ�׼��ͷ�ļ�                                         */
/*============================================================================*/
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "VarType.h"
#include "ServerCount.h"
#include "TimeTag.h"
#include "Shell.h"
#include "Dbug.h"
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
static TClientOperationRecord g_ClientRecord[CLIENT_RECORD_MAX_NUM] = {0};

static UINT32 g_RecordIndex = 0;

static pthread_mutex_t Record_Mutex;

/////////////////////////////////////////////
static TClientCount g_ClientCount = {0};

static pthread_mutex_t g_ClientCount_Mutex = {0};

/////////////////////////////////////////////
static TtimeTag g_TimeTag;

/*============================================================================*/
/* ���ļ�ʹ�õı���                                                           */
/*============================================================================*/

/*============================================================================*/
/* ���ʵ��                                                                   */
/*============================================================================*/
static UINT32 ShowClientCountInfo(UINT32 argc, UINT8 *argv[]);

static UINT32 ShowClientRecord(UINT32 argc, UINT8 *argv[]);

static void PrintRecord(INT32 index, TClientOperationRecord Record);
/*============================================================================*/
/*  ������ : InitClientCount                                                  */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.27                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
INT32 InitClientCount(void)
{
    struct timeval tv;
    TShell ShellNode;
    UINT8 *pCountCmd = "count";
    UINT8 *pCountCmdDesc = "show client count info useage:count";
    UINT8 *pRecordCmd = "record";
    UINT8 *pRecordCmdDesc = "show client record usage:record [pos:0-all,1-befor,2-after] [fd:0 -all,fd]";

    if (pthread_mutex_init(&Record_Mutex, NULL) != 0)  
    {  
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }

    if (pthread_mutex_init(&g_ClientCount_Mutex, NULL) != 0)  
    {  
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }

    ShellNode.MaxParaNum = 0;
    ShellNode.pfunc = ShowClientCountInfo;
    strncpy(ShellNode.ucCmdDesc, pCountCmdDesc, strlen(pCountCmdDesc) + 1);
    strncpy(ShellNode.ucCmdstr, pCountCmd, strlen(pCountCmd) + 1);
    Sh_AddCmd(ShellNode);

    ShellNode.MaxParaNum = 2;
    ShellNode.pfunc = ShowClientRecord;
    strncpy(ShellNode.ucCmdDesc, pRecordCmdDesc, strlen(pRecordCmdDesc) + 1);
    strncpy(ShellNode.ucCmdstr, pRecordCmd, strlen(pRecordCmd) + 1);
    Sh_AddCmd(ShellNode);

    return 0;
}

/*============================================================================*/
/*  ������ : AddClientRecord                                                  */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.27                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
INT32 AddClientRecord(TClientOperationRecord Record)
{
    INT32 ret;

    if ((ret = pthread_mutex_lock(&Record_Mutex)) != 0)
    {
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return -1;
    }
    
    g_ClientRecord[g_RecordIndex & (CLIENT_RECORD_MAX_NUM - 1)] = Record;
    g_RecordIndex++;
    
    pthread_mutex_unlock(&Record_Mutex);

    return 0;
}

/*============================================================================*/
/*  ������ : ClientCount                                                      */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.28                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
void ClientCount(INT32 CountOper)
{
    INT32 ret;

    if ((ret = pthread_mutex_lock(&g_ClientCount_Mutex)) != 0)
    {
        perror("");
        printf("%s:%d\n", __FILE__, __LINE__);
        return;
    }

    switch (CountOper)
    {
        case CLIENT_RECORD_OPER_CONNECT:
        {
            g_ClientCount.OnlineClientNum++;
            break;
        }
        case CLIENT_RECORD_OPER_CLOSE:
        {
            g_ClientCount.OnlineClientNum--;
            break;
        }
        case CLIENT_RECORD_OPER_WRITE:
        {
            g_ClientCount.OnlineRequestNum++;
            break;
        }
        default:
        {
            printf("Input CountOper Error\n");
        }
    }

    pthread_mutex_unlock(&g_ClientCount_Mutex);
    
    return ;
}

/*============================================================================*/
/*  ������ : ShowClientCountInfo                                              */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.28                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
static UINT32 ShowClientCountInfo(UINT32 argc, UINT8 *argv[])
{
    printf("OnlineUserNum OnlineRequestNum\n");

    printf("%8d    %8d\n",
           g_ClientCount.OnlineClientNum,
           g_ClientCount.OnlineRequestNum); 

    return 0;
}

/*============================================================================*/
/*  ������ : ShowClientRecord                                                 */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.28                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
static UINT32 ShowClientRecord(UINT32 argc, UINT8 *argv[])
{
    INT32 pos = atoi(argv[1]);
    INT32 fd = atoi(argv[2]);
    INT32 loop = 0;

    printf("index SecondOfToday  fd      Oper\n");
    if (pos == 0)
    {
        if (fd == 0)
        {
            for (loop = 0; loop < CLIENT_RECORD_MAX_NUM; loop++)
            {
                PrintRecord(loop, g_ClientRecord[loop]);
            }
        }
        else
        {
            for (loop = 0; loop < CLIENT_RECORD_MAX_NUM; loop++)
            {
                if (g_ClientRecord[loop].fd == fd)
                {
                    PrintRecord(loop, g_ClientRecord[loop]);
                }
            }
        }
    }
    else if (pos == 1)
    {
        if (fd == 0)
        {
            for (loop = 0; loop < g_RecordIndex; loop++)
            {
                PrintRecord(loop, g_ClientRecord[loop]);
            }
        }
        else
        {
            for (loop = 0; loop < g_RecordIndex; loop++)
            {
                if (g_ClientRecord[loop].fd == fd)
                {
                    PrintRecord(loop, g_ClientRecord[loop]);
                }
            }
        }
    }
    else if (pos == 2)
    {
        if (fd == 0)
        {
            for (loop = 0; loop < g_RecordIndex; loop++)
            {
                PrintRecord(loop, g_ClientRecord[loop]);
            }
        }
        else
        {
            for (loop = g_RecordIndex; loop < CLIENT_RECORD_MAX_NUM; loop++)
            {
                if (g_ClientRecord[loop].fd == fd)
                {
                    PrintRecord(loop, g_ClientRecord[loop]);
                }
            }
        }
    }
    else
    {
        printf("Para pos Input Error\n");
        return -1;
    }
    
    return 0;
}

/*============================================================================*/
/*  ������ : PrintRecord                                                      */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.10.28                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
static void PrintRecord(INT32 index, TClientOperationRecord Record)
{
    printf("%-8d %-8d %-8d %-8d\n", index, Record.SecondOfToday, Record.fd, Record.Oper);

    return;
}
