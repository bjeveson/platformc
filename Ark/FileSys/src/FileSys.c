/*============================================================================*/
/*  Copyright:           (c) 2008 -2014  By HuYongFu                          */
/*  Programm :                 My Lib                                         */
/*  File     : FileSys.c                                                      */
/*  Descript :                                                                */
/*  API Pref :                                                                */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.10                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Descript                                 */
/*  1.                                                                        */
/*============================================================================*/

/*============================================================================*/
/* Include Head File,Order:Lib Head File,Public Head File,Private Head File   */
/*============================================================================*/
#include "Arch.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
	#include <unistd.h>
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
	#include <windows.h>
#else
	#error OSI_LACK_OS_ENVIRMENT
#endif
#include "VarType.h"
#include "ErrorCode.h"

#include "FileSys.h"
/*============================================================================*/
/* Macro Define                                                               */
/*============================================================================*/

/*============================================================================*/
/* Gloabal Var                                                                */
/*============================================================================*/

/*============================================================================*/
/* Data Type In This File                                                     */
/*============================================================================*/

/*============================================================================*/
/* Static Function Declare                                                    */
/*============================================================================*/

/*============================================================================*/
/* Non-Static Function Code                                                   */
/*============================================================================*/
/*============================================================================*/
/*  Function : Str_ParseCfgFile                                               */
/*  Input    : %1 -- file path and name                                       */
/*             %2 -- to search key                                            */
/*             %3 -- number of key                                            */
/*  Output   : put into value[][] of key[]                                    */
/*  Return   : Error Code                                                     */
/*  Desc     : read config file as key[],                                     */
/*             every line like this:ip = 192.168.0.110                        */ 
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.24                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 FS_ParseCfgFile(const UINT8 *filename ,
                       const UINT8 *key[],
                       UINT32 len,
                       UINT8 value[][255])
{
	FILE *fp;
	UINT8 buffer[255], row[255], temp[255], *p, *q;
	UINT32 i, m, n, k;

	if((fp = fopen(filename,"rb")) == NULL) 
	{
		perror ("file open failed.\n");
		return LIB_ERR_FILE_OPEN_FAILED;
	}

	i = k = 0;

	while (fgets(row,255,fp) != NULL) 
	{
		q = row;
		m = n = 0;

		while(*q != '\0') 
		{		/* delete the space char in every row */
			if(*q != ' ') 
			{
				buffer[m] = row[n];
				m++;
				n++;
			} 
			else
			{
				n++;
			}
			q++;
		}

		buffer[m - 1] = '\0';        /* delete the \n char. */

		p = buffer;
		k = 0;

		while (*p != '\0') 
		{		//part the variable and relevant value.
			temp[k] = *p;

			if(*p == '=') 
			{
				temp[k] = '\0';
				for (i = 0; i < len; i++) 
				{
					if(strcmp(temp,key[i]) == 0)
					{
						strcpy(value[i],p + 1);
						break;
					}
				}
			}

			k++;
			p++;
		}
	}

	fclose (fp);

	return LIB_ERR_SUCCESS;
}

/*============================================================================*/
/*  Function : Str_FileType                                                   */
/*  Input    : %1 -- the path of file while include file name                 */
/*  Output   : %2 -- return argument,store the postfix of file name           */
/*  Return   : File type                                                      */
/*  Desc     : return file type,and return postfix of file name               */
/*  Author   : HuYongFu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.7.25                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 FS_FileType(const UINT8 *path, UINT8 *type)
{
	const UINT8 *fname,*p;
	UINT32 i, j = 0;;

	for (p = &path[strlen(path) + 1];*p != '/';p--) ;/* obtain the file name */

	fname = p + 1;

	for (i = 0;fname[i] != '\0';i++) 
	{		/* obtain the postfix */
		if (fname[i] == '.') 
		{
			strcpy(type, &fname[i + 1]);
			break;
		}
	}

	if (fname[i] == '\0')
	{
		return STRING_FILE_TYPE_NULL;
	}
	else if (strcmp(type,"jpg") == 0 
	         || strcmp(type,"png") == 0 
	         || strcmp(type,"gif") == 0)		/* picture file type */
	{
		return STRING_FILE_TYPE_PICTURE;
	}
	else if (strcmp(type,"txt") == 0 
	         || strcmp(type,"doc") == 0)		/* text file type */
    {
		return STRING_FILE_TYPE_TEXT;
	}
	else
	{
		return STRING_FILE_TYPE_UNKNOW;		/* others file type */
	}
}

/*============================================================================*/
/*  Function : FS_GetCurDir                                                   */
/*  Input    : %1 --                                                          */
/*  Output   :                                                                */
/*  Return   : Error Code                                                     */
/*  Desc     :                                                                */
/*  Author   : HuYongfu                                                       */
/*  Version  : V1.00                                                          */
/*  Date     : 2013.9.11                                                      */
/*============================================================================*/
/*  Modify Log:Must Have Date Author Description                              */
/*  1.                                                                        */
/*============================================================================*/
UINT32 FS_GetCurDir(UINT8 *pucDir)
{
    UINT32 size = 0;

#if (COMPILE_ON_PLATFORM == LINUX_KERNEL || COMPILE_ON_PLATFORM == LINUX_USER)
    if ((size = pathconf("./", _PC_PATH_MAX)) <= 0)
    {
         printf("pathconf error for _PC_PATH_MAX\n");
         return -1;
    }

    getcwd(pucDir, size);
#elif (COMPILE_ON_PLATFORM == WINDOWS32)
	size = GetModuleFileName(NULL, (LPWSTR)pucDir, 255);
	if (0 == size)
	{
		printf("GetModuleFileName failed (%d)\n", GetLastError());
		return 0;
	}
#else
	#error OSI_LACK_OS_ENVIRMENT
#endif
	strcat(pucDir, "/");

    return size + 1 + 1;
}

/*============================================================================*/
/* Static Function Code                                                       */
/*============================================================================*/


