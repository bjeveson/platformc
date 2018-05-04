/*============================================================================*/
/*  ��Ȩ���� : Copyright (c) 1998-2008 ����ͨѶ�ɷ����޹�˾���䱱������������ */
/*  ϵͳ���� : ZXONE 8600�⴫��ϵͳ                                           */
/*  �� �� �� : ThreadPool.h                                                   */
/*  �������� :                                                                */
/*  �� �� �� : ������                                                         */
/*  �ļ��汾 : V1.00.001                                                      */
/*  �������� : 2014.10.21                                                     */
/*============================================================================*/
/*  �޸ļ�¼                                                                  */
/*  ����         �汾            �޸���          �޸�ժҪ                     */
/*============================================================================*/
#ifndef _THREADPOOL_H
#define _THREADPOOL_H

/*============================================================================*/
/* �����ı�׼����������Ǳ�׼��ͷ�ļ�                                         */
/*============================================================================*/

/*============================================================================*/
/* ��������                                                                   */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֺ�                                                                     */
/*============================================================================*/
#define THREAD_MAX_POOLS_NUM        32
/*============================================================================*/
/* ȫ����������                                                               */
/*============================================================================*/
typedef struct worker 
{ 
	void *(*process) (void *arg); 
	void *arg; 
	struct worker *next; 
} CThread_worker; 

typedef struct 
{ 
	pthread_mutex_t queue_lock; 
	pthread_cond_t queue_ready; 
	CThread_worker *queue_head; 
	INT32 shutdown; 
	pthread_t *threadid; 
	INT32 max_thread_num; 
	INT32 cur_queue_size; 
	INT32 max_queue_size;
}CThread_pool; 

/*============================================================================*/
/* �ඨ��                                                                     */
/*============================================================================*/

/*============================================================================*/
/* ģ��(template)(����C++�е���ģ��ͺ���ģ��)                                */
/*============================================================================*/

/*============================================================================*/
/* ȫ�ֺ���ԭ��                                                               */
/*============================================================================*/
INT32 ThreadPool_AddWorker(void *(*process)(void *arg), void *arg, INT32 PoolId); 

INT32 ThreadPool_Init(INT32 max_thread_num, INT32 max_queue_size);

INT32 ThreadPool_Destroy(INT32 PoolId);

#endif