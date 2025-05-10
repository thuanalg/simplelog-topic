/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
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
 *		<2025-Jan-08>
 *		<2025-Jan-10>
 *		<2025-Feb-01>
 *		<2025-Feb-16>
 *		<2025-Apr-11>
 *		<2025-Apr-22>
 * Decription:
 *		The (only) main header file to export 4 APIs: [spl_init_log_ext, spllog, spllogtopic, spl_finish_log].
 */
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef ___SIMPLE_LOG__
#define ___SIMPLE_LOG__                 
#include <stdio.h>
#include <string.h>
/*strrchr*/

#define SPL_MIN_AB(a, b) ((a) < (b)) ? (a) : (b)
#define SPL_MAX_AB(a, b) ((a) > (b)) ? (a) : (b)

#if 0
#ifndef UNIX_LINUX
#define UNIX_LINUX                      
#endif
#endif

#if 0
#ifndef __MACH__
#define __MACH__                        
#endif
#endif

#if 0
#ifndef SPL_USING_SPIN_LOCK
#define SPL_USING_SPIN_LOCK             
#endif // !SPL_USING_SPIN_LOCK
#endif

#if 0
#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
#define __UNIX_LINUX_CPP11_AND_NEWERS__ 
#endif
#endif

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
#else
#include <string>
#endif
#ifdef __cplusplus
extern "C" {
#endif

#define LLU                             unsigned long long

#define SPL_LOG_BASE                    0
#define SPL_LOG_DEBUG                   1
#define SPL_LOG_INFO                    2
#define SPL_LOG_WARNING                 3
#define SPL_LOG_ERROR                   4
#define SPL_LOG_FATAL                   5
#define SPL_LOG_PEAK                    6

#if 0
#ifndef SPL_RL_BUF
#define SPL_RL_BUF                      50
#endif
#endif

#define SPL_RL_BUF                      256
#define SPL_PATH_FOLDER                 (256 + 16)
#define SPL_IDD_NAME                    32
#define SPL_TOPIC_SIZE                  32
#define SPL_MEMO_PADDING                2048
#define SPL_SHARED_KEY_LEN              32
#define SPL_SHARED_NAME_LEN             64
#define SPL_FNAME_LEN                   (SPL_IDD_NAME + 32)
#define SPL_TEMPLATE_LEN                (SPL_PATH_FOLDER + SPL_FNAME_LEN + 32)
#define SPL_FULLPATH_LEN                (SPL_TEMPLATE_LEN + 32 + 16)
#define SPL_MILLION                     1000000

#ifndef UNIX_LINUX
#ifndef __SIMPLE_STATIC_LOG__
#ifdef EXPORT_DLL_API_SIMPLE_LOG
#define DLL_API_SIMPLE_LOG              __declspec(dllexport)
#else
#define DLL_API_SIMPLE_LOG              __declspec(dllimport)
#endif
#else
#define DLL_API_SIMPLE_LOG              
#endif
#else
#define DLL_API_SIMPLE_LOG              
#endif /*! UNIX_LINUX */

typedef enum __SPL_LOG_ERR_CODE__ {
	SPL_NO_ERROR,
	SPL_INIT_PATH_FOLDER_EMPTY_ERROR,
	SPL_LOG_LEVEL_ERROR,
	SPL_ERROR_CREATE_MUTEX,
	SPL_ERROR_CREATE_SEM,
	SPL_LOG_BUFF_SIZE_ERROR,
	SPL_LOG_BUFF_MALLOC_ERROR,
	SPL_LOG_MAX_SZ_MSG_ERROR,
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
	SPL_LOG_THREAD_W32_CREATE,
	SPL_LOG_THREAD_PX_CREATE,
	SPL_LOG_MACH_GETTIME_ERROR,
	SPL_LOG_MACH_CLOCK_SERVICE_ERROR,
	SPL_LOG_OSX_SEM_CLOSE,
	SPL_LOG_OSX_SEM_UNLINK,
	SPL_LOG_SEM_OSX_CREATED_ERROR,
	SPL_LOG_SEM_INIT_OSX,
	SPL_LOG_SEM_OSX_UNLINK_ERROR,
	SPL_LOG_OPEN_CFG,
	SPL_LOG_WIN32_WAIT_OBJECT,
	SPL_LOG_WIN32_MUTEX_RELEASE,
	SPL_LOG_WIN32_MAP_FILE,

	SPL_END_ERROR,
} SPL_LOG_ERR_CODE;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
typedef struct __SPL_CALLBACL_DATA__ {
	int total;
	int eventid;
	/*int range;*/
	int pc;
	int pl;
	char data[0];
} SPL_CALLBACL_DATA;
typedef int (*SPL_CALLBACL_FUNCTION)(void *);
typedef struct __SPL_GENERIC_DATA__ {
	int total; /*Total size*/
	int range; /*Total size*/
	int pc; /*Point to the current*/
	int pl; /*Point to the last*/
	char data[0]; /*Generic data */
} spl_gen_data_st;

#define spl_uchar                       unsigned char
#define spl_uint                        unsigned int

typedef struct __spl_local_time_st__ {
	spl_uint year;
	spl_uchar month;
	spl_uchar day;
	spl_uchar hour;
	spl_uchar minute;
	spl_uchar sec;
	spl_uint nn; /*Nanosecond*/
} spl_local_time_st;

typedef struct __SIMPLE_LOG_TOPIC_ST__ {
	int index; /*Index of a topic*/
	char topic[SPL_TOPIC_SIZE]; /*Name of topic*/
	spl_gen_data_st *buf; /*Buff for writing*/
	int fizize; /*Size of file.*/
	void *fp; /*File stream.*/
} SIMPLE_LOG_TOPIC_ST;

typedef struct __SIMPLE_LOG_ST__ {
	int llevel; /*Level of log.*/
	int file_limit_size; /*Limitation of each log file. No nead SYNC.*/
	int buff_size; /*Buffer size for each buffer. No nead SYNC.*/
	int range; /*The limitation of usage buffer.*/
	int krange; /*The limitation of usage buffer.*/
	int max_sz_msg; /*If the size of the message is less than the number, it is safe to write. If not, it may be
			   truncated.*/
	int index; /*Index of default log, not in a topic. No nead SYNC.*/
	char folder[SPL_PATH_FOLDER]; /*Path of genera folder. No nead SYNC.*/
	char off; /*Must be sync*/
	void *mtx_rw; /*mtx: Need to close handle*/
	void *sem_rwfile; /*sem_rwfile: Need to close handle*/
	void *sem_off; /*sem_off: Need to close handle*/
	spl_local_time_st lc_time_now; /*Current time.*/
	FILE *fp; /*fp: Need to close*/
	spl_gen_data_st *buf; /*buf: Must be synchoronized. Must be freed.*/
	char *topics; /*topics: topics string. Must be freed */
	int n_topic; /*Number of topics, SIMPLE_LOG_TOPIC_ST.*/
	SIMPLE_LOG_TOPIC_ST *arr_topic; /*List od topics: SIMPLE_LOG_TOPIC_ST. Must be freed*/
	int renew; /*In a thread of logger, NO NEED SYNC.*/
	char path_template[SPL_TEMPLATE_LEN]; /*In a thread of logger, NO NEED SYNC.*/
	int ncpu; /*Number of CPU.*/
	int trigger_thread; /*Use trigger thread or not.*/
	void **arr_mtx; /*List of lock: Spinlock or Mutex. Must be freed.*/
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef UNIX_LINUX
	void *
#else
	int
#endif
	    hd; /* Handle of shared memory.*/
	char shared_key[SPL_SHARED_KEY_LEN]; /* Name of shared key.
		For safety, you should have a different key for your app,
		although it just need for MACOSX.*/
	char id_name[SPL_IDD_NAME]; /*To avoid duplicating of file name.*/
	char isProcessMode; /*For cross processes mode.*/
	int map_mem_size; /*Total mapped memory.*/
	char is_master;
	SPL_CALLBACL_FUNCTION fn; /* Callback function pointer (fn(obj)). */
	SPL_CALLBACL_DATA *obj;
} SIMPLE_LOG_ST;

typedef struct __SPL_INPUT_ARG__ {
	char folder[SPL_PATH_FOLDER]; /* Output folder of the logger. */
	char id_name[SPL_IDD_NAME]; /* Identifier name for the logger*/
	SPL_CALLBACL_FUNCTION fn; /* Callback function pointer (fn(obj)). */
	SPL_CALLBACL_DATA *obj; /* Data pointer passed to the callback function (fn(obj)). */
} SPL_INPUT_ARG;
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#define __FILLE__(__p__)                                                                                                    \
	do {                                                                                                                \
		__p__ = strrchr(__FILE__, '/');                                                                             \
		if (__p__) {                                                                                                \
			++__p__;                                                                                            \
			break;                                                                                              \
		}                                                                                                           \
		__p__ = strrchr(__FILE__, '\\');                                                                            \
		if (__p__) {                                                                                                \
			++__p__;                                                                                            \
			break;                                                                                              \
		}                                                                                                           \
		__p__ = __FILE__;                                                                                           \
	} while (0);

#ifndef __UNIX_LINUX_CPP11_AND_NEWERS__
#define spl_console_log(___fmttt___, ...)                                                                                   \
	{                                                                                                                   \
		char buf[1024];                                                                                             \
		const char *pfn = 0;                                                                                        \
		__FILLE__(pfn);                                                                                             \
		spl_fmmt_now(buf, 1024);                                                                                    \
		fprintf(stdout,                                                                                             \
		    "[%s] [%s:%s:%d] [thid: %llu] "___fmttt___                                                              \
		    "\n",                                                                                                   \
		    (char *)buf, (char *)pfn, (char *)__FUNCTION__, (int)__LINE__, spl_get_threadid(), ##__VA_ARGS__);      \
	}
#else
#define spl_console_log(___fmttt___, ...)                                                                                   \
	{                                                                                                                   \
		std::string __c11fmt__ = "[%s] [%s:%s:%d] [thid: %llu] ";                                                   \
		__c11fmt__ += ___fmttt___;                                                                                  \
		__c11fmt__ += "\n";                                                                                         \
		;                                                                                                           \
		char buf[1024];                                                                                             \
		const char *pfn = 0;                                                                                        \
		__FILLE__(pfn);                                                                                             \
		spl_fmmt_now(buf, 1024);                                                                                    \
		fprintf(stdout, __c11fmt__.c_str(), (char *)buf, (char *)pfn, (char *)__FUNCTION__, (int)__LINE__,          \
		    spl_get_threadid(), ##__VA_ARGS__);                                                                     \
	}
#endif

#define spl_malloc(__nn__, __obj__, __type__)                                                                               \
	{                                                                                                                   \
		(__obj__) = (__type__ *)malloc(__nn__);                                                                     \
		if (__obj__) { /*spl_console_log("Malloc: 0x%p\n", (__obj__));*/                                            \
			;                                                                                                   \
			memset((void *)(__obj__), 0, (__nn__));                                                             \
		} else {                                                                                                    \
			spl_console_log("Malloc: error.\n");                                                                \
		}                                                                                                           \
	}

#define spl_free(__obj__)                                                                                                   \
	{ /*spl_console_log("Free: 0x%p.\n", (__obj__));*/                                                                  \
		;                                                                                                           \
		free(__obj__);                                                                                              \
		;                                                                                                           \
		(__obj__) = 0;                                                                                              \
	}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#define SPL_KEYBUF(__t__, __i__) ((spl_gen_data_st *)((char *)__t__->buf + (__t__->buff_size * __i__)))
#define __spl_log_buf_level__(__lv__, ___fmttt___, ...)                                                                     \
	{                                                                                                                   \
		;                                                                                                           \
		SIMPLE_LOG_ST *__t__ = spl_control_obj();                                                                   \
		if (__t__->llevel <= (__lv__) && ___fmttt___[0]) {                                                          \
			;                                                                                                   \
			;                                                                                                   \
			int __outlen__ = 0;                                                                                 \
			;                                                                                                   \
			const char *__pfn__ = 0; /*char __isOof = 0;*/                                                      \
			;                                                                                                   \
			;                                                                                                   \
			unsigned short __r__ = 0;                                                                           \
			;                                                                                                   \
			char __tnow__[SPL_RL_BUF];                                                                          \
			char *__pprefmt__ = 0;                                                                              \
			;                                                                                                   \
			;                                                                                                   \
			;                                                                                                   \
			__FILLE__(__pfn__);                                                                                 \
			__pprefmt__ = spl_fmt_now_ext(                                                                      \
			    __tnow__, SPL_RL_BUF, __lv__, __pfn__, __FUNCTION__, __LINE__, &__r__, &__outlen__);            \
			;                                                                                                   \
			{                                                                                                   \
				do {                                                                                        \
					;                                                                                   \
					int __len__ = 0;                                                                    \
					;                                                                                   \
                                                                                                                            \
					;                                                                                   \
					;                                                                                   \
					;                                                                                   \
					spl_mutex_lock(__t__->arr_mtx[__r__]);                                              \
					;                                                                                   \
					if (__t__->range > SPL_KEYBUF(__t__, __r__)->pl) {                                  \
						;                                                                           \
						memcpy(SPL_KEYBUF(__t__, __r__)->data + SPL_KEYBUF(__t__, __r__)->pl,       \
						    __pprefmt__, __outlen__);                                               \
						;                                                                           \
						SPL_KEYBUF(__t__, __r__)->pl += __outlen__;                                 \
						;                                                                           \
						__len__ =                                                                   \
						    snprintf(SPL_KEYBUF(__t__, __r__)->data + SPL_KEYBUF(__t__, __r__)->pl, \
							__t__->krange - SPL_KEYBUF(__t__, __r__)->pl, ___fmttt___,          \
							##__VA_ARGS__);                                                     \
						;                                                                           \
						if (__len__ > 0) {                                                          \
							;                                                                   \
							__outlen__ = SPL_MIN_AB(                                            \
							    __len__, __t__->krange - SPL_KEYBUF(__t__, __r__)->pl);         \
							;                                                                   \
							SPL_KEYBUF(__t__, __r__)->pl += __outlen__;                         \
							;                                                                   \
						};                                                                          \
					}                                                                                   \
                                                                                                                            \
					spl_mutex_unlock(__t__->arr_mtx[__r__]);                                            \
                                                                                                                            \
					if (__len__ > 0)                                                                    \
						break;                                                                      \
					; /*spl_console_log("--OVER ===                                                     \
					     r: %d", (int)r);*/                                                             \
					;                                                                                   \
					__r__++;                                                                            \
					__r__ %= __t__->ncpu;                                                               \
					;                                                                                   \
					;                                                                                   \
					continue;                                                                           \
				} while (1);                                                                                \
				if (!__t__->trigger_thread)                                                                 \
					spl_rel_sem(__t__->sem_rwfile);                                                     \
				if (__pprefmt__ != __tnow__) {                                                              \
					spl_free(__pprefmt__);                                                              \
				}                                                                                           \
			}                                                                                                   \
		}                                                                                                           \
	}

#define SPL_ST_LOGBUFTOPIC(__t__, __i__) (&(__t__->arr_topic[__i__]))->buf
#define SPL_ST_LOGBUFTOPIC_RANGE(__t__, __i__, __r__)                                                                       \
	((spl_gen_data_st *)((char *)SPL_ST_LOGBUFTOPIC(__t__, __i__) + __t__->buff_size * __r__))

#define __spl_log_buf_topic_level__(__lv__, __tpic__, ___fmttt___, ...)                                                     \
	{                                                                                                                   \
		;                                                                                                           \
		SIMPLE_LOG_ST *__t__ = spl_control_obj();                                                                   \
		;                                                                                                           \
		if (__t__->llevel <= (__lv__) && ___fmttt___[0] && __t__->arr_topic) {                                      \
			;                                                                                                   \
			short __tpp__ = 0;                                                                                  \
			int __len__ = 0;                                                                                    \
			unsigned short __r__ = 0;                                                                           \
			;                                                                                                   \
			const char *__pfn__ = 0;                                                                            \
			;                                                                                                   \
			;                                                                                                   \
			int __outlen__ = 0;                                                                                 \
			;                                                                                                   \
			char *__pprefmt__ = 0;                                                                              \
			;                                                                                                   \
			char __tnow__[SPL_RL_BUF];                                                                          \
			;                                                                                                   \
			__tpp__ = __tpic__ % __t__->n_topic;                                                                \
			;                                                                                                   \
			;                                                                                                   \
			;                                                                                                   \
			__FILLE__(__pfn__);                                                                                 \
			;                                                                                                   \
			;                                                                                                   \
			__pprefmt__ = spl_fmt_now_ext(                                                                      \
			    __tnow__, SPL_RL_BUF, __lv__, __pfn__, __FUNCTION__, __LINE__, &__r__, &__outlen__);            \
			;                                                                                                   \
			do {                                                                                                \
				;                                                                                           \
				;                                                                                           \
				spl_mutex_lock(__t__->arr_mtx[__r__]);                                                      \
				/*do                                                                                        \
				{*/                                                                                         \
				/*if(__t__->arr_topic){*/;                                                                  \
				;                                                                                           \
				;                                                                                           \
				;                                                                                           \
				if (__t__->range > SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl) {                   \
					;                                                                                   \
					memcpy(SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->data +                      \
						   SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl,                     \
					    __pprefmt__, __outlen__);                                                       \
					;                                                                                   \
					SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl += __outlen__;                  \
					;                                                                                   \
					;                                                                                   \
					__len__ = snprintf(SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->data +          \
							       SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl,         \
					    __t__->krange - SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl,            \
					    ___fmttt___, ##__VA_ARGS__);                                                    \
					; /*spl_console_log("--------------lllllllennnnnnnnnnnnnnnnn---r: %d, len: %d",     \
					     (int)r, len);*/                                                                \
					;                                                                                   \
					if (__len__ > 0) {                                                                  \
						;                                                                           \
						__outlen__ = SPL_MIN_AB(__len__,                                            \
						    __t__->krange - SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl);   \
						;                                                                           \
						;                                                                           \
						SPL_ST_LOGBUFTOPIC_RANGE(__t__, __tpp__, __r__)->pl += __outlen__;          \
					}                                                                                   \
				}                                                                                           \
				/*}*/                                                                                       \
				/*}                                                                                         \
				while(0);*/                                                                                 \
				spl_mutex_unlock(__t__->arr_mtx[__r__]);                                                    \
				if (__len__ > 0)                                                                            \
					break;                                                                              \
				;                                                                                           \
				;                                                                                           \
				__r__++;                                                                                    \
				__r__ %= __t__->ncpu;                                                                       \
				;                                                                                           \
				continue;                                                                                   \
			} while (1);                                                                                        \
			if (!__t__->trigger_thread)                                                                         \
				spl_rel_sem(__t__->sem_rwfile);                                                             \
			;                                                                                                   \
			if (__pprefmt__ != __tnow__) {                                                                      \
				spl_free(__pprefmt__);                                                                      \
			}                                                                                                   \
		}                                                                                                           \
	}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

/* Please demo with spl_init_log */
DLL_API_SIMPLE_LOG int
spl_init_log(char *path);

/* Please demo with spl_init_log */
DLL_API_SIMPLE_LOG int
spl_init_log_ext(SPL_INPUT_ARG *input);

/*
 * Export name:	spllog
 * Sample:		spllog(SPL_LOG_INFO, "Hello spllog: %llu", time(0));
 */
#define spllog                          __spl_log_buf_level__

/*
 * Export name:	spllogtopic
 * Sample:		spllogtopic(SPL_LOG_INFO, 0, "Hello spllog: %llu", time(0));
 */
#define spllogtopic                     __spl_log_buf_topic_level__

/* Please demo with spl_finish_log */
DLL_API_SIMPLE_LOG int
spl_finish_log();

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

DLL_API_SIMPLE_LOG char *
spl_fmt_now_ext(char *fmtt, int len, 
	int lv, const char *filename, 
	const char *funcname, int line, 
	unsigned short *r, int *);

DLL_API_SIMPLE_LOG int
spl_mutex_lock(void *mtx);

DLL_API_SIMPLE_LOG int
spl_mutex_unlock(void *mtx);

DLL_API_SIMPLE_LOG int
spl_fmmt_now(char *fmtt, int len);

DLL_API_SIMPLE_LOG LLU
spl_get_threadid();

DLL_API_SIMPLE_LOG int
spl_rel_sem(void *sem);

DLL_API_SIMPLE_LOG
void
spl_sleep(unsigned int);
DLL_API_SIMPLE_LOG
void
spl_milli_sleep(unsigned int);

DLL_API_SIMPLE_LOG
LLU
spl_milli_now();

DLL_API_SIMPLE_LOG
SIMPLE_LOG_ST *
spl_control_obj();

DLL_API_SIMPLE_LOG const char *
spl_err_txt(int i);
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifdef __cplusplus
}
#endif
#endif
