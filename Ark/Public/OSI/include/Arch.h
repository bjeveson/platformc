/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : CPUI.h                                                         */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.10.15                                                     */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _ARCH_H
#define _ARCH_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
/* Complie Switch Of OS */
#define COMPILE_ON_PLATFORM  WINDOWS32

#define LINUX_KERNEL        1
#define LINUX_USER          2
#define WINDOWS32           3

#define _X86_

/* CPU Type */
#define CPU_STRUCTURE        CPU_X86
#define CPU_X86              1
#define CPU_POWERPC603       2

#define DEBUG_MODULE
#define DEBUG_ON    1
#define DEBUG_OFF   0

#define OSI_LACK_CPU_TYPE "Not Define CPU Type!"

/* Jump Instruction Length */
#if (CPU_STRUCTURE == CPU_X86)
#define JUMP_INSTRUCTION_LEN   5
#elif (CPU_STRUCTURE == CPU_POWERPC603)
#define JUMP_INSTRUCTION_LEN   4
#else
#error OSI_LACK_CPU_TYPE
#endif
/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/

#endif
