/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司波分软件开发部     */
/*  系统名称 : 8000平台                                                       */
/*  文 件 名 : LibInit.h                                                      */
/*  功能描述 :                                                                */
/*  作    者 : 胡永福                                                         */
/*  文件版本 : V1.00                                                          */
/*  创建日期 : 2014.6.19                                                      */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/
#ifndef _LIBINIT_H
#define _LIBINIT_H

/*============================================================================*/
/* 包含的头文件，依次为标准库头文件、非标准库头文件                           */
/*============================================================================*/

/*============================================================================*/
/* 常量定义                                                                   */
/*============================================================================*/

/*============================================================================*/
/* 全局宏                                                                     */
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
/* 全局数据类型                                                               */
/*============================================================================*/

/*============================================================================*/
/* 类定义(C++),局部函数定义(C)                                                */
/*============================================================================*/

/*============================================================================*/
/* 模板(template)(包括C++中的类模板和函数模板),C语言没有此项                  */
/*============================================================================*/

/*============================================================================*/
/* 全局函数原型                                                               */
/*============================================================================*/
void LibInit(void);

#endif

