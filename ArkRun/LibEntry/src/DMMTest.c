/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : DMMTest.c                                                      */
/*  Descript :                                                                */
/*  API Pref : TDMM_                                                          */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.18                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include <stdio.h>
#include <string.h>

#include "Arch.h"
#include "VarType.h"
#include "Shell.h"
#include "MemoryPool.h"
#include "Maths.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/
static UINT32 NumArray[0x1000000] = {0};

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/
static UINT32 TestDMM(UINT32 argc, UINT8 *argv[]);

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
UINT32 TDMM_RandomMalloc(UINT32 argc, UINT8 *argv[])
{
    UINT32 index = 0;
    void *p = NULL;
    UINT32 RandomNum;

    while (1)
    {
        //usleep(index);
        RandomNum = Math_Random(index++) % 0x1000000;
        NumArray[RandomNum]++;

        p = (void *)MemPool_Malloc(RandomNum);
        if (NULL != p)
        {
            MemPool_Free(p);
        }

        if ((index % 1000) == 0)
        {
            printf("===========================\n");
            TestDMM(0, NULL);
        }
    }
    
    return 0;
}

/*============================================================================*/
/*  Function : AddShellDynTestCmd                                             */
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
void AddShellDynTestCmd(void)
{
    TShell ShellCmd;

    ShellCmd.MaxParaNum = 0;
    ShellCmd.pfunc = TDMM_RandomMalloc;
    memcpy(ShellCmd.ucCmdDesc, "testdmm", SHELL_CMD_STR_MAX_LEN * 3);
    memcpy(ShellCmd.ucCmdstr, "testdmm", SHELL_CMD_STR_MAX_LEN);
    Sh_AddCmd(ShellCmd);
    
    return;
}

/*============================================================================*/
/*  Function : TestDMM                                                        */
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
static UINT32 TestDMM(UINT32 argc, UINT8 *argv[])
{
    UINT32 index = 0;

    for (index = 0; index < 0x1000000; ++index)
    {
        if (NumArray[index] != 0)
        {
            printf("Random:0x%0x\t\t, %d\n", index, NumArray[index]);
        }
    }

	return 0;
}
/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


