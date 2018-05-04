/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾�������������     */
/*  ϵͳ���� : 8000ƽ̨                                                       */
/*  �� �� �� : LibInit.h                                                      */
/*  �������� :                                                                */
/*  ��    �� : ������                                                         */
/*  �ļ��汾 : V1.00                                                          */
/*  �������� : 2014.6.19                                                      */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/
#ifndef _LIBINIT_H
#define _LIBINIT_H

/*============================================================================*/
/* ������ͷ�ļ�������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ�                           */
/*============================================================================*/

/*============================================================================*/
/* ��������                                                                   */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֺ�                                                                     */
/*============================================================================*/
#define DMM_BLOCK_SIZE_2BYTE        (2)
#define DMM_BLOCK_SIZE_4BYTE        (4)
#define DMM_BLOCK_SIZE_8BYTE        (8)
#define DMM_BLOCK_SIZE_16BYTE       (16)
#define DMM_BLOCK_SIZE_32BYTE       (32)
#define DMM_BLOCK_SIZE_48YTE        (48)
#define DMM_BLOCK_SIZE_64YTE        (64)
#define DMM_BLOCK_SIZE_96YTE        (96)
#define DMM_BLOCK_SIZE_128YTE       (128)
#define DMM_BLOCK_SIZE_192YTE       (192)
#define DMM_BLOCK_SIZE_256BYTE      (256)
#define DMM_BLOCK_SIZE_384YTE       (384)
#define DMM_BLOCK_SIZE_512BYTE      (512)
#define DMM_BLOCK_SIZE_768YTE       (768)
#define DMM_BLOCK_SIZE_1KBYTE       (1024)
#define DMM_BLOCK_SIZE_2KBYTE       (DMM_BLOCK_SIZE_1KBYTE * 2)
#define DMM_BLOCK_SIZE_4KBYTE       (DMM_BLOCK_SIZE_1KBYTE * 4)
#define DMM_BLOCK_SIZE_8KBYTE       (DMM_BLOCK_SIZE_1KBYTE * 8)
#define DMM_BLOCK_SIZE_16KBYTE      (DMM_BLOCK_SIZE_1KBYTE * 16)
#define DMM_BLOCK_SIZE_32KBYTE      (DMM_BLOCK_SIZE_1KBYTE * 32)
#define DMM_BLOCK_SIZE_48KBYTE      (DMM_BLOCK_SIZE_1KBYTE * 48)
#define DMM_BLOCK_SIZE_64KBYTE      (DMM_BLOCK_SIZE_1KBYTE * 64)
#define DMM_BLOCK_SIZE_96KBYTE      (DMM_BLOCK_SIZE_1KBYTE * 96)
#define DMM_BLOCK_SIZE_128KBYTE     (DMM_BLOCK_SIZE_1KBYTE * 128)
#define DMM_BLOCK_SIZE_192KBYTE     (DMM_BLOCK_SIZE_1KBYTE * 192)
#define DMM_BLOCK_SIZE_256KBYTE     (DMM_BLOCK_SIZE_1KBYTE * 256)
#define DMM_BLOCK_SIZE_384KBYTE     (DMM_BLOCK_SIZE_1KBYTE * 384)
#define DMM_BLOCK_SIZE_512KBYTE     (DMM_BLOCK_SIZE_1KBYTE * 512)
#define DMM_BLOCK_SIZE_768KBYTE     (DMM_BLOCK_SIZE_1KBYTE * 768)
#define DMM_BLOCK_SIZE_1MKBYTE      (DMM_BLOCK_SIZE_1KBYTE * 1024)

/*============================================================================*/
/* ȫ����������                                                               */
/*============================================================================*/

/*============================================================================*/
/* �ඨ��(C++),�ֲ���������(C)                                                */
/*============================================================================*/

/*============================================================================*/
/* ģ��(template)(����C++�е���ģ��ͺ���ģ��),C����û�д���                  */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֺ���ԭ��                                                               */
/*============================================================================*/
void LibInit(void);

#endif

