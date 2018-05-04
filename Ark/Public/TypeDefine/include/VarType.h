/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : VarType.h                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _VARTYPE_H
#define _VARTYPE_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define UINT8_NULL              0xFE                /* 一个字节的填充值       */
#define UINT16_NULL             0xFFFE              /* 两个字节的填充值       */
#define UINT32_NULL             0xFFFFFFFE          /* 四个字节的填充值       */
#define UINT8_ALL               0xFF                /* 一个字节的全选值       */
#define UINT16_ALL              0xFFFF              /* 两个字节的全选值       */
#define UINT32_ALL              0xFFFFFFFF          /* 四个字节的最大值       */
#define UINT8_RESERVE           0xFE                /* 一个字节的保留值       */
#define UINT16_RESERVE          0xFEFE              /* 两个字节的保留值       */
#define UINT32_RESERVE          0xFEFEFEFE          /* 四个字节的保留值       */

#define FPOS(type, field) \
    /*lint -e545 */ (UINT32)&((type *)0)->field) /*lint +e545 */ 

#define FSIZ(type, field) sizeof(((type *)0)->field)

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define MAX(a,b) ((a) > (b) ? (a) : (b))

#define ARR_SIZE(a) (sizeof((a)) / sizeof((a[0]))) 

#define MEM_UINT8(x) (*((UINT8 *)(x))) 

#define MEM_UINT16(x) (*((UINT16 *)(x))) 

#define MEM_UINT32(x) (*((UINT32 *)(x))) 

#define PTR_UINT8(var)  ((UINT8 *)(void *)&(var)) 

#define PTR_UINT16(var)  ((UINT16 *)(void *)&(var)) 

#define PTR_UINT32(var) ((UINT32 *)(void *)&(var)) 

#define OPEN_FILE(fp, samplefile, formate)\
    {\
        fp = fopen(samplefile,formate);\
        if (NULL == fp)\
        {\
            perror("Open File Failed");\
            return LIB_ERR_FILE_OPEN_FAILED;\
        }\
    }                                                               

#define CLOSE_FILE(fp)\
    {\
        if (NULL != fp)\
        {\
            fclose(fp);\
        }\
    }

#define COMPARE_EQUAL  0
#define COMPARE_MORE   1
#define COMPARE_LESS   2

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef unsigned int    UINT32;
typedef int             INT32;
typedef unsigned short 	UINT16;
typedef short 	        INT16;
typedef unsigned char 	UINT8;
typedef char 	        INT8;
typedef unsigned long   ULong;
typedef unsigned long int UINT64;

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
#include <sys/types.h>
typedef pid_t OSI_PID_T;
typedef pthread_attr_t OSI_PTHREAD_ATTR;
typedef void (*pThreadFunction)(void *);
typedef int OSI_THREADRET;
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
#include <windows.h>
typedef unsigned int OSI_PID_T;
typedef void OSI_PTHREAD_ATTR;
typedef unsigned int (__stdcall *pThreadFunction)(void *);
typedef HANDLE OSI_THREADRET;
#else
#error OSI_LACK_CPU_TYPE
#endif

typedef union
{
    struct
    {
        UINT8 bit15:1;
        UINT8 bit14:1;
        UINT8 bit13:1;
        UINT8 bit12:1;
        UINT8 bit11:1;
        UINT8 bit10:1;
        UINT8 bit9 :1;      
        UINT8 bit8 :1; 
        UINT8 bit7 :1;
        UINT8 bit6 :1;
        UINT8 bit5 :1;
        UINT8 bit4 :1;
        UINT8 bit3 :1;
        UINT8 bit2 :1;
        UINT8 bit1 :1;
        UINT8 bit0 :1;                     
    }Reg;

    struct
    {
        UINT8  High;
        UINT8  Low;
    }Byte;

    UINT8 ucArray[2];
    
    UINT16 usVar;
}UnionU16;

typedef UINT32 (* Compare)(void *, void *);

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

#endif

