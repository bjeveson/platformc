/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾���䱱������������ */
/*  ϵͳ���� : ZXONE 8600�⴫��ϵͳ                                           */
/*  �� �� �� : Queue.c                                                        */
/*  �������� :                                                                */
/*  �� �� �� : ������                                                         */
/*  �ļ��汾 : V1.00.001                                                      */
/*  �������� : 2014.11.24                                                     */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/

/*============================================================================*/
/* �����ı�׼����������Ǳ�׼��ͷ�ļ�                                         */
/*============================================================================*/
#include <stdlib.h>
#include <stdio.h>

#include "VarType.h"
#include "MemoryPool.h"
#include "Queue.h"
/*============================================================================*/
/* ���ļ�ʹ�õĳ�������                                                       */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õĺ궨��                                                         */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õ���������                                                       */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֱ���                                                                   */
/*============================================================================*/

/*============================================================================*/
/* ���ļ�ʹ�õı���                                                           */
/*============================================================================*/

/*============================================================================*/
/* ���ʵ��                                                                   */
/*============================================================================*/

/*============================================================================*/
/*  ������ : Queue_Init                                                       */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
Queue *Queue_Init(void)
{
	Queue *pqueue = (Queue *)MemPool_Malloc(sizeof(Queue));

	if(pqueue != NULL)
	{
		pqueue->front = NULL;
		pqueue->rear = NULL;
		pqueue->size = 0;
	}
	
	return pqueue;
}

/*============================================================================*/
/*  ������ : Queue_Destroy                                                    */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
void Queue_Destroy(Queue *pqueue)
{
	if (Queue_IsEmpty(pqueue)!=1)
	{
		Queue_Clear(pqueue);
    }
    
	MemPool_Free(pqueue);
}

/*============================================================================*/
/*  ������ : Queue_Clear                                                      */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
void Queue_Clear(Queue *pqueue)
{
	while (Queue_IsEmpty(pqueue) != 1)
	{
		Queue_Pop(pqueue,NULL);
	}

    return;
}

/*============================================================================*/
/*  ������ : Queue_IsEmpty                                                    */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
UINT32 Queue_IsEmpty(Queue *pqueue)
{
	if (pqueue->front == NULL && pqueue->rear == NULL && pqueue->size == 0)
	{
		return 1;
    }
	else
	{
		return 0;
    }
}

/*============================================================================*/
/*  ������ : Queue_GetSize                                                    */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
UINT32 Queue_GetSize(Queue *pqueue)
{
	return pqueue->size;
}

/*============================================================================*/
/*  ������ : Queue_GetFront                                                   */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
PNode Queue_GetFront(Queue *pqueue, void **pitem)
{
	if (Queue_IsEmpty(pqueue) != 1 && pitem != NULL && *pitem != NULL)
	{
		*pitem = pqueue->front->data;
	}
	
	return pqueue->front;
}

/*============================================================================*/
/*  ������ : Queue_GetRear                                                    */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
PNode Queue_GetRear(Queue *pqueue, void **pitem)
{
	if (Queue_IsEmpty(pqueue) != 1 && pitem != NULL)
	{
		*pitem = pqueue->rear->data;
	}
	
	return pqueue->rear;
}

/*============================================================================*/
/*  ������ : Queue_Push                                                       */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
PNode Queue_Push(Queue *pqueue, void *pitem)
{
	PNode pnode = (PNode)MemPool_Malloc(sizeof(Node));
	
	if(pnode != NULL)
	{
		pnode->data = pitem;
		pnode->Next = NULL;
		
		if(Queue_IsEmpty(pqueue))
		{
			pqueue->front = pnode;
		}
		else
		{
			pqueue->rear->Next = pnode;
		}
		
		pqueue->rear = pnode;
		pqueue->size++;
	}
	
	return pnode;
}

/*============================================================================*/
/*  ������ : Queue_Pop                                                        */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
PNode Queue_Pop(Queue *pqueue,void **pitem)
{
	PNode pnode = pqueue->front;
	
	if (Queue_IsEmpty(pqueue) != 1 && pnode != NULL)
	{
		if (pitem != NULL)
		{
			*pitem = pnode->data;
        }
        
		pqueue->size--;
		pqueue->front = pnode->Next;
		MemPool_Free(pnode);
		
		if (pqueue->size == 0)
		{
			pqueue->rear = NULL;
        }
	}
	
	return pqueue->front;
}

/*============================================================================*/
/*  ������ : Queue_Traverse                                                   */
/*  ��  �� : ���������ṹ��˵��                                             */
/*  ��  �� : ��                                                               */
/*  ����ֵ : ִ�н��: 0-�ɹ�, ��0-ʧ��                                       */
/*  ��  �� : ������������                                                     */
/*  ��  �� : ������                                                           */
/*  ��  �� : V1.00.001                                                        */
/*  ��  �� : 2014.11.24                                                       */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����            �汾            �޸���          �޸�ժҪ                  */
/*============================================================================*/
void Queue_Traverse(Queue *pqueue,void (*visit)())
{
	PNode pnode = pqueue->front;
	int i = pqueue->size;
	
	while (i--)
	{
		visit(pnode->data);
		pnode = pnode->Next;
	}

    return;
}
