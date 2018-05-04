/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : TimeTag.h                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.17                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/
#ifndef _TIMETAG_H
#define _TIMETAG_H

/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/
#define TC_HEADNODE_DESCLEN     32
#define TC_HEADNODE_MAXNUM      20
#define TC_COUNTERNODE_MAXNUM   600


#define TC_TAG_OPEN_FLAG        1
#define TC_TAG_CLOSE_FLAG       0

#define TC_TAG_RECORD_BEGIN     0
#define TC_TAG_RECORD_END       1

#define TC_ADDITEM_IS_FIRST     0
#define TC_ADDITEM_NOT_FIRST    1

/*============================================================================*/
/* Globoal Data Type                                                          */
/*============================================================================*/
/* Time Tag Struct */
typedef struct
{
    struct timeval begintime;
    struct timeval endtime;
}TtimeTag;

/* Register Head Node By User */
typedef struct
{
    UINT8 m_ucHEADNodeDesc[TC_HEADNODE_DESCLEN];/* Head Node Desc             */
    UINT32 m_ulHeadNodeID;                      /* Head Node ID               */
    UINT32 m_ulCountNodeNum;                    /* Sub Head Node Num Of Head  */
}TRegHeadInfo;

/* Head Node Info By Progamme */
typedef struct
{ 
    TRegHeadInfo m_tHeadInfo;                   /* Head Node Info By User     */
    UINT32 m_TagRecordTimes;                    /* Record Times Of Time Tag   */
    UINT32 m_TagRunFlag;                        /* This Tag Is Run Or not     */
    TtimeTag *m_pTimeTag;                       /* Store                      */
}TCountHeadNode;

/*============================================================================*/
/* Api Declare                                                                */
/*============================================================================*/
UINT32 TC_AddTimeTag(TRegHeadInfo tHeadNode);

UINT32 TC_SetTimeTag(UINT32 ulTagTypeID, UINT32 ulBeginOrEndOfTag);

INT32 TC_SetTimeTagStatus(INT32 argc, INT8** argv);

INT32 TC_PrintTimeTagInfo(INT32 argc, INT8** argv);

void TC_TimeFormate(UINT8 *pucStrTimeBuff, UINT8 ucStrLen, UINT8 *pucTimeFormate);

UINT32 TC_CalcUsOfTime(TtimeTag tTimeTag);

UINT32 GetCurrentSecondSOfToday(void);

#endif

