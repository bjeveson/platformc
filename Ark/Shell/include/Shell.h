/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Shell.h                                                        */
/*  Descript :                                                                */
/*  API Pref : SH_                                                            */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _SHELL_H
#define _SHELL_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define SHELL_CMD_STR_MAX_LEN        32
#define SHELL_CMD_PARA_MAX_NUM       8

#define SHELL_THREAD_NUM_MAX        64
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef UINT32 (* pfn_Shell)(UINT32 argc, UINT8 *argv[]);

typedef struct
{
    UINT8 ucCmdstr[SHELL_CMD_STR_MAX_LEN];
    UINT32 MaxParaNum;
    UINT8 ucCmdDesc[SHELL_CMD_STR_MAX_LEN * 5];
    pfn_Shell pfunc;
}TShell;

typedef struct
{
    pthread_t pid;
    UINT8 desc[64];
}TThreadInfo;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
void Sh_Run(const INT8 *notice);

UINT32 Sh_AddCmd(TShell ShellNode);

void Sh_StoreThreadInfo(pthread_t *pThreadID, UINT32 Num);

#endif

