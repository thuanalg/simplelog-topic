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
*		<2024-Sep-14>
*		<2024-Dec-18>
*		<2024-Dec-20>
*		<2024-Dec-22>
*		<2024-Dec-23>
* Decription:
*		The (only) main header file to export 3 APIs: [spl_init_log, spllog, spllogtopic, spl_finish_log].
*/
/*===============================================================================================================*/
#ifndef ___SIMPLE_LOG__
#define ___SIMPLE_LOG__
#include <stdio.h>
#include <string.h>
//#include "simplelog_config.h"
/*strrchr*/

#ifndef SPL_USING_SPIN_LOCK
	#define SPL_USING_SPIN_LOCK
#endif // !SPL_USING_SPIN_LOCK

//#define __UNIX_LINUX_CPP11_AND_NEWERS__

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
#else
#include <string>
#endif
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

#define					SPL_RL_BUF						256

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
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	typedef
		struct __GENERIC_DATA__ {
		int
			total;
		/*Total size*/
		int
			range;
		/*Total size*/
		int
			pc;
		/*Point to the current*/
		int
			pl;
		/*Point to the last*/
		char
			data[0];
		/*Generic data */
	} generic_dta_st;

#define spl_uchar			unsigned char
#define spl_uint			unsigned int

	typedef struct __spl_local_time_st__ {
		spl_uint	year;
		spl_uchar	month;
		spl_uchar	day;
		spl_uchar	hour;
		spl_uchar	minute;
		spl_uchar	sec;
		//spl_uint	ms;						/*Millisecond*/
		spl_uint	nn;						/*Nanosecond*/
	} spl_local_time_st;

#define SPL_TOPIC_SIZE		32

	typedef
		struct __SIMPLE_LOG_TOPIC_ST__ {
		int
			index;
		/*Index of a topic*/
		char
			topic[SPL_TOPIC_SIZE];
		/*Name of topic*/
		generic_dta_st*
			buf;
		/*Buff for writing*/
		int
			fizize;
		/*Size of file.*/
		void*
			fp;
		/*File stream.*/
	} SIMPLE_LOG_TOPIC_ST;

	typedef
		struct __SIMPLE_LOG_ST__ {
		int
			llevel; /*Level of log.*/
		int
			file_limit_size; /*Limitation of each log file. No nead SYNC.*/
		int
			buff_size; /*Buffer size for each buffer. No nead SYNC.*/
		int
			index; /*Index of default log, not in a topic. No nead SYNC.*/
		char
			folder[1024]; /*Path of genera folder. No nead SYNC.*/
		char
			off; /*Must be sync*/
		void*
			mtx_rw; /*mtx: Need to close handle*/
//	void*
//		mtx_off; /*mtx_off: Need to close handle*/
		void*
			sem_rwfile; /*sem_rwfile: Need to close handle*/
		void*
			sem_off; /*sem_off: Need to close handle*/
		spl_local_time_st
			lc_time_now; /*lc_time: Need to sync, free*/
		FILE*
			fp; /*fp: Need to close*/
		generic_dta_st*
			buf; /*buf: Must be sync, free*/
		char*
			topics; /*topics: topics string. Must be freed */
		int
			n_topic; /*Number of topics, SIMPLE_LOG_TOPIC_ST.*/
		SIMPLE_LOG_TOPIC_ST*
			arr_topic; /*List od topics: SIMPLE_LOG_TOPIC_ST*.*/
		int
			renew; /*In a thread of logger, NO NEED SYNC.*/
		char
			path_template[1024]; /*In a thread of logger, NO NEED SYNC.*/
	} SIMPLE_LOG_ST;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define __FILLE__(__p__)	do { __p__ = strrchr(__FILE__, '/'); if(__p__) {++__p__;break;} \
__p__ = strrchr(__FILE__, '\\'); if(__p__) {++__p__;break;}\
__p__ = __FILE__;} while(0);

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__

#define spl_console_log(___fmttt___, ...)		{char buf[1024]; const char *pfn = 0; __FILLE__(pfn);spl_fmmt_now(buf, 1024);\
fprintf(stdout, "[%s] [%s:%s:%d] [thid: %llu] "___fmttt___"\n" , buf, pfn, __FUNCTION__, __LINE__, spl_get_threadid(), ##__VA_ARGS__);}




#define SPLCHECKOFF(__t__)					__t__->off
#define SPLCHECKBUF(__t__)					__t__->buf
#define SLPCHECKRANGE(__t__)				((SPLCHECKBUF(__t__)->range > (SPLCHECKBUF(__t__)->pl)) ? (SPLCHECKBUF(__t__)->range - SPLCHECKBUF(__t__)->pl) : 0);

#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)	{if(spl_get_log_levwel() <= (__lv__) )\
{SIMPLE_LOG_ST *t = 0;int len = 0;;const char *pfn = 0;char __isOof = 0; char *pprefmt = 0;\
 char tnow[SPL_RL_BUF];void *__mtx__ =  spl_get_mtx();;\
 __FILLE__(pfn);pprefmt = spl_fmt_now_ext(tnow, SPL_RL_BUF, __lv__, pfn, __FUNCTION__, __LINE__);t = spl_control_obj();\
do{\
spl_mutex_lock(__mtx__);\
	do {\
		if(SPLCHECKOFF(t)) { __isOof = 1; break;}\
		if(SPLCHECKBUF(t)->range > SPLCHECKBUF(t)->pl) {\
			len = snprintf((SPLCHECKBUF(t)->data + SPLCHECKBUF(t)->pl), SPLCHECKBUF(t)->range - SPLCHECKBUF(t)->pl, \
				"%s"___fmttt___"\n\n", pprefmt, ##__VA_ARGS__);\
			if(len > 0) SPLCHECKBUF(t)->pl += (len-1);\
		}\
	}while(0);\
spl_mutex_unlock(__mtx__);\
if(len > 0) break;if(__isOof)break;spl_milli_sleep(10);continue;\
}\
while(1);\
spl_rel_sem(spl_get_sem_rwfile()); if(pprefmt != tnow) { free(pprefmt);}}\
}





#define SLPCHECKINDEX(__t__)	
#define STSPLOGBUFTOPIC(__t__,__i__)				(&(__t__->arr_topic[__i__]))->buf

#define __spl_log_buf_topic_level__(__lv__, __tpic, ___fmttt___, ...)	{ if(spl_get_log_levwel() <= (__lv__) ) \
{int len = 0;;const char *pfn = 0;SIMPLE_LOG_ST *t = 0;char __isbrf = 0; char *pprefmt = 0;; char tnow[SPL_RL_BUF];; void *__mtx__ =  spl_get_mtx();;\
; __FILLE__(pfn);t = spl_control_obj();\
;pprefmt = spl_fmt_now_ext(tnow, SPL_RL_BUF, __lv__, pfn, __FUNCTION__, __LINE__);;\
do{\
if (__tpic < 0 || ((__tpic + 1) > t->n_topic)){ __isbrf = 1; break;}/*they are constant.*/\
spl_mutex_lock(__mtx__);\
	do {\
		if(SPLCHECKOFF(t)) { __isbrf = 1; break;};\
		;\
		if(t->arr_topic){\
			if(STSPLOGBUFTOPIC(t,__tpic)->range > STSPLOGBUFTOPIC(t,__tpic)->pl) {\
				len = snprintf(STSPLOGBUFTOPIC(t, __tpic)->data + STSPLOGBUFTOPIC(t, __tpic)->pl, \
					STSPLOGBUFTOPIC(t,__tpic)->range - STSPLOGBUFTOPIC(t,__tpic)->pl, \
					"%s"___fmttt___"\n\n", pprefmt, ##__VA_ARGS__);\
				if(len > 0) STSPLOGBUFTOPIC(t,__tpic)->pl += (len-1);\
			}\
		}else{__isbrf = 1; break;;}\
	}\
	while(0);\
spl_mutex_unlock(__mtx__);\
if(len > 0) break;if(__isbrf)break;spl_milli_sleep(10);continue;\
}\
while(1);\
spl_rel_sem(spl_get_sem_rwfile());if(pprefmt != tnow) { free(pprefmt);}}\
}


/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#else
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#define spl_console_log(___fmttt___, ...)		{std::string __c11fmt__="[%s] [%s:%s:%d] [thid: %llu] ";__c11fmt__+=___fmttt___;__c11fmt__+="\n";;char buf[1024]; const char *pfn = 0; __FILLE__(pfn);spl_fmmt_now(buf, 1024);\
fprintf(stdout, __c11fmt__.c_str(), buf, pfn, __FUNCTION__, __LINE__, spl_get_threadid(), ##__VA_ARGS__);}



#define SPLCHECKOFF(__t__)					__t__->off
#define SPLCHECKBUF(__t__)					__t__->buf
#define SLPCHECKRANGE(__t__)				((SPLCHECKBUF(__t__)->range > (SPLCHECKBUF(__t__)->pl)) ? (SPLCHECKBUF(__t__)->range - SPLCHECKBUF(__t__)->pl) : 0);

#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)	{if(spl_get_log_levwel() <= (__lv__) )\
{std::string __c11fmt__="%s";__c11fmt__+=___fmttt___;__c11fmt__+="\n\n"; const char *__c11fmt_c_str__ = __c11fmt__.c_str();\
;SIMPLE_LOG_ST *t = 0;int len = 0;;const char *pfn = 0;char __isOof = 0; char *pprefmt = 0;\
 char tnow[SPL_RL_BUF];void *__mtx__ =  spl_get_mtx();;\
 __FILLE__(pfn);pprefmt = spl_fmt_now_ext(tnow, SPL_RL_BUF, __lv__, pfn, __FUNCTION__, __LINE__);t = spl_control_obj();\
do{\
spl_mutex_lock(__mtx__);\
	do {\
		if(SPLCHECKOFF(t)) { __isOof = 1; break;}\
		if(SPLCHECKBUF(t)->range > SPLCHECKBUF(t)->pl) {\
			len = snprintf((SPLCHECKBUF(t)->data + SPLCHECKBUF(t)->pl), SPLCHECKBUF(t)->range - SPLCHECKBUF(t)->pl, \
				__c11fmt_c_str__, pprefmt, ##__VA_ARGS__);\
			if(len > 0) SPLCHECKBUF(t)->pl += (len-1);\
		}\
	}while(0);\
spl_mutex_unlock(__mtx__);\
if(len > 0) break;if(__isOof)break;spl_milli_sleep(10);continue;\
}\
while(1);\
spl_rel_sem(spl_get_sem_rwfile()); if(pprefmt != tnow) { free(pprefmt);}}\
}



#define SLPCHECKINDEX(__t__)	
#define STSPLOGBUFTOPIC(__t__,__i__)				(&(__t__->arr_topic[__i__]))->buf

#define __spl_log_buf_topic_level__(__lv__, __tpic, ___fmttt___, ...)	{ if(spl_get_log_levwel() <= (__lv__) ) \
{std::string __c11fmt__="%s";__c11fmt__+=___fmttt___;__c11fmt__+="\n\n"; const char *__c11fmt_c_str__ = __c11fmt__.c_str();\
int len = 0;;const char *pfn = 0;SIMPLE_LOG_ST *t = 0;char __isbrf = 0; char *pprefmt = 0;; char tnow[SPL_RL_BUF];; void *__mtx__ =  spl_get_mtx();;\
; __FILLE__(pfn);t = spl_control_obj();\
;pprefmt = spl_fmt_now_ext(tnow, SPL_RL_BUF, __lv__, pfn, __FUNCTION__, __LINE__);;\
do{\
if (__tpic < 0 || ((__tpic + 1) > t->n_topic)){ __isbrf = 1; break;}/*they are constant.*/\
spl_mutex_lock(__mtx__);\
	do {\
		if(SPLCHECKOFF(t)) { __isbrf = 1; break;};\
		;\
		if(t->arr_topic){\
			if(STSPLOGBUFTOPIC(t,__tpic)->range > STSPLOGBUFTOPIC(t,__tpic)->pl) {\
				len = snprintf(STSPLOGBUFTOPIC(t, __tpic)->data + STSPLOGBUFTOPIC(t, __tpic)->pl, \
					STSPLOGBUFTOPIC(t,__tpic)->range - STSPLOGBUFTOPIC(t,__tpic)->pl, \
					__c11fmt_c_str__, pprefmt, ##__VA_ARGS__);\
				if(len > 0) STSPLOGBUFTOPIC(t,__tpic)->pl += (len-1);\
			}\
		}else{__isbrf = 1; break;;}\
	}\
	while(0);\
spl_mutex_unlock(__mtx__);\
if(len > 0) break;if(__isbrf)break;spl_milli_sleep(10);continue;\
}\
while(1);\
spl_rel_sem(spl_get_sem_rwfile());if(pprefmt != tnow) { free(pprefmt);}}\
}


#endif

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/* Please demo with spl_init_log */
DLL_API_SIMPLE_LOG int
	spl_init_log(char* path);

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


/* Please demo with spl_finish_log */
DLL_API_SIMPLE_LOG int
	spl_finish_log();

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
DLL_API_SIMPLE_LOG int									
	spl_set_log_levwel(int val);

DLL_API_SIMPLE_LOG int									
	spl_get_log_levwel();

DLL_API_SIMPLE_LOG int									
	spl_fmt_now(char* fmtt, int len);

DLL_API_SIMPLE_LOG char *
	spl_fmt_now_ext(char* fmtt, int len, int lv, 
		const char *filename, const char* funcname, int  line);

DLL_API_SIMPLE_LOG int									
	spl_fmmt_now(char* fmtt, int len);
DLL_API_SIMPLE_LOG int									
	spl_mutex_lock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_mutex_unlock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_set_off(int );
//DLL_API_SIMPLE_LOG int									
//	spl_get_off();
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
DLL_API_SIMPLE_LOG char *								
	spl_get_buf_ext(int* n, int** ppl, char*);

DLL_API_SIMPLE_LOG char*
	spl_get_buf_topic(int* n, int** ppl, int );

DLL_API_SIMPLE_LOG char*
	spl_get_buf_topic_ext(int* n, int** ppl, int, char *isoff);


DLL_API_SIMPLE_LOG 
	void* spl_mutex_create();
DLL_API_SIMPLE_LOG
	void spl_sleep(unsigned  int);

DLL_API_SIMPLE_LOG
	void spl_milli_sleep(unsigned  int);

DLL_API_SIMPLE_LOG
	int spl_standardize_path(char* fname);

DLL_API_SIMPLE_LOG
	LLU spl_milli_now();

DLL_API_SIMPLE_LOG
	SIMPLE_LOG_ST *spl_control_obj();
//DLL_API_SIMPLE_LOG
//	void splLockSpinlock(volatile long* p);
//DLL_API_SIMPLE_LOG
//	void splUnlockSpinlock(volatile long* p);
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#ifdef __cplusplus
}
#endif
#endif