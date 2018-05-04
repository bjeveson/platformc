/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : Shell.c                                                        */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Arch.h"
#include "VarType.h"
#include "ErrorCode.h"
#include "List.h"
#include "Shell.h"
#include "MemoryPool.h"
#include "Strings.h"

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static PLNode pShellCmdList = NULL;

static PLNode pThreadInfoList = NULL;

static UINT32 g_CurThreadNum = 0;

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static UINT32 ListNode_Compare(void *e1, void *e2);

static UINT32 Help(UINT32 argc, UINT8 *argv[]);

static void printShellNode(void *pData);

static UINT32 Quit(UINT32 argc, UINT8 *argv[]);

static void AddShellCmd(void);

static UINT32 ShowThread(UINT32 argc, UINT8 *argv[]);

static void ThreadInfoList_PrintData(void *pdata);
/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Sh_AddCmd                                                      */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.18                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 Sh_AddCmd(TShell ShellNode)
{
    UINT32 ulRet = LIB_ERR_SUCCESS;
    TShell *pSheelNode = NULL;
    PLNode pNode;

    if (NULL == pShellCmdList)
    {
        ulRet = List_Init(&pShellCmdList);
        if (LIB_ERR_SUCCESS != ulRet)
        {
            printf("List Create Error\n");
            return ulRet;
        }
    }

    pSheelNode = (TShell *)MemPool_Malloc(sizeof(TShell));
    if (NULL == pSheelNode)
    {
        printf("Create Node Error\n");
        return LIB_ERR_POINTER_NULL;
    }

    *pSheelNode = ShellNode;

    pNode = List_InsertNodeAtTail(pShellCmdList, pSheelNode);
    if (NULL != pNode)
    {
        return LIB_ERR_SUCCESS;
    }
    else
    {
        return LIB_ERR_POINTER_NULL;
    }
}

/*============================================================================*/
/*  Function : Sh_Run                                                         */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.18                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
void Sh_Run(const INT8 *notice)
{   
    UINT8 ucCmdStr[SHELL_CMD_STR_MAX_LEN] = {0};
    UINT8 *pucCmdParaPos[SHELL_CMD_PARA_MAX_NUM] = {NULL};
    UINT8 ucCmdParaStr[SHELL_CMD_PARA_MAX_NUM][SHELL_CMD_STR_MAX_LEN];
    UINT32 VectorIndex = 0;
    UINT32 ParaNum = 0;
    TShell *pShellInfo = NULL;
    UINT32 ulRet = LIB_ERR_SUCCESS;

    AddShellCmd();
    
    for (VectorIndex= 0; VectorIndex < SHELL_CMD_PARA_MAX_NUM; VectorIndex++)
    {
        pucCmdParaPos[VectorIndex]= ucCmdParaStr[VectorIndex];
    }
    
    while (1)
    {
        printf("%s", notice);
        fgets((INT8 *)ucCmdStr, SHELL_CMD_STR_MAX_LEN, stdin);

        ParaNum = Str_CmdToVector(ucCmdStr, SHELL_CMD_PARA_MAX_NUM, pucCmdParaPos);
        if (0 == ParaNum)
        {
            continue;
        }

        if (ParaNum > SHELL_CMD_PARA_MAX_NUM)
        {
            printf("Para Num Too Much\n");
            continue;
        }
        
        ulRet = List_FindCurNodeByData(pShellCmdList, 
                                     pucCmdParaPos[0],
                                     (void **)&pShellInfo, 
                                     ListNode_Compare);
        
        if (LIST_FIND_OK != ulRet)
        {
            printf("Command Bad And Not Fount\n");
            continue;
        }

        if (ParaNum > pShellInfo->MaxParaNum + 1)
        {
            printf("Command Para Num Must Be:%d\n", pShellInfo->MaxParaNum);
            continue;
        }
        
        if (NULL == pShellInfo->pfunc)
        {
            printf("No Function Execute\n");
            continue;
        }
        
        ulRet = pShellInfo->pfunc(ParaNum, pucCmdParaPos);
        if (LIB_ERR_SUCCESS != ulRet)
        {
            printf("Command Exceute Failed\n");
            continue;
        }
    }
    
    return;
}

/*============================================================================*/
/*  Function : AddShellCmd                                                    */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void AddShellCmd(void)
{
    TShell ShellCmd;

    ShellCmd.MaxParaNum = 0;
    ShellCmd.pfunc = Help;
    memcpy(ShellCmd.ucCmdDesc, "help", SHELL_CMD_STR_MAX_LEN * 3);
    memcpy(ShellCmd.ucCmdstr, "help", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);

    ShellCmd.MaxParaNum = 0;
    ShellCmd.pfunc = Quit;
    memcpy(ShellCmd.ucCmdDesc, "quit", SHELL_CMD_STR_MAX_LEN * 3);
    memcpy(ShellCmd.ucCmdstr, "quit", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);

    ShellCmd.MaxParaNum = 0;
    ShellCmd.pfunc = ShowThread;
    memcpy(ShellCmd.ucCmdDesc, "thread", SHELL_CMD_STR_MAX_LEN * 3);
    memcpy(ShellCmd.ucCmdstr, "thread", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);   
    
    return;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/
/*============================================================================*/
/*  Function : ListNode_Compare                                               */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 ListNode_Compare(void *e1, void *e2)
{
    TShell *pShellInfo = (TShell *)e1;
    UINT8 *pCurCmd = (UINT8 *)e2;

    if (0 == strcmp(pShellInfo->ucCmdstr, pCurCmd))
    {
        return LIST_DATA_EQUAL;
    }
    else
    {
        return LIST_DATA_NOT_EQUAL;
    }
}

/*============================================================================*/
/*  Function : Hellp                                                          */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 Help(UINT32 argc, UINT8 *argv[])
{   
    printf("---------------------------------------------\n");
    
    List_Traverse(pShellCmdList, printShellNode);

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : printShellNode                                                 */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static void printShellNode(void *pData)
{
    TShell *pShellNode = (TShell *)pData;

    if (NULL == pData)
    {
        return;
    }

    printf("%-10.32s", pShellNode->ucCmdstr);
    printf("        ");
    printf("%-10.120s\n", pShellNode->ucCmdDesc);
}

/*============================================================================*/
/*  Function : Quit                                                           */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.26                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
static UINT32 Quit(UINT32 argc, UINT8 *argv[])
{
    UINT32 loop = 0;
    
    ThreadPool_Destroy(-1);

    DBConPool_Destroy();

    MemPool_DestroyMem();

    MsgQue_AllDestroy();

    printf("Server Quit!\n");
    
    exit(0);

    return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  函数名 : Sh_StoreThreadInfo                                               */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.6                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
void Sh_AddThreadInfo(TThreadInfo Thread)
{
    UINT32 ret;
    TThreadInfo *ThreadInfoNode = NULL;
    
    if (NULL == pThreadInfoList)
    {
        ret = List_Init(&pThreadInfoList);
        if (LIB_ERR_SUCCESS != ret)
        {
            printf("Thread Info List Init Failed\n");
            return;
        }
    }

    ThreadInfoNode = (TThreadInfo *)MemPool_Malloc(sizeof(TThreadInfo));
    if (NULL == ThreadInfoNode)
    {
        printf("Thread Info Node Malloc Failed\n");
        return;
    }

    *ThreadInfoNode = Thread;
    
    List_InsertNodeAtTail(pThreadInfoList, (void *)ThreadInfoNode);    
    
    return;
}

/*============================================================================*/
/*  函数名 : ShowThread                                                       */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : Huyongfu                                                         */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.6                                                        */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT32 ShowThread(UINT32 argc, UINT8 *argv[])
{
    printf("ThreadID       desc\n");
    List_Traverse(pThreadInfoList, ThreadInfoList_PrintData);

    return 0;
}

static void ThreadInfoList_PrintData(void *pdata)
{
    TThreadInfo *ThreadInfoNode = pdata;
    
    printf("0x%x     %s\n", (UINT32)ThreadInfoNode->pid, ThreadInfoNode->desc);

    return;
}
