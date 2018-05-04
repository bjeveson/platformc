/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾���䱱������������ */
/*  ϵͳ���� : ZXONE 8600�⴫��ϵͳ                                           */
/*  �� �� �� : ClientCount.h                                                  */
/*  �������� :                                                                */
/*  �� �� �� : ������                                                         */
/*  �ļ��汾 : V1.00.001                                                      */
/*  �������� : 2014.10.27                                                     */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/
#ifndef _CLIENTCOUNT_H
#define _CLIENTCOUNT_H

/*============================================================================*/
/* �����ı�׼����������Ǳ�׼��ͷ�ļ�                                         */
/*============================================================================*/

/*============================================================================*/
/* ��������                                                                   */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֺ�                                                                     */
/*============================================================================*/
#define CLIENT_RECORD_MAX_NUM           (1024 * 1024 * 8) /* ǧ�� */
#define CLIENT_RECORD_NUM_PLUS          0
#define CLIENT_RECORD_NUM_MINUS         1
#define CLIENT_RECORD_DAILY_NUM         (60 * 60 *24)

#define CLIENT_RECORD_OPER_CONNECT      0
#define CLIENT_RECORD_OPER_CLOSE        1
#define CLIENT_RECORD_OPER_WRITE        2

/*============================================================================*/
/* ȫ����������                                                               */
/*============================================================================*/
typedef struct
{
    UINT32 SecondOfToday;
    INT32 fd;
    INT32 Oper;
}TClientOperationRecord;

/* ����enum��TClientCount����������ܸı�ṹ���Ա˳�� */
typedef struct
{
    UINT32 OnlineClientNum;   /* �����û��� */
    UINT32 OnlineRequestNum; /* ������������� */
}TClientCount;

enum Mutex
{
    ONLINECLIENT_NUM,
    CONNECTEDCLIENT_NUM,
    REQUEST_NUM
};

/*============================================================================*/
/* �ඨ��                                                                     */
/*============================================================================*/

/*============================================================================*/
/* ģ��(template)(����C++�е���ģ��ͺ���ģ��)                                */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֺ���ԭ��                                                               */
/*============================================================================*/
INT32 InitClientCount(void);

INT32 AddClientRecord(TClientOperationRecord Record);

void ClientCount(INT32 CountOper);

#endif
