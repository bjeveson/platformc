/*============================================================================*/
/*  版权声明 : Copyright (c) 1998-2008 中兴通讯股份有限公司传输北京软件开发部 */
/*  系统名称 : ZXONE 8600光传输系统                                           */
/*  文 件 名 : RdCfgFile.c                                                    */
/*  功能描述 :                                                                */
/*  创 建 人 : 胡永福                                                         */
/*  文件版本 : V1.00.00                                                       */
/*  创建日期 : 2014.12.13                                                     */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期         版本            修改人          修改摘要                     */
/*============================================================================*/

/*============================================================================*/
/* 包含的标准库或者其他非标准库头文件                                         */
/*============================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>

#include "VarType.h"
#include "Log.h"
#include "RdCfgFile.h"
/*============================================================================*/
/* 本文件使用的常量定义                                                       */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的宏定义                                                         */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的数据类型                                                       */
/*============================================================================*/

/*============================================================================*/
/* 全局变量                                                                   */
/*============================================================================*/

/*============================================================================*/
/* 本文件使用的变量                                                           */
/*============================================================================*/

/*============================================================================*/
/* 类的实现                                                                   */
/*============================================================================*/
/*   删除左边的空格   */
static UINT8 * l_trim(UINT8 * szOutput, const UINT8 *szInput);
/*   删除右边的空格   */
static UINT8 *r_trim(UINT8 *szOutput, const UINT8 *szInput);
/*   删除两边的空格   **/
static UINT8 * a_trim(UINT8 * szOutput, const UINT8 * szInput);

/*============================================================================*/
/*  函数名 : l_trim                                                           */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 删除左边的空格                                                   */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.13                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT8 * l_trim(UINT8 * szOutput, const UINT8 *szInput)
{
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    
    for (NULL; *szInput != '\0' && isspace(*szInput); ++szInput)   ;
    
    return strcpy(szOutput, szInput);
}

/*============================================================================*/
/*  函数名 : r_trim                                                           */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 删除右边的空格                                                   */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.13                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT8 *r_trim(UINT8 *szOutput, const UINT8 *szInput)
{
    UINT8 *p = NULL;
    
    assert(szInput != NULL);
    assert(szOutput != NULL);
    assert(szOutput != szInput);
    strcpy(szOutput, szInput);
    for(p = szOutput + strlen(szOutput) - 1; p >= szOutput && isspace(*p); --p) ;
    *(++p) = '\0';
    
    return szOutput;
}

/*============================================================================*/
/*  函数名 : a_trim                                                           */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 删除两边的空格                                                   */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.13                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
static UINT8 * a_trim(UINT8 * szOutput, const UINT8 * szInput)
{
    UINT8 *p = NULL;
    
    assert(szInput != NULL);
    assert(szOutput != NULL);
    l_trim(szOutput, szInput);
    for (p = szOutput + strlen(szOutput) - 1;p >= szOutput && isspace(*p); --p) ;
    *(++p) = '\0';
    
    return szOutput;
}

/*============================================================================*/
/*  函数名 : GetProfileString                                                 */
/*  输  入 : 输入参数或结构的说明                                             */
/*  输  出 : 无                                                               */
/*  返回值 : 执行结果: 0-成功, 非0-失败                                       */
/*  描  述 : 函数功能描述                                                     */
/*  作  者 : 胡永福                                                           */
/*  版  本 : V1.00.00                                                         */
/*  日  期 : 2014.12.13                                                       */
/*============================================================================*/
/*  修改记录                                                                  */
/*  日期            版本            修改人          修改摘要                  */
/*============================================================================*/
INT32 GetProfileString(UINT8 *profile, UINT8 *AppName, UINT8 *KeyName, UINT8 *KeyVal)
{
    UINT8 appname[32], keyname[32];
    UINT8 *buf, *c;
    UINT8 buf_i[KEYVALLEN], buf_o[KEYVALLEN];
    UINT8 *KeyVal_o;
    FILE *fp;
    INT32 found = 0; /* 1 AppName 2 KeyName */
    
    if((fp = fopen(profile, "r")) == NULL)
    {
        LOG_PRINT(LOG_LEVEL_ERROR, "openfile [%s] error [%s]\n", profile, strerror(errno));
        return -1;
    }
    
    fseek(fp, 0, SEEK_SET);
    memset(appname, 0, sizeof(appname));
    sprintf(appname,"[%s]", AppName);

    while (!feof(fp) && fgets(buf_i, KEYVALLEN, fp ) != NULL)
    {
        l_trim(buf_o, buf_i);
        if (strlen(buf_o) <= 0)
            continue;
        buf = NULL;
        buf = buf_o;

        if (found == 0)
        {
            if (buf[0] != '[')
            {
                continue;
            } 
            else if (strncmp(buf, appname, strlen(appname)) == 0)
            {
                found = 1;
                continue;
            }
        } 
        else if (found == 1)
        {
            if (buf[0] == '#' )
            {
                continue;
            }
            else if (buf[0] == '[')
            {
                break;
            } 
            else
            {
                if ((c = (char *)strchr(buf, '=')) == NULL)
                    continue;
                    
                memset(keyname, 0, sizeof(keyname));
                sscanf(buf, "%[^=|^ |^\t]", keyname);
                
                if (strcmp(keyname, KeyName) == 0)
                {
                    sscanf(++c, "%[^\n]", KeyVal);
                    KeyVal_o = (UINT8 *)malloc(strlen(KeyVal) + 1);
                    if (KeyVal_o != NULL)
                    {
                        memset(KeyVal_o, 0, sizeof(KeyVal_o));
                        a_trim(KeyVal_o, KeyVal);
                        if (KeyVal_o && strlen(KeyVal_o) > 0)
                            strcpy(KeyVal, KeyVal_o);
                        free(KeyVal_o);
                        KeyVal_o = NULL;
                    }
                    
                    found = 2;
                    break;
                } 
                else
                {
                    continue;
                }
            }
        }
    }
    
    fclose( fp );

    if( found == 2 )
        return(0);
    else
        return(-1);
}

