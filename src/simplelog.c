
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
*		<2024-August-17>
* Decription:													
*		The (only) main file to implement simple log.
*/
/*===============================================================================================================*/

#include "simplelog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef UNIX_LINUX
	#include <Windows.h>
	#define YEAR_PADDING				0
	#define MONTH_PADDING				0
#else
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <pthread.h>
	#include <semaphore.h>
	#include <unistd.h>
	#include <errno.h>

	#define YEAR_PADDING				1900
	#define MONTH_PADDING				1
#endif

/*===========================================================================================================================*/

#define spl_malloc(__nn__, __obj__, __type__) { (__obj__) = (__type__*) malloc(__nn__); if(__obj__) \
	{spl_console_log("Malloc: 0x%p\n", (__obj__)); memset((__obj__), 0, (__nn__));} \
	else {spl_console_log("Malloc: error.\n");}} 

#define spl_free(__obj__) \
	{ spl_console_log("Free: 0x:%p.\n", (__obj__)); free(__obj__); ; (__obj__) = 0;} 

#define SPL_FCLOSE(__fp__, __n) { if(__fp__){ (__n) = fclose((FILE*)(__fp__)) ; if(__n) { spl_fclose_err(__n, __fp__); } \
	else { spl_console_log("Close FILE 0x%p DONE.", (__fp__));;(__fp__) = 0;;}}}

#define SPL_FFLUSH(__fp__, __n) { if(__fp__){ (__n) = fflush((FILE*)(__fp__)) ; if(__n) { spl_fflush_err(__n, __fp__); }}}

#define FFOPEN(__fp, __path, __mode) \
	{ (__fp) = fopen((__path), (__mode));spl_console_log("Open FILE error code: 0x%p, %s.\n", (__fp), (__fp) ? "DONE": "FAILED"); }

#define FFTELL(__fp)						ftell((FILE*)(__fp))
#define FFSEEK(__fp, __a, __b)				fseek((FILE*)(__fp), (__a), (__b))

#ifndef UNIX_LINUX
	#define SPL_CloseHandle(__obj) \
		{ int bl = CloseHandle((__obj)); spl_console_log("CloseHandle %s", bl ? "DONE": "ERROR");}
#else
	#define SPL_sem_wait(__obj) \
		sem_wait((sem_t*)(__obj))
	#define SPL_sem_post(__obj) \
		sem_post((sem_t*)(__obj))
	#define SPL_sem_destroy(__obj, __err) \
		{ (__err) = sem_destroy((sem_t*)(__obj)); if((__err)) spl_console_log("sem_destroy errcode: %d. %s\n", (__err), (__err) ? "FALIED": "DONE")}
	#define SPL_pthread_mutex_destroy(__obj, __err) \
		{ (__err) = pthread_mutex_destroy((pthread_mutex_t*)(__obj)); if((__err)) spl_console_log("pthread_mutex_destroy errcode: %d. %s\n", (__err), (__err) ? "FALIED": "DONE");}
	#define SPL_pthread_mutex_lock(__obj, __err) \
		{ (__err) = pthread_mutex_lock((pthread_mutex_t*)(__obj)); if((__err)) spl_console_log("pthread_mutex_lock errcode: %d. %s\n", (__err), (__err) ? "FALIED": "DONE");}
	#define SPL_pthread_mutex_unlock(__obj, __err) \
		{ (__err) = pthread_mutex_unlock((pthread_mutex_t*)(__obj)); if((__err)) spl_console_log("pthread_mutex_unlock errcode: %d. %s\n", (__err), (__err) ? "FALIED": "DONE");}
#endif
/*===========================================================================================================================*/

#define	SPLOG_PATHFOLDR \
	"pathfoder="
#define	SPLOG_LEVEL \
	"level="
#define	SPLOG_BUFF_SIZE \
	"buffsize="
#define	SPLOG_ROT_SIZE \
	"rotation_size="
#define	SPLOG_TOPIC \
	"topic="
#define	SPLOG_END_CFG \
	"end_configuring="

#define SPL_FILE_NAME_FMT \
	"%s\\%s\\%s_%.8d.log"
#define SPL_FILE_NAME_FMT_TOPIC \
	"%s\\%s\\%s"
#define SPL_FMT_DATE_ADDING \
	"%.4d-%.2d-%.2d"
#define SPL_FMT_HOUR_ADDING \
	"%.2d:%.2d:%.2d"
#define SPL_FMT_DELT_ADDING \
	"%s %s.%.3u (+%.7llu)"
#define SPL_FMT_MILL_ADDING \
	"%s %s.%.3d"

#define				SPL_TEXT_UNKNOWN				"U"
#define				SPL_TEXT_DEBUG					"D"
#define				SPL_TEXT_INFO					"I"
#define				SPL_TEXT_WARN					"W"
#define				SPL_TEXT_ERROR					"E"
#define				SPL_TEXT_FATAL					"F"


#define				SPL_MEMO_PADDING				2
/*===========================================================================================================================*/

typedef 
struct __GENERIC_DATA__ {
	int	
		total;	 
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
	spl_uint	ms;						/*Millisecond*/
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
		llevel;
	int
		file_limit_size;
		/*Limitation of each log file. No nead SYNC.*/
	int
		buff_size;
		/*Buffer size for each buffer. No nead SYNC.*/
	int
		index;
		/*Index of default log, not in a topic. No nead SYNC.*/
	char
		folder[1024];
		/*Path of genera folder. No nead SYNC.*/
	char
		off;					
		/*Must be sync*/
	void*
		mtx;					
		/*mtx: Need to close handle*/
	void*
		mtx_off;				
		/*mtx_off: Need to close handle*/
	void*
		sem_rwfile;				
		/*sem_rwfile: Need to close handle*/
	void*
		sem_off;				
		/*sem_off: Need to close handle*/
	spl_local_time_st
		lc_time_now;				
		/*lc_time: Need to sync, free*/
	FILE*
		fp;						
		/*fp: Need to close*/

	generic_dta_st*
		buf;
		/*buf: Must be sync, free*/
	char*
		topics;
		/*topics: topics string. Must be freed */
	int
		n_topic;
		/*Number of topics, SIMPLE_LOG_TOPIC_ST.*/
	SIMPLE_LOG_TOPIC_ST*
		arr_topic;
		/*List od topics: SIMPLE_LOG_TOPIC_ST*.*/
	int 
		renew;
		/*In a thread of logger, NO NEED SYNC.*/
	char
		path_template[1024];
		/*In a thread of logger, NO NEED SYNC.*/
} SIMPLE_LOG_ST;

typedef enum 
__CHANGE_NAME_E__ {
	SPL_NO_CHANGE_NAME = 0,
	SPL_CHANGE_FILE_SIZE,
	SPL_CHANGE_DAY,
	SPL_CHANGE_MONTH,
	SPL_CHANGE_YEAR,


	SPL_CHANGE_FINISH
} __CHANGE_NAME_E__;

/*===========================================================================================================================*/
static const char* __splog_pathfolder[] = { 
		SPLOG_PATHFOLDR, 
		SPLOG_LEVEL, 
		SPLOG_BUFF_SIZE, 
		SPLOG_ROT_SIZE, 
		SPLOG_TOPIC, 
		SPLOG_END_CFG,
		0 
};

static	SIMPLE_LOG_ST
	__simple_log_static__;;

/*===========================================================================================================================*/

static int	
	spl_init_log_parse(char* buff, char* key, char *);
static void*
	spl_sem_create(int ini);
static int	
	spl_verify_folder(char* folder);
static int	
	spl_simple_log_thread(SIMPLE_LOG_ST* t);
static int	
	spl_gen_file(SIMPLE_LOG_ST* t, int *n, int limit, int *);
static int
	spl_gen_topics(SIMPLE_LOG_ST* t);
static int	
	spl_get_fname_now(char* name);
static int	
	spl_get_fname_now(char* name);
static int	
	spl_folder_sup(char* folder, spl_local_time_st* lctime, char *year_month);
static int	
	spl_local_time_now(spl_local_time_st*st_time);
static int	
	spl_stdz_topics(char *buff, int *inoutlen, int *, char** );
static int 
	spl_gen_topic_buff(SIMPLE_LOG_ST* t);
#ifndef UNIX_LINUX
	static DWORD WINAPI
		spl_written_thread_routine(LPVOID lpParam);
#else
	static void*
		spl_written_thread_routine(void*);
#endif

static int 
	spl_fclose_err(int t, void *fpp);
static int
	spl_fflush_err(int t, void *fpp);

/*===========================================================================================================================*/
int spl_local_time_now(spl_local_time_st*stt) {
	int ret = 0;
#ifndef UNIX_LINUX
	SYSTEMTIME lt;
#else
	struct tm* lt, rlt;
	struct timespec nanosec;
#endif
	do {
		if (!stt) {
			ret = SPL_LOG_ST_NAME_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		GetLocalTime(&lt);
		stt->year = (unsigned int) lt.wYear;
		stt->month = (unsigned char)lt.wMonth;
		stt->day = (unsigned char)lt.wDay;

		stt->hour = (unsigned char)lt.wHour;
		stt->minute = (unsigned char)lt.wMinute;
		stt->sec = (unsigned char)lt.wSecond;
		stt->ms = (unsigned int)lt.wMilliseconds;
#else
/* https://linux.die.net/man/3/localtime*/
/* https://linux.die.net/man/3/clock_gettime*/
		time_t t = time(0);
		lt = localtime_r(&t, &rlt);
		if (!lt) {
			ret = SPL_LOG_TIME_NULL_ERROR;
			break;
		}
		lt = (struct tm*) &rlt;
		/*No need freeing, 
		//https://stackoverflow.com/questions/35031647/do-i-need-to-free-the-returned-pointer-from-localtime-function*/
		ret = clock_gettime(CLOCK_REALTIME, &nanosec);
		if (ret) {
			ret = SPL_LOG_TIME_NANO_NULL_ERROR;
			break;
		}
		stt->year = lt->tm_year;
		stt->month = lt->tm_mon;
		stt->day = lt->tm_mday;

		stt->hour = lt->tm_hour;
		stt->minute = lt->tm_min;
		stt->sec = lt->tm_sec;
		stt->ms = (nanosec.tv_nsec/1000000);
#endif
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int spl_set_log_levwel(int val) {
	__simple_log_static__.llevel = val;
	return 0;
}
/*===========================================================================================================================*/
int spl_get_log_levwel() {
	return __simple_log_static__.llevel;
}
/*===========================================================================================================================*/
int spl_set_off(int isoff) {
	int ret = 0;
	spl_mutex_lock(__simple_log_static__.mtx_off);
	do {
		__simple_log_static__.off = isoff;
	} while (0);
	spl_mutex_unlock(__simple_log_static__.mtx_off);
	
	if (isoff) {
		int errCode = 0;
		spl_rel_sem(__simple_log_static__.sem_rwfile);
#ifndef UNIX_LINUX
		//errCode = (int) WaitForSingleObject(__simple_log_static__.sem_off, 3 * 1000);
		errCode = (int) WaitForSingleObject(__simple_log_static__.sem_off, INFINITE);
#else
		errCode = SPL_sem_wait(__simple_log_static__.sem_off);
#endif
		spl_console_log("------- errCode: %d\n", (int)errCode);
	}
	return ret;
}
/*===========================================================================================================================*/
int spl_get_off() {
	int ret = 0;
	spl_mutex_lock(__simple_log_static__.mtx_off);
	do {
		ret = __simple_log_static__.off;
	} while (0);
	spl_mutex_unlock(__simple_log_static__.mtx_off);
	return ret;
}
/*===========================================================================================================================*/

int spl_init_log_parse(char* buff, char *key, char *isEnd) {
	int ret = SPL_NO_ERROR;
	do {
		if (strcmp(key, SPLOG_PATHFOLDR) == 0) {
			if (!buff[0]) {
				ret = SPL_INIT_PATH_FOLDER_EMPTY_ERROR;
				break;
			}
			snprintf(__simple_log_static__.folder, 1024, "%s", buff);
			break;
		}
		if (strcmp(key, SPLOG_LEVEL) == 0) {
			int n = 0;
			int count = 0;
			count = sscanf(buff, "%d", &n);
			if (n < 0 || count < 1) {
				ret = SPL_LOG_LEVEL_ERROR;
				break;
			}
			spl_set_log_levwel(n);
			break;
		}
		if (strcmp(key, SPLOG_BUFF_SIZE) == 0) {
			int n = 0;
			int sz = 0;
			//char* p = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPL_LOG_BUFF_SIZE_ERROR;
				break;
			}
			__simple_log_static__.buff_size = n;
			//spl_malloc(n, p, char);
			//if (!p) {
			//	ret = SPL_LOG_MEM_MALLOC_ERROR;
			//	break;
			//}
			//__simple_log_static__.buf = (generic_dta_st *) p;
			//__simple_log_static__.buf->total = n -1;
			break;
		}
		if (strcmp(key, SPLOG_ROT_SIZE) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPL_LOG_ROT_SIZE_ERROR;
				break;
			}
			__simple_log_static__.file_limit_size = n;
			spl_console_log("__simple_log_static__.file_limit_size: %d.\n", __simple_log_static__.file_limit_size);
			break;
		}
		if (strcmp(key, SPLOG_TOPIC) == 0) {
			int n = 0, count = 0;
			char* p = 0;
			n = (int)strlen(buff);
			if (n < 1) {
				//ret = SPL_LOG_TOPIC_EMPTY;
				break;
			}
			ret = spl_stdz_topics(buff, &n, &count, &p);
			if(ret) {
				break;
			}
			__simple_log_static__.n_topic = count;
			__simple_log_static__.topics = p;
			break;
		}
		if (strcmp(key, SPLOG_END_CFG) == 0) {
			spl_console_log("End configuration.\n");
			if (isEnd) {
				*isEnd = 1;
			}
			break;
		}
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int 
spl_init_log( char *pathcfg) 
{
	int ret = 0;
	FILE* fp = 0;
	char c = 0;
	int count = 0;
	char buf[1024];
	void* obj = 0;
	char isEnd = 0;
	do {
		memset(buf, 0, sizeof(buf));
		//fp = fopen(pathcfg, "r");
		FFOPEN(fp, pathcfg, "r");
		if (!fp) {
			ret = 1;
			spl_console_log("Cannot open file error.");
			break;
		}
		while (1) {
			c = fgetc(fp);
			if (c == '\r' || c == '\n' || c == EOF) {
				int  j = 0;
				char* node = 0;
				if (count < 1) {
					continue;
				}
				while (1) {
					char* pp = 0;
					node = (char *)__splog_pathfolder[j];
					if (!node) {
						break;
					}
					pp = strstr(buf, node);
					if (pp)
					{
						char* p = 0;
						size_t k = 0; 
						k =  strlen(node);
						p = (buf + k);
						spl_console_log("Find out the keyword: [%s] value [%s].", node, p);
						ret = spl_init_log_parse(p, node, &isEnd);
						break;
					}
					j++;
				}

				if (ret) {
					break;
				}			
				count = 0;
				memset(buf, 0, sizeof(buf));

				if (isEnd) {
					break;
				}
				if (c == EOF) {
					break;
				}
				continue;
				
			}
			if (isEnd) {
				break;
			}
			if (c == EOF) {
				break;
			}
			buf[count++] = c;
		}
		if (ret) {
			break;
		}
		obj = spl_mutex_create();
		if (!obj) {
			ret = SPL_ERROR_CREATE_MUTEX;
			break;
		}
		__simple_log_static__.mtx = obj;

		obj = spl_mutex_create();
		if (!obj) {
			ret = SPL_ERROR_CREATE_MUTEX;
			break;
		}
		__simple_log_static__.mtx_off = obj;
		
		obj = spl_sem_create(1);
		if (!obj) {
			ret = SPL_ERROR_CREATE_SEM;
			break;
		}
		__simple_log_static__.sem_rwfile = obj;

		obj = spl_sem_create(1);
		if (!obj) {
			ret = SPL_ERROR_CREATE_SEM;
			break;
		}
		__simple_log_static__.sem_off = obj;

		char* folder = __simple_log_static__.folder;
		ret = spl_verify_folder(folder);
		if (ret) {
			break;
		}
	} while (0);
	if (fp) {
		SPL_FCLOSE(fp,ret);
	}
	if (ret == 0) {
		ret = spl_gen_topic_buff(&__simple_log_static__);
	}
	if (ret == 0) {
		ret = spl_simple_log_thread(&__simple_log_static__);
	}

	return ret;
}
/*===========================================================================================================================*/
void* spl_mutex_create() {
	void *ret = 0;
	do {
#ifndef UNIX_LINUX
		ret = CreateMutexA(0, 0, 0);
#else
	/*https://linux.die.net/man/3/pthread_mutex_init*/
		spl_malloc(sizeof(pthread_mutex_t), ret, void);
		//ret = malloc(sizeof(pthread_mutex_t));
		if (!ret) {
			break;
		}
		memset(ret, 0, sizeof(pthread_mutex_t));
		pthread_mutex_init((pthread_mutex_t*)ret, 0);
#endif 
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
void* spl_sem_create(int ini) {
	void* ret = 0;
	do {
#ifndef UNIX_LINUX
		ret = CreateSemaphoreA(0, 0, ini, 0);
#else
		//ret = malloc(sizeof(sem_t));
		spl_malloc(sizeof(sem_t), ret, void);
		if (!ret) {
			break;
		}
		memset(ret, 0, sizeof(sem_t));
		sem_init((sem_t*)ret, 0, 0);
#endif 
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int spl_mutex_lock(void* obj) {
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD err = 0;
#else
#endif	
	do {
		if (!obj) {
			ret = SPL_LOG_MUTEX_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		err = WaitForSingleObject(obj, INFINITE);
		if (err != WAIT_OBJECT_0) {
			ret = 1;
			break;
		}
#else
		SPL_pthread_mutex_lock((pthread_mutex_t*)obj, ret);
#endif
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int spl_mutex_unlock(void* obj) {
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD done = 0;
#else
#endif	
	do {
		if (!obj) {
			ret = SPL_LOG_MUTEX_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		done = ReleaseMutex(obj);
		if (!done) {
			ret = 1;
			break;
		}
#else
		SPL_pthread_mutex_unlock((pthread_mutex_t*)obj, ret);
#endif
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int spl_verify_folder(char* folder) {
	int ret = 0;
	do {
#ifdef WIN32
		/*https://learn.microsoft.com/en-us/windows/win32/fileio/retrieving-and-changing-file-attributes
		// https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya
		// ERROR_ALREADY_EXISTS, ERROR_PATH_NOT_FOUND */
		BOOL result = CreateDirectoryA(folder, NULL);
		if (!result) {
			DWORD werr = GetLastError();
			if (werr == ERROR_ALREADY_EXISTS) {
				//ret = 1;
				break;
			}
			ret = SPL_LOG_FOLDER_ERROR;
		}
#endif
	} while (0);
	return ret;
}
/*https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemtime*/
/*===========================================================================================================================*/
int spl_get_fname_now(char* name) {
	int ret = 0;
	spl_local_time_st lt;
	spl_local_time_now(&lt);
	if (name) {
		snprintf(name, 64, "%.4d-%.2d-%.2d-simplelog", lt.year + YEAR_PADDING, lt.month + MONTH_PADDING, lt.day);
	}
	return ret;
}
#include <time.h>
/*===========================================================================================================================*/
#ifndef UNIX_LINUX
DWORD WINAPI spl_written_thread_routine(LPVOID lpParam)
#else
void* spl_written_thread_routine(void* lpParam)
#endif
{	
	int k = 0;
	SIMPLE_LOG_ST* t = (SIMPLE_LOG_ST*)lpParam;
	int ret = 0, off = 0, sz = 0, i = 0, err = 0;
	char* buffer = 0;
	int total_buf_sz = 0;
	generic_dta_st* tmpBuff = 0;
	total_buf_sz = t->buff_size * (1 + t->n_topic);
	spl_malloc(total_buf_sz, buffer, char);
	
	do {	
		if (!buffer) {
			ret = SPL_LOG_TOPIC_BUFF_MEM;
			break;
		}
		if (!t) {
			exit(1);
		}
		if (!t->sem_rwfile) {
			exit(1);
		}
		spl_console_log("Semaphore: 0x%p.\n", t->sem_rwfile);
		if (!t->mtx) {
			exit(1);
		}
		//spl_console_log("Mutex: 0x%p.\n", t->mtx);
		while (1) {
			
#ifndef UNIX_LINUX
			WaitForSingleObject(t->sem_rwfile, INFINITE);
#else
			SPL_sem_wait(t->sem_rwfile);
#endif
			off = spl_get_off();
			if (off) {
				break;
			}
			ret = spl_gen_file(t, &sz, t->file_limit_size, &(t->index));
			if (ret) {
				spl_console_log("--spl_gen_file, ret: %d --\n", ret);
				continue;
			}
			ret = spl_gen_topics(t);
			if (ret) {
				spl_console_log("--spl_gen_topics, ret: %d --\n", ret);
				continue;
			}
			spl_mutex_lock(t->mtx);
			do {
				
				if (t->buf->pl > t->buf->pc) {
					
					memcpy(buffer, t->buf, sizeof(generic_dta_st) + t->buf->pl + 1);
					t->buf->pl = t->buf->pc = 0;
				}
				for (i = 0; i < t->n_topic; ++i) {
					if (t->arr_topic[i].buf->pl > t->arr_topic[i].buf->pc) {
						memcpy(buffer + (t->buff_size * (i + 1)), t->arr_topic[i].buf, sizeof(generic_dta_st) + t->arr_topic[i].buf->pl + 1);
						t->arr_topic[i].buf->pl = t->arr_topic[i].buf->pc = 0;
					}
				}
			} while (0);
			spl_mutex_unlock(t->mtx);
			tmpBuff = (generic_dta_st*) buffer;
			k = (int)fwrite(tmpBuff->data, 1, tmpBuff->pl, t->fp);
			sz += k;

			//err = SPL_FFLUSH((FILE *)(t->fp));

			SPL_FFLUSH((t->fp), err);

			tmpBuff->pl = 0;
			if (err) {
				//TO-TEST
				ret = SPL_LOG_TOPIC_FLUSH;
				spl_console_log("--fflush, ret: %d --\n", err);
				break;
			}
			for (i = 0; i < t->n_topic; ++i) {
				//TO-TEST
				tmpBuff = (generic_dta_st*)(buffer + (t->buff_size * (i + 1)));
				k = (int)fwrite(tmpBuff->data, 1, tmpBuff->pl, (FILE*)(t->arr_topic[i].fp));
				t->arr_topic[i].fizize += k;

				//err = SPL_FFLUSH((FILE *)(t->arr_topic[i].fp));
				SPL_FFLUSH((t->arr_topic[i].fp), err);

				tmpBuff->pl = 0;
				if (err) {
					spl_console_log("--fflush, ret: %d --\n", err);
					ret = SPL_LOG_TOPIC_FLUSH;
					break;
				}
			}
			if (ret) {
				break;
			}
		}
		if (t->fp) {
			int werr = 0;
			SPL_FCLOSE(t->fp, werr);
			for (i = 0; i < t->n_topic; ++i) {
				if (t->arr_topic[i].fp) {
					SPL_FCLOSE(t->arr_topic[i].fp, werr);
				}
			}
			spl_mutex_lock(t->mtx);
				if (t->buf) {
					spl_free(t->buf);
				}
				for (i = 0; i < t->n_topic; ++i) {
					if (t->arr_topic[i].buf) {
						t->arr_topic[i].buf = 0;
					}
				}
			spl_mutex_unlock(t->mtx);
		}

		spl_free(buffer);
	} while (0);
	
	/*Send a signal to the waiting thread.*/
	spl_rel_sem(__simple_log_static__.sem_off);
	return 0;
}
/*===========================================================================================================================*/
int spl_simple_log_thread(SIMPLE_LOG_ST* t) {
	int ret = 0;
	do {
#ifndef UNIX_LINUX
		HANDLE hd = 0;
		DWORD thread_id = 0;
		hd = CreateThread(NULL, 0, spl_written_thread_routine, t, 0, &thread_id);
		if (!hd) {
			ret = SPL_LOG_CREATE_THREAD_ERROR;
			break;
		}
#else
		pthread_t idd = 0;
		int err = 0;
		tzset();
		err = pthread_create(&idd, 0, spl_written_thread_routine , t);
		if (err) {
			ret = SPL_LOG_CREATE_THREAD_ERROR;
			break;
		}
#endif
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int spl_fmt_now(char* fmtt, int len) {
	int ret = 0;
	spl_local_time_st stt;
	static LLU pre_tnow = 0;
	LLU _tnow = 0;
	LLU _delta = 0;
	int n = 0; 
	char buff[20], buff1[20];
	memset(buff, 0, 20);
	memset(buff1, 0, 20);	

	time_t t = time(0);
	do {
		memset(&stt, 0, sizeof(stt));
		ret = spl_local_time_now(&stt);
		if (ret) {
			break;
		}
		if (!fmtt) {
			ret = (int) SPL_LOG_FMT_NULL_ERROR;
			break;
		}
		
		_tnow = t;
		_tnow *= 1000;
		_tnow += stt.ms;
		do {
			spl_mutex_lock(__simple_log_static__.mtx_off);
				do {

					if (!pre_tnow) {
						break;
					}
					if (_tnow > pre_tnow) {
						_delta = _tnow - pre_tnow;
					}
				} while (0);
				pre_tnow = _tnow;
			spl_mutex_unlock(__simple_log_static__.mtx_off);
		} while (0);

		n = snprintf(buff, 20, SPL_FMT_DATE_ADDING, stt.year + YEAR_PADDING, stt.month + MONTH_PADDING, stt.day);
		if (n < 1) {
			ret = SPL_LOG_PRINTF_ERROR;
			break;
		}
		n = snprintf(buff1, 20, SPL_FMT_HOUR_ADDING, stt.hour, stt.minute, stt.sec);
		n = snprintf(fmtt, len, SPL_FMT_DELT_ADDING, buff, buff1, (unsigned int)stt.ms, _delta);

	} while (0);
	return ret;
}

/*===========================================================================================================================*/
int spl_fmmt_now(char* fmtt, int len) {
	int ret = 0;
	spl_local_time_st stt;
	char buff[20], buff1[20];
	int n = 0; 
	do {
		if (!fmtt) {
			ret = (int)SPL_LOG_FMT_NULL_ERROR;
			break;
		}
		memset(&stt, 0, sizeof(stt));
		ret = spl_local_time_now(&stt);
		if (ret) {
			break;
		}
		
		memset(buff, 0, 20);
		memset(buff1, 0, 20);

		n = snprintf(buff, 20, SPL_FMT_DATE_ADDING, stt.year + YEAR_PADDING, stt.month + MONTH_PADDING, stt.day);
		if (n < 1) {
			ret = SPL_LOG_PRINTF_ERROR;
			break;
		}
		n = snprintf(buff1, 20, SPL_FMT_HOUR_ADDING, stt.hour, stt.minute, stt.sec);
		n = snprintf(fmtt, len, SPL_FMT_MILL_ADDING, buff, buff1, (int)stt.ms);

	} while (0);
	return ret;
}

/*===========================================================================================================================*/

int spl_gen_file(SIMPLE_LOG_ST* t, int *sz, int limit, int *index) {
	int ret = 0;
	spl_local_time_st lt,* plt = 0;;
	//int renew = SPL_NO_CHANGE_NAME;
	char path[1024];
	char fmt_file_name[64];
	int ferr = 0;
	char yearmonth[16];
	
	do {
		t->renew = SPL_NO_CHANGE_NAME;
		ret = spl_local_time_now(&lt);
		if (ret) {
			spl_console_log("spl_local_time_now: ret: %d.\n", ret);
			break;
		}

		memcpy(&(t->lc_time_now), &lt, sizeof(spl_local_time_st));
		plt = &(t->lc_time_now);
		if (!t->fp) {
			memset(path, 0, sizeof(path));
			memset(fmt_file_name, 0, sizeof(fmt_file_name));
			spl_get_fname_now(fmt_file_name);
			ret = spl_folder_sup(t->folder, &(t->lc_time_now), yearmonth);
			if (ret) {
				spl_console_log("spl_folder_sup: ret: %d.\n", ret);
				break;
			}
			do {
				int err = 0;
				int cszize = 0; 
				snprintf(path, 1024, SPL_FILE_NAME_FMT, t->folder, yearmonth, fmt_file_name, *index);
				snprintf(t->path_template, 1024, SPL_FILE_NAME_FMT_TOPIC, t->folder, yearmonth, fmt_file_name);
				spl_standardize_path(path);
				spl_standardize_path(t->path_template);
				
				//t->fp = fopen(path, "a+");
				FFOPEN(t->fp, path, "a+");
				if (!t->fp) {
					ret = SPL_LOG_OPEN_FILE_ERROR;
					break;
				}
				FFSEEK(t->fp, 0, SEEK_END);
				cszize = FFTELL(t->fp);
				if (cszize < limit) {
					*sz = cszize;
					break;
				}
				SPL_FCLOSE(t->fp, err);
				if(err) {
					ret = SPL_LOG_CLOSE_FILE_ERROR;
				}
				(*index)++;

			} while (1);
			if (ret) {
				break;
			}
			break;
		}
		if (ret) {
			break;
		}
		do {
			if (*sz > limit) {
				(*index)++;
				t->renew = SPL_CHANGE_FILE_SIZE;
				break;
			}
			if (lt.year > plt->year) {
				(*index) = 0;
				t->renew = SPL_CHANGE_YEAR;
				break;
			}
			if (lt.month > plt->month) {
				t->renew = SPL_CHANGE_MONTH;
				(*index) = 0;
				break;
			}
			if (lt.day > plt->day) {
				(*index) = 0;
				t->renew = SPL_CHANGE_DAY;
				break;
			}
			t->renew = SPL_NO_CHANGE_NAME;
		} while (0);
		if (!t->renew) {
			break;
		}
		memcpy(&(t->lc_time_now), &lt, sizeof(spl_local_time_st));
		spl_get_fname_now(fmt_file_name);
		ret = spl_folder_sup(t->folder, &(t->lc_time_now), yearmonth);
		if (ret) {
			spl_console_log("spl_folder_sup: ret: %d.\n", ret);
			break;
		}
		snprintf(path, 1024, SPL_FILE_NAME_FMT, t->folder, yearmonth, fmt_file_name, *index);
		snprintf(t->path_template, 1024, SPL_FILE_NAME_FMT_TOPIC, t->folder, yearmonth, fmt_file_name);

		SPL_FCLOSE(t->fp, ferr);
		if (ferr) {
			ret = SPL_LOG_CLOSE_FILE_ERROR;
			break;
		}
		spl_standardize_path(path);
		spl_standardize_path(t->path_template);
		//t->fp = fopen(path, "a+");
		FFOPEN(t->fp, path, "a+");
		if (sz) {
			*sz = 0;
		}
		if (!t->fp) {
			ret = SPL_LOG_OPEN1_FILE_ERROR;
			break;
		}
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
void* spl_get_mtx() {
	return __simple_log_static__.mtx;
}
/*===========================================================================================================================*/
void* spl_get_sem_rwfile() {
	return __simple_log_static__.sem_rwfile;
}
/*===========================================================================================================================*/
LLU	spl_get_threadid() {
#ifndef UNIX_LINUX
	return (LLU)GetCurrentThreadId();
#else
	return (LLU)pthread_self();
#endif
}
/*===========================================================================================================================*/
int spl_rel_sem(void *sem) {
	int ret = 0;
#ifndef UNIX_LINUX
#else
	int err = 0, val = 0;
#endif
	do {
		if (!sem) {
			ret = SPL_LOG_SEM_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		ReleaseSemaphore(sem, 1, 0);
#else
		err = sem_getvalue((sem_t*)sem, &val);
		if (!err) {
			if (val < 1) {
				SPL_sem_post(sem);
			}
		}
#endif 
	} while (0);
	return ret;
}
/*===========================================================================================================================*/

const char* spl_get_text(int lev) {
	const char* val = SPL_TEXT_UNKNOWN;
	do {
		if (lev == SPL_LOG_DEBUG) {
			val = SPL_TEXT_DEBUG;
			break;
		}
		if (lev == SPL_LOG_INFO) {
			val = SPL_TEXT_INFO;
			break;
		}
		if (lev == SPL_LOG_WARNING) {
			val = SPL_TEXT_WARN;
			break;
		}
		if (lev == SPL_LOG_ERROR) {
			val = SPL_TEXT_ERROR;
			break;
		}
		if (lev == SPL_LOG_FATAL) {
			val = SPL_TEXT_FATAL;
			break;
		}
	} while(0);
	return val;
}
/*===========================================================================================================================*/
int spl_finish_log() {
	int ret = 0; 
	spl_set_off(1);
#ifndef UNIX_LINUX
	SPL_CloseHandle(__simple_log_static__.mtx);
	SPL_CloseHandle(__simple_log_static__.mtx_off);
	SPL_CloseHandle(__simple_log_static__.sem_rwfile);
	SPL_CloseHandle(__simple_log_static__.sem_off);
#else
	int err = 0;
/*https://linux.die.net/man/3/SPL_sem_destroy
//https://linux.die.net/man/3/pthread_mutex_init*/
	SPL_pthread_mutex_destroy(__simple_log_static__.mtx, err);
	spl_free(__simple_log_static__.mtx);
	SPL_pthread_mutex_destroy(__simple_log_static__.mtx_off, err);
	spl_free(__simple_log_static__.mtx_off);
	SPL_sem_destroy(__simple_log_static__.sem_rwfile, err);
	spl_free(__simple_log_static__.sem_rwfile);
	SPL_sem_destroy(__simple_log_static__.sem_off, err);
	spl_free(__simple_log_static__.sem_off);
#endif

	spl_free(__simple_log_static__.topics);
	spl_free(__simple_log_static__.arr_topic);

	memset(&__simple_log_static__, 0, sizeof(__simple_log_static__));
	return ret;
}
/*===========================================================================================================================*/
char* spl_get_buf(int* n, int** ppl) {
	SIMPLE_LOG_ST* t = &__simple_log_static__;
	char* ret = 0;
	if (t->buf) {
		if (n && ppl) {
			*n = (t->buf->total > sizeof(generic_dta_st) + t->buf->pl) ? (t->buf->total - sizeof(generic_dta_st) - t->buf->pl) : 0;
			ret = t->buf->data;
			(*ppl) = &(t->buf->pl);
		}
	}
	return ret;
}
/*===========================================================================================================================*/
/*https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya*/
int spl_folder_sup(char* folder, spl_local_time_st* lctime, char* year_month) {
	int ret = 0;
	char path[1024];
#ifndef UNIX_LINUX
	int result = 0;
#else
	int err = 0;
	struct stat buf;
#endif	
	memset(path, 0, sizeof(path));
	do {
		if (!folder) {
			ret = SPL_LOG_CHECK_FOLDER_NULL_ERROR;
			break;
		}
		if (!lctime) {
			ret = SPL_LOG_CHECK_FOLDER_NULL_ERROR;
			break;
		}
		if (!year_month) {
			ret = SPL_LOG_CHECK_FOLDER_NULL_ERROR;
			break;
		}
		snprintf(path, 1024, "%s", folder);
#ifndef UNIX_LINUX
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPL_LOG_CHECK_FOLDER_ERROR;
				break;
			}
		}
		snprintf(path, 1024, "%s/%0.4u", folder, lctime->year + YEAR_PADDING);
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPL_LOG_CHECK_FOLDER_YEAR_ERROR;
				break;
			}
		}
		snprintf(path, 1024, "%s/%0.4d/%0.2d", folder, (int)lctime->year + YEAR_PADDING, (int) lctime->month + MONTH_PADDING);
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPL_LOG_CHECK_FILE_YEAR_ERROR;
				break;
			}
		}
#else
	/*https://linux.die.net/man/3/mkdir
	//https://linux.die.net/man/2/stat*/
		memset(&buf, 0, sizeof(buf));
		stat(path, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			memset(&buf, 0, sizeof(buf));
			stat(path, &buf);
			if (!S_ISDIR(buf.st_mode)) {
				ret = SPL_LOG_CHECK_FOLDER_ERROR;
				spl_console_log("Mkdir err path: %s, err: %d\n", path, err);
				break;
				
			}
		}

		memset(&buf, 0, sizeof(buf));
		snprintf(path, 1024, "%s/%.4u", folder, lctime->year + YEAR_PADDING);
		err = stat(path, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (err) {
				ret = SPL_LOG_CHECK_FOLDER_YEAR_ERROR;
				spl_console_log("Mkdir err path: %s, err: %d\n", path, err);
				break;
			}
		}
		memset(&buf, 0, sizeof(buf));
		snprintf(path, 1024, "%s/%.4d/%.2d", folder, (int)lctime->year + YEAR_PADDING, (int)lctime->month + MONTH_PADDING);
		err = stat(path, &buf);
		if (!S_ISDIR(buf.st_mode)) {
			err = mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			if (err) {
				spl_console_log("Mkdir err path: %s, err: %d\n", path, err);
				ret = SPL_LOG_CHECK_FILE_YEAR_ERROR;
				break;
			}
		}
#endif
		snprintf(year_month, 10, "%.4d\\%.2d", (int)lctime->year + YEAR_PADDING, (int)lctime->month + MONTH_PADDING);
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
void spl_sleep(unsigned int sec) {
#ifndef UNIX_LINUX
	Sleep( ((DWORD)(sec)) * 1000);
#else
	sleep(sec);
#endif 
}
/*===========================================================================================================================*/
int spl_standardize_path(char* fname) {
	int ret = 0;
	int i = 0;
	while (fname[i]) {
		if (fname[i] == '\\') {
			fname[i] = '/';
		}
		++i;
	}
	return ret;
}
/*===========================================================================================================================*/
int	
spl_stdz_topics
(char *buff, int *inoutlen, int *ntopics, char** pchar)
{
	int  ret = 0, i = 0;
	int count = 1;
	int n = 0;
	char *p = 0, *tmp = 0, *commas = 0;
	do {
		if(!inoutlen) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		if (!pchar) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		if(!buff) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}			
		if (!ntopics) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		n = *inoutlen;
		i = n - 1;
		while (i > -1)
		{
			if (buff[i] != ',') {
				break;
			}
			buff[i] = 0;
			--i;
		}
		n = (int)strlen(buff);
		if (n < 1) {
			ret = SPL_LOG_TOPIC_EMPTY;
			break;
		}
		spl_malloc(n + 1, p, char);
		if (!p) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		memcpy(p, buff, n);
		tmp = buff;
		while (1) {
			commas = strstr(tmp, ",");
			if (!commas) {
				break;
			}
			tmp = ++commas;
			count++;
		}
		(*pchar) = p;
		*ntopics = count;
		spl_console_log("Topic.\n");	

	} while(0);
	//if (p) {
	//	spl_free(p);
	//}
	return ret;
}
/*===========================================================================================================================*/
int
spl_gen_topics(SIMPLE_LOG_ST* t) {
	int ret = 0;
	char path[1024];
	//int renew = 0;
	LLU cszize = 0;
	do {
		int i = 0;
		if (t->n_topic < 1) {
			//ret = SPL_LOG_TOPIC_ZERO;
			break;
		}
		for (i = 0; i < t->n_topic; ++i) 
		{
			if (t->arr_topic[i].fp) {
				continue;
			}			
			do {
				int err = 0;
				snprintf(path, 1024, "%s-%s-%.7d.log", 
					t->path_template, t->arr_topic[i].topic, t->arr_topic[i].index);
				FFOPEN(t->arr_topic[i].fp, path, "a+");
				if (!t->arr_topic[i].fp) {
					ret = SPL_LOG_TOPIC_FOPEN;
					break;
				}
				FFSEEK(t->arr_topic[i].fp, 0, SEEK_END);
				cszize = (LLU)FFTELL(t->arr_topic[i].fp);
				if (cszize < t->file_limit_size) {
					t->arr_topic[i].fizize = (int)cszize;
					break;
				}
				t->arr_topic[i].fizize = (int)cszize;
				SPL_FCLOSE(t->arr_topic[i].fp, err);
				if (err) {
					ret = SPL_LOG_CLOSE_FILE_ERROR;
					break;
				}
				t->arr_topic[i].index++;
			} while (1);
		}
		if (ret) {
			break;
		}
		/*--------------*/
		if (ret) {
			break;
		}
		/*--------------*/
		if (t->renew > SPL_CHANGE_FILE_SIZE) {
			//TO-TEST
			for (i = 0; i < t->n_topic; ++i) {
				do {
					int err = 0;
					SPL_FCLOSE(t->arr_topic[i].fp, err);
					if (err) {
						ret = SPL_LOG_CLOSE_FILE_ERROR;
						break;
					}
					t->arr_topic[i].index = 0;
					t->arr_topic[i].fizize = 0;
					snprintf(path, 1024, "%s-%s-%.7d.log", t->path_template, t->arr_topic[i].topic, t->arr_topic[i].index);
					//t->arr_topic[i].fp = fopen(path, "a+");
					FFOPEN(t->arr_topic[i].fp, path, "a+");
					if (!t->arr_topic[i].fp) {
						ret = SPL_LOG_TOPIC_FOPEN;
						break;
					}
				} while (0);
				if (ret) {
					break;
				}
			}
			break;
		}
		for (i = 0; i < t->n_topic; ) {
			//TO-TEST
			if ((t->arr_topic[i].fizize) < t->file_limit_size) {
				++i;
				continue;
			}
			do {
				int err = 0;

				SPL_FCLOSE(t->arr_topic[i].fp, err);
				if (err) {
					ret = SPL_LOG_CLOSE_FILE_ERROR;
					break;
				}

				t->arr_topic[i].fizize = 0;
				
				snprintf(path, 1024, "%s-%s-%.7d.log", t->path_template, t->arr_topic[i].topic, t->arr_topic[i].index);
				//t->arr_topic[i].fp = fopen(path, "a+");
				FFOPEN(t->arr_topic[i].fp, path, "a+");
				if (!t->arr_topic[i].fp) {
					ret = SPL_LOG_TOPIC_FOPEN;
					break;
				}
				FFSEEK(t->arr_topic[i].fp, 0, SEEK_END);
				cszize = (LLU)FFTELL(t->arr_topic[i].fp);
				if (cszize < t->file_limit_size) {
					break;
				}
				SPL_FCLOSE(t->arr_topic[i].fp, err);
				if (err) {
					ret = SPL_LOG_CLOSE_FILE_ERROR;
					break;
				}
				t->arr_topic[i].index++;
			} while (1);
			++i;
			if (ret) {
				break;
			}
		}
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
char*
spl_get_buf_topic(int* n, int** ppl, int index) {
	SIMPLE_LOG_ST* tg = &__simple_log_static__;
	char* ret = 0;
	do {
		
		if (index < 0 || ((index + 1) > tg->n_topic)) {
			ret = spl_get_buf(n, ppl);
			break;
		}
		if (tg->arr_topic) {
			SIMPLE_LOG_TOPIC_ST* obj = &(tg->arr_topic[index]);
			if (n && ppl) {
				*n = (obj->buf->total > sizeof(generic_dta_st) + obj->buf->pl) ? (obj->buf->total - sizeof(generic_dta_st) - obj->buf->pl) : 0;
				ret = obj->buf->data;
				(*ppl) = &(obj->buf->pl);
			}
		}
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
LLU
spl_milli_now() {
	LLU ret = 0;
	time_t t0 = time(0);
	do {
#ifndef UNIX_LINUX
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		ret = t0 * 1000 + lt.wMilliseconds;
#else
		int err = 0;
		struct timespec nanosec;
		err = clock_gettime(CLOCK_REALTIME, &nanosec);
		if (err) {
			break;
		}
		ret = t0 * 1000 + (nanosec.tv_nsec / 1000000);
#endif	
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int spl_gen_topic_buff(SIMPLE_LOG_ST* t) {
	int ret = 0;
	char path[1024];
	int i = 0;
	//LLU cszize = 0;
	//spl_local_time_st lt, * plt = 0;;
	//char fmt_file_name[64];
	//int ferr = 0;
	//char yearmonth[16];
	char* buffer = 0;
	int total_buf_sz = 0;
	generic_dta_st* tmpBuff = 0;
	total_buf_sz = t->buff_size * (1 + t->n_topic);
	spl_malloc(total_buf_sz, buffer, char);
	do {
		if (!buffer) {
			ret = SPL_LOG_TOPIC_BUFF_MEM;
			break;
		}
		tmpBuff = (generic_dta_st*)buffer;
		tmpBuff->total = t->buff_size - SPL_MEMO_PADDING;
		t->buf = tmpBuff;

		if (!t->arr_topic) {
			char* p0 = t->topics;
			int sz = sizeof(SIMPLE_LOG_TOPIC_ST) * t->n_topic;
			spl_malloc(sz, t->arr_topic, SIMPLE_LOG_TOPIC_ST);
			memset(path, 0, sizeof(path));
			if (!t->arr_topic) {
				ret = SPL_LOG_TOPIC_MEMORY;
				break;
			}
			for (i = 0; i < t->n_topic; ++i) {
				char* p1 = 0;

				p1 = strstr(p0, ",");
				if (!p1) {
					snprintf(t->arr_topic[i].topic, SPL_TOPIC_SIZE, "%s", p0);
				}
				else {
					int n = p1 - p0;
					snprintf(t->arr_topic[i].topic, n + 1, "%s", p0);
					p1++;
					p0 = p1;
				}

				tmpBuff = (generic_dta_st*)(buffer + (( i + 1) * t->buff_size));
				tmpBuff->total = t->buff_size - SPL_MEMO_PADDING;
				t->arr_topic[i].buf = tmpBuff;
			}
			if (ret) {
				break;
			}
		}
		if (ret) {
			break;
		}
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
int
spl_fclose_err(int terr, void* ffp)
{
	int ret = 0;
	do {
#ifndef UNIX_LINUX
		spl_console_log("ffp: %p, terr: %d, GetLastError: 0x%x,", ffp, terr, (int)GetLastError());
#else
		char buf[64];
		/* https://linux.die.net/man/3/strerror_r , */
		/* - The strerror_r() function is similar to strerror(), but is thread safe */
		strerror_r(errno, buf, 64);
		spl_console_log("ffp: %p,terr: %d, errno: %d, strerror_r: %s", ffp, (int)errno, buf);
#endif
	} while(0);
	return ret;
}
/*===========================================================================================================================*/
int
spl_fflush_err(int terr, void* ffp) {
	int ret = 0;
	do {
#ifndef UNIX_LINUX
		spl_console_log("ffp: %p, terr: %d, GetLastError: 0x%x,", ffp, terr, (int)GetLastError());
#else
		char buf[64];
		/* https://linux.die.net/man/3/strerror_r , */
		/* - The strerror_r() function is similar to strerror(), but is thread safe */
		strerror_r(errno, buf, 64);
		spl_console_log("ffp: %p,terr: %d, errno: %d, strerror_r: %s", ffp, (int)errno, buf);
#endif
	} while (0);
	return ret;
}
/*===========================================================================================================================*/
/*===========================================================================================================================*/
#ifndef UNIX_LINUX
#else
#endif
/*===========================================================================================================================*/
/*===========================================================================================================================*/
/*===========================================================================================================================*/
/*===========================================================================================================================*/