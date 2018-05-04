/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TestDrv.h                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _TESTDRV_H
#define _TESTDRV_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define TD_TESTCASE_MAX_NUM         1000
#define TD_TESTCASE_DESC_LEN        64

#define TD_ASSERT_EQUAL(var, value) \
        {\
            if ((var) != (value))\
            {\
                LOG_PRINT(LOG_LEVEL_INFO,\
                          "Error:%d != %d\n",\
                          var, \
                          value);\
                TD_TestCaseFailed();\
            }\
            else\
            {\
                LOG_PRINT(LOG_LEVEL_INFO, "Test Pass\n");\
                TD_TestCasePass();\
            }\
        }

#define TD_ASSERT_NOT_EQUAL(var, value) \
        {\
            if ((var) == (value))\
            {\
                LOG_PRINT(LOG_LEVEL_INFO,\
                          "Error:%d == %d\n",\
                          var, \
                          value);\
                TD_TestCaseFailed();\
            }\
            else\
            {\
                LOG_PRINT(LOG_LEVEL_INFO, "Test Pass\n");\
                TD_TestCasePass();\
            }\
        }

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
typedef void (*Func_TestCase)(void *Para);        

typedef struct
{
    UINT8 m_ucTestCaseDesc[TD_TESTCASE_DESC_LEN];
    Func_TestCase m_pFuncTestCase;
    void *Para;
}TTestCase;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 TD_AddTestCase(TTestCase *pTestCase);

void TD_RunAllTestCase(void);

void TD_TestCasePass(void);

void TD_TestCaseFailed(void);

#endif

