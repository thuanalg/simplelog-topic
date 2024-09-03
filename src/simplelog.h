/*===============================================================================================================*/
/* Email:
*		<nguyenthaithuanalg@gmail.com> - Nguyễn Thái Thuận
* Mobile:
*		<+084.799.324.179>
* Skype:
*		<nguyenthaithuanalg>
* Date:
*		<2024-July-14>
* The lasted modified date:
*		<2024-Sep-03>
* Decription:
*		The (only) main header file to export 3 APIs: [spl_init_log, spllog, spllogtopic, spl_finish_log].
*/
/*===============================================================================================================*/
#ifndef ___SIMPLE_LOG__
#define ___SIMPLE_LOG__
#include <stdio.h>
#include "simplelog_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LLU				unsigned long long

#define					SPL_LOG_BASE					0
#define					SPL_LOG_DEBUG					1
#define					SPL_LOG_INFO					2
#define					SPL_LOG_WARNING					3
#define					SPL_LOG_ERROR					4
#define					SPL_LOG_FATAL					5
#define					SPL_LOG_PEAK					6

#ifndef  UNIX_LINUX
	#ifndef __SIMPLE_STATIC_LOG__
		#ifdef EXPORT_DLL_API_SIMPLE_LOG
			#define DLL_API_SIMPLE_LOG		__declspec(dllexport)
		#else
			#define DLL_API_SIMPLE_LOG		__declspec(dllimport)
		#endif
	#else
	#define DLL_API_SIMPLE_LOG
	#endif
#else
	#define DLL_API_SIMPLE_LOG
#endif /*! UNIX_LINUX */ 


	typedef enum __SPL_LOG_ERR_CODE__ {
		SPL_NO_ERROR = 0,
		SPL_INIT_PATH_FOLDER_EMPTY_ERROR,
		SPL_LOG_LEVEL_ERROR,
		SPL_ERROR_CREATE_MUTEX,
		SPL_ERROR_CREATE_SEM,
		SPL_LOG_BUFF_SIZE_ERROR,
		SPL_LOG_FOLDER_ERROR,
		SPL_LOG_CREATE_THREAD_ERROR,
		SPL_LOG_FMT_NULL_ERROR,
		SPL_LOG_MEM_GEN_FILE_ERROR,
		SPL_LOG_MEM_MALLOC_ERROR,
		SPL_LOG_OPEN_FILE_ERROR,
		SPL_LOG_OPEN1_FILE_ERROR,
		SPL_LOG_CLOSE_FILE_ERROR,
		SPL_LOG_SEM_NULL_ERROR,
		SPL_LOG_ROT_SIZE_ERROR,
		SPL_LOG_TOPIC_EMPTY,
		SPL_LOG_TOPIC_NULL,
		SPL_LOG_MEM_FILE_MALLOC_ERROR,
		SPL_LOG_CHECK_FOLDER_ERROR,
		SPL_LOG_CHECK_FOLDER_YEAR_ERROR,
		SPL_LOG_CHECK_FILE_YEAR_ERROR,
		SPL_LOG_CHECK_FOLDER_NULL_ERROR,
		SPL_LOG_MUTEX_NULL_ERROR,
		SPL_LOG_ST_NAME_NULL_ERROR,
		SPL_LOG_TIME_NULL_ERROR,
		SPL_LOG_TIME_NANO_NULL_ERROR, 
		SPL_LOG_STAT_FOLDER_ERROR,
		SPL_LOG_PRINTF_ERROR,
		SPL_LOG_TOPIC_ZERO,
		SPL_LOG_TOPIC_MEMORY,
		SPL_LOG_TOPIC_FOPEN,
		SPL_LOG_TOPIC_FLUSH,
		SPL_LOG_TOPIC_BUFF_MEM,


		SPL_END_ERROR,
	} SPL_LOG_ERR_CODE;



#define spl_console_log(___fmttt___, ...)		{char buf[1024];spl_fmmt_now(buf, 1024);\
fprintf(stdout, "[%s] [%s:%d] [thid: %llu] "___fmttt___"\n" , buf, __FUNCTION__, __LINE__, spl_get_threadid(), ##__VA_ARGS__);}





#define __spl_log_buf__(___fmttt___, ...)	{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf(&range, &__ppl); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [tid: %llu] [%s:%d] "___fmttt___"\n\n", \
tnow, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}

#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)	{if(spl_get_log_levwel() <= (__lv__) )\
{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; const char *lv_text = spl_get_text(__lv__);spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf(&range, &__ppl); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [%s] [tid:\t%llu]\t[%s:%d]\t"___fmttt___"\n\n", \
tnow, lv_text, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}\
}



#define __spl_log_buf_topic__(__tpic, ___fmttt___, ...)	{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf_topic(&range, &__ppl, (__tpic)); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [tid:\t%llu]\t[%s:%d] "___fmttt___"\n\n", \
tnow, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}

#define __spl_log_buf_topic_level__(__lv__, __tpic, ___fmttt___, ...)	{ if(spl_get_log_levwel() <= (__lv__) ) \
{int *__ppl = 0; char tnow[40]; int range=0; char* __p = 0; void *__mtx__ =  spl_get_mtx(); LLU thrid = spl_get_threadid();\
int len = 0; const char *lv_text = spl_get_text(__lv__);spl_fmt_now(tnow, 40);\
spl_mutex_lock(__mtx__);\
__p = spl_get_buf_topic(&range, &__ppl, (__tpic)); if (__p && __ppl) { len = snprintf((__p + (*__ppl)), range, \
"[%s] [%s] [tid:\t%llu]\t[%s:%d]\t"___fmttt___"\n\n", \
tnow, lv_text, thrid, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
if(len > 0) (*__ppl) += (len -1);}\
spl_mutex_unlock(__mtx__); spl_rel_sem(spl_get_sem_rwfile());}\
}


/* 
* Export name:	spllog
* Sample:		spllog(SPL_LOG_INFO, "Hello spllog: %llu", time(0));
*/
#define spllog					__spl_log_buf_level__

/*
* Export name:	spllogtopic
* Sample:		spllogtopic(SPL_LOG_INFO, 0, "Hello spllog: %llu", time(0));
*/
#define spllogtopic				__spl_log_buf_topic_level__

/* Please demo with spl_init_log */
DLL_API_SIMPLE_LOG int
	spl_init_log(char* path);

/* Please demo with spl_finish_log */
DLL_API_SIMPLE_LOG int
	spl_finish_log();

/*--------------------------------------------------------------------*/
DLL_API_SIMPLE_LOG int									
	spl_set_log_levwel(int val);
DLL_API_SIMPLE_LOG int									
	spl_get_log_levwel();
DLL_API_SIMPLE_LOG int									
	spl_fmt_now(char* fmtt, int len);
DLL_API_SIMPLE_LOG int									
	spl_fmmt_now(char* fmtt, int len);
DLL_API_SIMPLE_LOG int									
	spl_mutex_lock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_mutex_unlock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_set_off(int );
DLL_API_SIMPLE_LOG int									
	spl_get_off();
DLL_API_SIMPLE_LOG void*								
	spl_get_mtx();
DLL_API_SIMPLE_LOG void*								
	spl_get_sem_rwfile();
DLL_API_SIMPLE_LOG LLU									
	spl_get_threadid();
DLL_API_SIMPLE_LOG int									
	spl_rel_sem(void* sem);
DLL_API_SIMPLE_LOG const char*							
	spl_get_text(int lev);
DLL_API_SIMPLE_LOG char *								
	spl_get_buf(int* n, int** ppl);
DLL_API_SIMPLE_LOG char*
	spl_get_buf_topic(int* n, int** ppl, int );
DLL_API_SIMPLE_LOG 
	void* spl_mutex_create();
DLL_API_SIMPLE_LOG
	void spl_sleep(unsigned  int);
DLL_API_SIMPLE_LOG
	int spl_standardize_path(char* fname);
DLL_API_SIMPLE_LOG
	LLU spl_milli_now();
/*--------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif