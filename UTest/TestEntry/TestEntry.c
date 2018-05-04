/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TestEntry.c                                                    */
/*  Descript :                                                                */
/*  API Pref :                                                                */
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
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "VarType.h"
#include "Log.h"
#include "MemoryPool.h"
#include "TDMMMalloc.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/

/*============================================================================*/
/* Macro Define                                                               */
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
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static void DMMInit(void);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
INT32 main(INT32 argc, INT8* argv[])
{
    TLogRegInfo tLog;
    UINT8 *pucLogFilePreFix = "MyLib";
    UINT8 ucCurDir[OSI_PATH_MAX];

    DMMInit();
    
    FS_GetCurDir(ucCurDir);
    
    memcpy((void *)tLog.m_ucLogFilePath, 
           (void *)ucCurDir,
           strlen(ucCurDir) + 1);
    memcpy((void *)tLog.m_ucFileNamePrefix,
           (void *)pucLogFilePreFix,
           strlen(pucLogFilePreFix) + 1);
    tLog.m_ulCurOutLevel = LOG_LEVEL_INFO;
    tLog.m_ulCurOutPos = LOG_OUTPOS_DISPLAY;
    tLog.m_ulLogFileSize = 1024 * 10;
    tLog.m_ulLogServerIp = 0;
    tLog.m_ulLogServerPort = 0;
    tLog.m_ulFlushFileEveryNLogs = 100;

    /* Init Log Module */
    Log_Init(tLog);

    /* Add Test Case */
   
    TStub_AddTestCase();

    TBit_AddTestCase();

    TChar_AddTestCase();

    TString_AddTestCase();    

    TSort_AddTestCase();

    TMaths_AddTestCase();

    THexcon_AddTestCase();

    TSearch_AddTestCase();

    THash_AddTestCase();

    TStack_AddTestCase();

    TList_AddTestCase();
    
    TThreadPool_AddTestCase();

    TDMM_Malloc_AddTestCase();    
 
    TQueue_AddTestCase();

    TDBConPool_AddTestCase();
    
    /* Run All Test Case */
    TD_RunAllTestCase();
   
    Log_Destroy();

    MemPool_DestroyMem();
    
    printf("Unit Test Finished!\n");
    
    return 0;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

/*============================================================================*/
/*  Function : DMMInit                                                        */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     : Use DMM Init                                                   */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.13                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void DMMInit(void)
{
    TMemListInfo listinfo[] =   
                        {         /* size                 num        usedno threshold */
                            { DMM_BLOCK_SIZE_2BYTE    ,    30000,      0,       80  },
                            { DMM_BLOCK_SIZE_4BYTE    ,    20000,      0,       80  },
                            { DMM_BLOCK_SIZE_8BYTE    ,    10000,      0,       80  },
                            { DMM_BLOCK_SIZE_16BYTE   ,    9000,       0,       80  },
                            { DMM_BLOCK_SIZE_32BYTE   ,    8000,       0,       80  },
                            { DMM_BLOCK_SIZE_48YTE    ,    7000,       0,       80  },
                            { DMM_BLOCK_SIZE_64YTE    ,    6000,       0,       80  },
                            { DMM_BLOCK_SIZE_96YTE    ,    5000,       0,       80  },
                            { DMM_BLOCK_SIZE_128YTE   ,    4000,       0,       80  },
                            { DMM_BLOCK_SIZE_192YTE   ,    3000,       0,       80  },
                            { DMM_BLOCK_SIZE_256BYTE  ,    2000,       0,       80  },
                            { DMM_BLOCK_SIZE_384YTE   ,    1000,       0,       80  },
                            { DMM_BLOCK_SIZE_512BYTE  ,    900,        0,       80  },
                            { DMM_BLOCK_SIZE_768YTE   ,    800,        0,       80  },
                            { DMM_BLOCK_SIZE_1KBYTE   ,    700,        0,       80  },
                            { DMM_BLOCK_SIZE_2KBYTE   ,    600,        0,       80  },
                            { DMM_BLOCK_SIZE_4KBYTE   ,    500,        0,       80  },
                            { DMM_BLOCK_SIZE_8KBYTE   ,    400,        0,       80  },
                            { DMM_BLOCK_SIZE_16KBYTE  ,    300,        0,       80  },
                            { DMM_BLOCK_SIZE_32KBYTE  ,    200,        0,       80  },
                            { DMM_BLOCK_SIZE_48KBYTE  ,    100,        0,       80  },
                            { DMM_BLOCK_SIZE_64KBYTE  ,    90,         0,       80  },
                            { DMM_BLOCK_SIZE_96KBYTE  ,    80,         0,       80  },
                            { DMM_BLOCK_SIZE_128KBYTE ,    70,         0,       80  },
                            { DMM_BLOCK_SIZE_192KBYTE ,    60,         0,       80  },
                            { DMM_BLOCK_SIZE_256KBYTE ,    50,         0,       80  },
                            { DMM_BLOCK_SIZE_384KBYTE ,    40,         0,       80  },
                            { DMM_BLOCK_SIZE_512KBYTE ,    30,         0,       80  },
                            { DMM_BLOCK_SIZE_768KBYTE ,    20,         0,       80  },
                            { DMM_BLOCK_SIZE_1MKBYTE  ,    10,         0,       80  }
                        };
                            
    MemPool_Init(listinfo, sizeof(listinfo) / sizeof(listinfo[0]));
}

