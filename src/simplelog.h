/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
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
*		<2024-Dec-30>
*		<2024-Jan-06>
*		<2024-Jan-08>
* Decription:
*		The (only) main header file to export 3 APIs: [spl_init_log, spllog, spllogtopic, spl_finish_log].
*/
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
#ifndef ___SIMPLE_LOG__
#define ___SIMPLE_LOG__
#include <stdio.h>
#include <string.h>
/*strrchr*/

#ifndef SPL_USING_SPIN_LOCK
	//#define SPL_USING_SPIN_LOCK
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

#define					SPL_LOG_BASE						0
#define					SPL_LOG_DEBUG						1
#define					SPL_LOG_INFO						2
#define					SPL_LOG_WARNING						3
#define					SPL_LOG_ERROR						4
#define					SPL_LOG_FATAL						5
#define					SPL_LOG_PEAK						6

//#define					SPL_RL_BUF						50
#define					SPL_RL_BUF							256
#define					SPL_PATH_FOLDER						1024
#define					SPL_IDD_NAME						64

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
		SPL_LOG_BUFF_MALLOC_ERROR,
		SPL_LOG_FOLDER_ERROR,
		SPL_LOG_CREATE_THREAD_ERROR,
		SPL_LOG_FMT_NULL_ERROR,
		SPL_LOG_MEM_GEN_FILE_ERROR,
		SPL_LOG_MEM_MALLOC_ERROR,
		SPL_LOG_OPEN_FILE_ERROR,
		SPL_LOG_OPEN1_FILE_ERROR,
		SPL_LOG_CLOSE_FILE_ERROR,
		SPL_LOG_SEM_NULL_ERROR,
		SPL_LOG_SEM_WIN32_CREATED_ERROR,
		SPL_LOG_MTX_WIN32_CREATED_ERROR,
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
		SPL_LOG_ALOCK_NUM,
		SPL_LOG_ALOCK_NULL,
		SPL_LOG_SHM_CREATE_NULL,
		SPL_LOG_SHM_WIN_UNMAP,
		SPL_LOG_SHM_UNIX_OPEN,
		SPL_LOG_SHM_UNIX_TRUNC, 
		SPL_LOG_SHM_UNIX_MAP_FAILED, 
		SPL_LOG_WIN_SHM_CLOSE,
		SPL_LOG_SHM_UNIX_UNMAP,
		SPL_LOG_VAR_NULL,
		SPL_LOG_ARR_MTX_NULL,
		SPL_LOG_ARR_BUFF_NULL,
		SPL_LOG_MTX_ATT_SHARED_MODE,
		SPL_LOG_MTX_ATT_SHARED_MODE_SET,
		SPL_LOG_MTX_INIT_ERR,
		SPL_LOG_SHM_UNIX_INIT_MUTEX,
		SPL_LOG_SPINLOCK_INIT_SHARED,
		SPL_LOG_SPINLOCK_INIT_PRIVATE,
		SPL_LOG_SEM_INIT_UNIX,
		
		
		
		SPL_END_ERROR,
	} SPL_LOG_ERR_CODE;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
	typedef
		struct __GENERIC_DATA__ {
		int
			total;						/*Total size*/
		int
			range;						/*Total size*/
		int
			pc;							/*Point to the current*/
		int
			pl;							/*Point to the last*/
		char
			data[0];					/*Generic data */
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
		spl_uint	nn;					/*Nanosecond*/
	} spl_local_time_st;

#define				SPL_TOPIC_SIZE					32
#define				SPL_MEMO_PADDING				2048
#define				SPL_SHARED_KEY_LEN				64
#define				SPL_SHARED_NAME_LEN				128

	typedef
		struct __SIMPLE_LOG_TOPIC_ST__ {
		int
			index;						/*Index of a topic*/
		char
			topic[SPL_TOPIC_SIZE];		/*Name of topic*/
		generic_dta_st*
			buf;						/*Buff for writing*/
		int
			fizize;						/*Size of file.*/
		void*
			fp;							/*File stream.*/
	} SIMPLE_LOG_TOPIC_ST;

	typedef
		struct __SIMPLE_LOG_ST__ {
		int
			llevel;						/*Level of log.*/
		int
			file_limit_size;			/*Limitation of each log file. No nead SYNC.*/
		int
			buff_size;					/*Buffer size for each buffer. No nead SYNC.*/
		int
			index;						/*Index of default log, not in a topic. No nead SYNC.*/
		char
			folder[1024];				/*Path of genera folder. No nead SYNC.*/
		char
			off;						/*Must be sync*/
		void*
			mtx_rw;						/*mtx: Need to close handle*/
		void*
			sem_rwfile;					/*sem_rwfile: Need to close handle*/
		void*
			sem_off;					/*sem_off: Need to close handle*/
		spl_local_time_st
			lc_time_now;				/*Current time.*/
		FILE*
			fp;							/*fp: Need to close*/
		generic_dta_st*
			buf;						/*buf: Must be synchoronized. Must be freed.*/
		char*
			topics;						/*topics: topics string. Must be freed */
		int
			n_topic;					/*Number of topics, SIMPLE_LOG_TOPIC_ST.*/
		SIMPLE_LOG_TOPIC_ST*
			arr_topic;					/*List od topics: SIMPLE_LOG_TOPIC_ST. Must be freed*/
		int
			renew;						/*In a thread of logger, NO NEED SYNC.*/
		char
			path_template[1024];		/*In a thread of logger, NO NEED SYNC.*/
		int
			ncpu;						/*Number of CPU.*/
		int
			trigger_thread;				/*Use trigger thread or not.*/
		void	
			**arr_mtx;					/*List of lock: Spinlock or Mutex. Must be freed.*/
		/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef UNIX_LINUX
		void*
#else
		int
#endif
			hd;							/* Handle of shared memory.*/
		char
			shared_key[SPL_SHARED_KEY_LEN]; /* Name of shared key.*/
		char 
			id_name[SPL_IDD_NAME];		/*To avoid duplicating of file name.*/
		char
			isProcessMode;				/*For cross processes mdoe.*/
	} SIMPLE_LOG_ST;
/*
typedef struct __FMT_FOR_OUTPUT__ {
	int outlen ;
	const char* finame ;
	const char* fcname;
	int line;
	char tnow[SPL_RL_BUF];
	char *prefmt;
	int lv;
	unsigned short r ;
} FMT_FOR_OUTPUT;
*/
	typedef struct __SPL_INPUT_ARG__ {
		char folder[SPL_PATH_FOLDER];
		char id_name[SPL_IDD_NAME];
	} SPL_INPUT_ARG;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define __FILLE__(__p__)	do { __p__ = strrchr(__FILE__, '/'); if(__p__) {++__p__;break;} \
__p__ = strrchr(__FILE__, '\\'); if(__p__) {++__p__;break;}\
__p__ = __FILE__;} while(0);

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
	#define spl_console_log(___fmttt___, ...)		{char buf[1024]; const char *pfn = 0; __FILLE__(pfn);spl_fmmt_now(buf, 1024);\
		fprintf(stdout, "[%s] [%s:%s:%d] [thid: %llu] "___fmttt___"\n" , (char *)buf, (char *)pfn, (char*)__FUNCTION__, (int)__LINE__, spl_get_threadid(), ##__VA_ARGS__);}
#else
	#define spl_console_log(___fmttt___, ...)		{std::string __c11fmt__="[%s] [%s:%s:%d] [thid: %llu] ";__c11fmt__+=___fmttt___;__c11fmt__+="\n";;char buf[1024]; const char *pfn = 0; __FILLE__(pfn);spl_fmmt_now(buf, 1024);\
		fprintf(stdout, __c11fmt__.c_str(), (char *)buf, (char *)pfn, (char *)__FUNCTION__, (int)__LINE__, spl_get_threadid(), ##__VA_ARGS__);}
#endif

#define spl_malloc(__nn__, __obj__, __type__) { (__obj__) = (__type__*) malloc(__nn__); if(__obj__) \
	{ /*spl_console_log("Malloc: 0x%p\n", (__obj__));*/; memset((void*)(__obj__), 0, (__nn__));} \
	else {spl_console_log("Malloc: error.\n");}} 

#define spl_free(__obj__) \
	{ /*spl_console_log("Free: 0x:%p.\n", (__obj__));*/; free(__obj__); ; (__obj__) = 0;} 

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#define SPLKEYBUF(__t__, __i__)				((generic_dta_st*)( (char*)__t__->buf + (t->buff_size * __i__)))
#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)	\
{SIMPLE_LOG_ST *t = spl_control_obj();\
	if(t->llevel <= (__lv__) && ___fmttt___[0])\
	{\
		;\
		;int outlen = 0;;const char *pfn = 0;/*char __isOof = 0;*/ ;\
		;unsigned short r = 0;;char tnow[SPL_RL_BUF]; char *pprefmt = 0; \
		;;\
		;__FILLE__(pfn);pprefmt = spl_fmt_now_ext(tnow, SPL_RL_BUF, __lv__, pfn, __FUNCTION__, __LINE__, &r, &outlen);;\
		{\
			do{\
				;int len = 0;; \
				\
				;;;\
				spl_mutex_lock(t->arr_mtx[r]);\
					;\
						if(SPLKEYBUF(t, r)->range > SPLKEYBUF(t, r)->pl) {\
							;memcpy(SPLKEYBUF(t, r)->data + SPLKEYBUF(t, r)->pl, pprefmt, outlen);SPLKEYBUF(t, r)->pl += outlen;\
							;len = snprintf( SPLKEYBUF(t, r)->data + SPLKEYBUF(t, r)->pl, (SPLKEYBUF(t, r)->range + SPL_MEMO_PADDING - SPLKEYBUF(t, r)->pl), \
								___fmttt___, ##__VA_ARGS__); if(len > 0) SPLKEYBUF(t, r)->pl += (len); ;\
							\
						}\
					\
				spl_mutex_unlock(t->arr_mtx[r]); \
				\
				if(len > 0) break;\
				;/*spl_console_log("---------------------------OVERRRRRRRRRRRRRRRRRRR======================, r: %d", (int)r);*/\
				;r++; r%=t->ncpu;\
				;;continue;\
			}\
			while(1);\
			if(!t->trigger_thread)spl_rel_sem(t->sem_rwfile); if(pprefmt != tnow) { spl_free(pprefmt);}\
		}\
	}\
}



#define STSPLOGBUFTOPIC(__t__,__i__)							(&(__t__->arr_topic[__i__]))->buf
#define STSPLOGBUFTOPIC_RANGE(__t__,__i__, __r__)				((generic_dta_st*)((char *)STSPLOGBUFTOPIC(__t__,__i__) + t->buff_size * __r__))

#define __spl_log_buf_topic_level__(__lv__, __tpic__, ___fmttt___, ...)	\
{SIMPLE_LOG_ST *t = spl_control_obj();\
	if(t->llevel <= (__lv__) && ___fmttt___[0] && t->arr_topic) \
	{\
		;short tpp = 0;int len = 0;unsigned short r = 0;;const char *pfn = 0;;\
		;int outlen = 0;;char *pprefmt = 0;; char tnow[SPL_RL_BUF];;;\
		; __FILLE__(pfn);;\
		;pprefmt = spl_fmt_now_ext(tnow, SPL_RL_BUF, __lv__, pfn, __FUNCTION__, __LINE__, &r, &outlen);;\
		do\
		{\
			;;tpp = __tpic__%t->n_topic;;\
			spl_mutex_lock(t->arr_mtx[r]);\
				/*do \
				{*/\
					/*if(t->arr_topic){*/\
						if(STSPLOGBUFTOPIC_RANGE(t,tpp, r)->range > STSPLOGBUFTOPIC_RANGE(t,tpp, r)->pl) {\
							;memcpy(STSPLOGBUFTOPIC_RANGE(t,tpp, r)->data + STSPLOGBUFTOPIC_RANGE(t,tpp, r)->pl, pprefmt, outlen);\
							;STSPLOGBUFTOPIC_RANGE(t, tpp, r)->pl += outlen;;\
							;len = snprintf(STSPLOGBUFTOPIC_RANGE(t,tpp, r)->data + STSPLOGBUFTOPIC_RANGE(t,tpp, r)->pl, \
								STSPLOGBUFTOPIC_RANGE(t, tpp, r)->range + SPL_MEMO_PADDING - STSPLOGBUFTOPIC_RANGE(t, tpp, r)->pl, \
								___fmttt___, ##__VA_ARGS__);\
							;/*spl_console_log("--------------lllllllennnnnnnnnnnnnnnnn---r: %d, len: %d", (int)r, len);*/;\
							if(len > 0) STSPLOGBUFTOPIC_RANGE(t, tpp, r)->pl += len;\
						}\
					/*}*/\
				/*}\
				while(0);*/\
			spl_mutex_unlock(t->arr_mtx[r]);\
			if(len > 0) break;\
			;\
			;r++; r%=t->ncpu;;continue;\
		}\
		while(1);\
		if(!t->trigger_thread)spl_rel_sem(t->sem_rwfile);;if(pprefmt != tnow) { spl_free(pprefmt);}\
	}\
}


/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

/* Please demo with spl_init_log */
DLL_API_SIMPLE_LOG int
	spl_init_log(char* path);

/* Please demo with spl_init_log */
DLL_API_SIMPLE_LOG int
	spl_init_log_ext(SPL_INPUT_ARG* input);

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
DLL_API_SIMPLE_LOG char *
	spl_fmt_now_ext(char* fmtt, int len, int lv, 
		const char *filename, const char* funcname, int  line, unsigned short* r, int *);
DLL_API_SIMPLE_LOG int									
	spl_fmmt_now(char* fmtt, int len);
DLL_API_SIMPLE_LOG int									
	spl_mutex_lock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_mutex_unlock(void* mtx);
DLL_API_SIMPLE_LOG int									
	spl_set_off(int );
DLL_API_SIMPLE_LOG void*								
	spl_get_mtx();
DLL_API_SIMPLE_LOG void*								
	spl_get_sem_rwfile();
DLL_API_SIMPLE_LOG LLU									
	spl_get_threadid();
DLL_API_SIMPLE_LOG int									
	spl_rel_sem(void* sem);
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
//	char *spl_prefmt_now(FMT_FOR_OUTPUT* p);
//
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#ifdef __cplusplus
}
#endif
#endif