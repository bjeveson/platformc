/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm : 					  My Lib                                      */
/*  File     : LibEntry.c                                                     */
/*  Descript : Entry Of Run Lib                                               */
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
#include <stdlib.h>
#include <string.h>

#include "Arch.h"
#include "VarType.h"
#include "OSI.h"
#include "ErrorCode.h"
#include "Log.h"
#include "TimeTag.h"
#include "Shell.h"
#include "LibEntry.h"
#include "MemoryPool.h"
#include "DMMTest.h"
#include "FileSys.h"
/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static UINT32 LogInit(void);

static void DMMInit(void);
/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/

/*============================================================================*/
/*  FUNCTION   : main                                                         */
/*  INPUT      :                                                              */
/*  OUTPUT     :                                                              */
/*  RETURN     :                                                              */
/*  DESCRIPTION:                                                              */
/*  AUTHOR     : HuYongFu                                                     */
/*  VERSION    : V1.00                                                        */
/*  DATE       : 2013.7.17                                                    */
/*============================================================================*/
/*  MODIFY LOG:MUST HAVE DATE AUTHOR DESCRIPT                                 */
/*  1.                                                                        */
/*============================================================================*/
INT32 main(INT32 argc, INT8* argv[])
{
    UINT32 ulRet = LIB_ERR_SUCCESS;

    //ulRet = LogInit();
    DMMInit();

    AddShellDynTestCmd();
    
    Sh_Run();

    return ulRet;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/

/*============================================================================*/
/*  Function : LogInit                                                        */
/*  Input    :                                                                */
/*  Output   :                                                                */
/*  Return   :                                                                */
/*  Desc     :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 LogInit(void)
{
    TLogRegInfo tLogInfo;
    UINT8 *pucLogFilePreFix = "MyLib";
    UINT8 ucCurDir[OSI_PATH_MAX];
    
    FS_GetCurDir(ucCurDir);
    
    memcpy((void *)tLogInfo.m_ucLogFilePath, 
           (void *)ucCurDir,
           strlen(ucCurDir) + 1);
    memcpy((void *)tLogInfo.m_ucFileNamePrefix,
           (void *)pucLogFilePreFix,
           strlen(pucLogFilePreFix) + 1);
    tLogInfo.m_ulCurOutLevel = LOG_LEVEL_INFO;
    tLogInfo.m_ulCurOutPos = LOG_OUTPOS_FILE;
    tLogInfo.m_ulLogFileSize = 1024;
    tLogInfo.m_ulLogServerIp = 0;
    tLogInfo.m_ulLogServerPort = 0;
    tLogInfo.m_ulFlushFileEveryNLogs = 100;

    return Log_Init(tLogInfo);
}

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

