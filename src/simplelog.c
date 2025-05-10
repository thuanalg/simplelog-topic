
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
 *		<2024-Sep-12>
 *		<2024-Dec-18>
 *		<2024-Dec-20>
 *		<2024-Dec-22>
 *		<2024-Dec-23>
 *		<2024-Dec-30>
 *		<2025-Jan-03>
 *		<2025-Jan-06>
 *		<2025-Jan-08>
 *		<2025-Feb-01>
 *		<2025-Apr-11>
 *		<2025-Apr-22>
 * Decription:
 *		The (only) main file to implement simple log.
 */
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#include "simplelog.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef UNIX_LINUX
#include <Windows.h>
#define YEAR_PADDING              0
#define MONTH_PADDING             0
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h> /* For mode constants */
#include <fcntl.h> /* For O_* constants */
#include <errno.h>
#ifdef __MACH__
#include <mach/mach.h>
#include <mach/clock.h>
#include <mach/clock_types.h>
#endif
#define YEAR_PADDING              1900
#define MONTH_PADDING             1

#define SPL_LOG_UNIX__SHARED_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define SPL_LOG_UNIX_CREATE_MODE  (O_CREAT | O_RDWR | O_EXCL)
#define SPL_LOG_UNIX_OPEN_MODE    (O_RDWR | O_EXCL)
#define SPL_LOG_UNIX_PROT_FLAGS   (PROT_READ | PROT_WRITE | PROT_EXEC)
#endif

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#define SPL_FCLOSE(__fp__, __n)                                                                                             \
	{                                                                                                                   \
		if (__fp__) {                                                                                               \
			(__n) = fclose((FILE *)(__fp__));                                                                   \
			if (__n) {                                                                                          \
				spl_fclose_err(__n, __fp__);                                                                \
			} else { /*spl_console_log("Close FILE 0x%p DONE.", (__fp__));*/                                    \
				;                                                                                           \
				(__fp__) = 0;                                                                               \
				;                                                                                           \
			}                                                                                                   \
		}                                                                                                           \
	}

#define SPL_FFLUSH(__fp__, __n)                                                                                             \
	{                                                                                                                   \
		if (__fp__) {                                                                                               \
			(__n) = fflush((FILE *)(__fp__));                                                                   \
			if (__n) {                                                                                          \
				spl_fflush_err(__n, __fp__);                                                                \
			}                                                                                                   \
		}                                                                                                           \
	}

#define FFOPEN(__fp, __path, __mode)                                                                                        \
	{                                                                                                                   \
		(__fp) = fopen((__path), (__mode));                                                                         \
		if (!(__fp))                                                                                                \
			spl_console_log("Open FILE error code: 0x%p, %s.\n", (__fp), (__fp) ? "DONE" : "FAILED");           \
	}

#define FFTELL(__fp) ftell((FILE *)(__fp))
#define FFSEEK(__fp, __a, __b) fseek((FILE *)(__fp), (__a), (__b))

#ifndef UNIX_LINUX
#define SPL_CloseHandle(__obj)                                                                                              \
	{                                                                                                                   \
		int bl = CloseHandle((__obj));                                                                              \
		if (!bl)                                                                                                    \
			spl_console_log("CloseHandle %s", bl ? "DONE" : "ERROR");                                           \
		(__obj) = 0;                                                                                                \
	}
#else
#define SPL_sem_wait(__obj) sem_wait((sem_t *)(__obj))
#define SPL_sem_post(__obj) sem_post((sem_t *)(__obj))
#define SPL_sem_destroy(__obj, __err)                                                                                       \
	{                                                                                                                   \
		(__err) = sem_destroy((sem_t *)(__obj));                                                                    \
		if ((__err))                                                                                                \
			spl_console_log("sem_destroy errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE")              \
	}
#define SPL_pthread_mutex_destroy(__obj, __err)                                                                             \
	{                                                                                                                   \
		(__err) = pthread_mutex_destroy((pthread_mutex_t *)(__obj));                                                \
		if ((__err))                                                                                                \
			spl_console_log("pthread_mutex_destroy errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE");   \
	}
#define SPL_pthread_mutex_lock(__obj, __err)                                                                                \
	{                                                                                                                   \
		(__err) = pthread_mutex_lock((pthread_mutex_t *)(__obj));                                                   \
		if ((__err))                                                                                                \
			spl_console_log("pthread_mutex_lock errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE");      \
	}
#define SPL_pthread_mutex_unlock(__obj, __err)                                                                              \
	{                                                                                                                   \
		(__err) = pthread_mutex_unlock((pthread_mutex_t *)(__obj));                                                 \
		if ((__err))                                                                                                \
			spl_console_log("pthread_mutex_unlock errcode: %d. %s\n", (__err), (__err) ? "FALIED" : "DONE");    \
	}
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#define SPL_SEM_NAME_RW           "_SEM_RW"
#define SPL_SEM_NAME_OFF          "_SEM_OFF"

#define SPL_MTX_NAME_RW           "_MTX_RW"
#define SPL_MTX_NAME_OFF          "_MTX_OFF"
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#define SPL_LOG_PATHFOLDR         "pathfoder="
#define SPL_LOG_LEVEL             "level="
#define SPL_LOG_BUFF_SIZE         "buffsize="
#define SPL_MAX_SZ_MSG            "max_sz_msg="
#define SPL_LOG_ROT_SIZE          "rotation_size="
#define SPL_LOG_TOPIC             "topic="
#define SPL_LOG_NCPU              "ncpu="
#define SPL_LOG_TRIGGER           "trigger="
#define SPL_LOG_SHARED_KEY        "shared_key="
#define SPL_LOG_END_CFG           "end_configuring="

#define SPL_FILE_NAME_FMT         "%s\\%s\\%s_%.8d.log"
#define SPL_FILE_NAME_FMT_TOPIC   "%s\\%s\\%s"
#define SPL_FMT_DATE_ADDING       "%.4d-%.2d-%.2d"
#define SPL_FMT_HOUR_ADDING       "%.2d:%.2d:%.2d"
#define SPL_FMT_DELT_ADDING       "%s %s.%.9u"
#define SPL_FMT_MILL_ADDING       "%s %s.%.9d"
#define SPL_FMT_DATE_ADDING_X     "\n[%.4d-%.2d-%.2d %.2d:%.2d:%.2d.%.9d] "
#define HHHHHHHHHHH               "%llu]\t"

#define SPL_TEXT_UNKNOWN          "U"
#define SPL_TEXT_DEBUG            "D"
#define SPL_TEXT_INFO             "I"
#define SPL_TEXT_WARN             "W"
#define SPL_TEXT_ERROR            "E"
#define SPL_TEXT_FATAL            "F"

#define SPL_CASTGEN(__t__) ((spl_gen_data_st *)__t__)
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef UNIX_LINUX
// DLL_API_SIMPLE_LOG
static void
splLockSpinlock(volatile long *p);
// DLL_API_SIMPLE_LOG
static void
splUnlockSpinlock(volatile long *p);
#define SplLockSpinlock(__p__) splLockSpinlock((volatile long *)(__p__))
#define SplUnlockSpinlock(__p__) splUnlockSpinlock((volatile long *)(__p__))

/*static
	volatile long spl_rw_spin = 0;*/
#define THREAD_ROUTINE            LPTHREAD_START_ROUTINE
#else
/*pthread_spinlock_t	spl_rw_spin; */
typedef void *(*THREAD_ROUTINE)(void *);
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

typedef enum __CHANGE_NAME_E__ {
	SPL_NO_CHANGE_NAME = 0,
	SPL_CHANGE_FILE_SIZE,
	SPL_CHANGE_DAY,
	SPL_CHANGE_MONTH,
	SPL_CHANGE_YEAR,

	SPL_CHANGE_FINISH
} __CHANGE_NAME_E__;

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
static const char *__splog_pathfolder[] = {SPL_LOG_PATHFOLDR, SPL_LOG_LEVEL, SPL_LOG_BUFF_SIZE, SPL_MAX_SZ_MSG,
    SPL_LOG_ROT_SIZE, SPL_LOG_TOPIC, SPL_LOG_NCPU, SPL_LOG_TRIGGER, SPL_LOG_SHARED_KEY, SPL_LOG_END_CFG, 0};

static SIMPLE_LOG_ST __simple_log_static__;
;
void
spl_err_txt_init();
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

static int
spl_init_log_parse(char *buff, char *key, char *);

static int
spl_verify_folder(char *folder);
static int
spl_simple_log_thread(SIMPLE_LOG_ST *t);
static int
spl_gen_file(SIMPLE_LOG_ST *t, int *n, int limit, int *);
static int
spl_gen_topics(SIMPLE_LOG_ST *t);
static int
spl_get_fname_now(char *name);
static int
spl_get_fname_now(char *name);
static int
spl_folder_sup(char *folder, spl_local_time_st *lctime, char *year_month);
static int
spl_local_time_now(spl_local_time_st *st_time);
static int
spl_stdz_topics(char *buff, int *inoutlen, int *, char **);

#ifndef UNIX_LINUX
static DWORD WINAPI
spl_written_thread_routine(LPVOID lpParam);
#else
static void *
spl_written_thread_routine(void *);
#endif

static int
spl_fclose_err(int t, void *fpp);
static int
spl_fflush_err(int t, void *fpp);

#ifndef UNIX_LINUX
static int
spl_create_thread(THREAD_ROUTINE f, void *arg, HANDLE *outhd);
#else
static int
spl_create_thread(THREAD_ROUTINE f, void *arg, pthread_t *outid);
#endif

#ifndef UNIX_LINUX
static int
spl_win32_sync_create();
static DWORD WINAPI
spl_trigger_routine(void *arg);
#else
#ifdef __MACH__
static int
spl_osx_sync_create();
static int
spl_osx_sync_del();
#endif
static int
spl_mtx_init(void *mtx, char shared);
static void *
spl_trigger_routine(void *arg);
#endif

static int
spl_create_memory(void **output, char *shared_key, int size_shared, char isCreating);
static int
spl_del_memory();
static int
spl_calculate_size();
static int
spl_init_segments();
static int
spl_allocate_topics();
static int
spl_gen_sync_tool();
static int
spl_clean_sync_tool();
static int
spl_set_off(int);
static int
spl_standardize_path(char *fname);
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
SIMPLE_LOG_ST *
spl_control_obj()
{
	return (SIMPLE_LOG_ST *)&__simple_log_static__;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

static const char spl_text_gb_c[SPL_LOG_PEAK] = {
    'A',
    'D',
    'I',
    'W',
    'E',
    'F',
};

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_local_time_now(spl_local_time_st *stt)
{
	int ret = 0;
#ifndef UNIX_LINUX
	SYSTEMTIME lt;
#else
	struct tm *lt, rlt;
	struct timespec nanosec;
#ifdef __MACH__
	clock_serv_t cclock;
	mach_timespec_t mts;
	kern_return_t result;
#endif
#endif
	do {
		if (!stt) {
			ret = SPL_LOG_ST_NAME_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		LARGE_INTEGER counter;
		GetLocalTime(&lt);
		QueryPerformanceCounter(&counter);
		stt->year = (unsigned int)lt.wYear;
		stt->month = (unsigned char)lt.wMonth;
		stt->day = (unsigned char)lt.wDay;

		stt->hour = (unsigned char)lt.wHour;
		stt->minute = (unsigned char)lt.wMinute;
		stt->sec = (unsigned char)lt.wSecond;
		stt->nn = (unsigned int)lt.wMilliseconds * SPL_MILLION + counter.QuadPart % SPL_MILLION;
#else
		time_t t = time(0);
		lt = localtime_r(&t, &rlt);
		if (!lt) {
			ret = SPL_LOG_TIME_NULL_ERROR;
			break;
		}
		lt = (struct tm *)&rlt;
		/* No need freeing,
		 * https://stackoverflow.com/questions/35031647/do-i-need-to-free-the-returned-pointer-from-localtime-function
		 */
#ifdef __MACH__
		result = host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &cclock);
		if (result != KERN_SUCCESS) {
			ret = SPL_LOG_MACH_CLOCK_SERVICE_ERROR;
			spl_console_log("SPL_LOG_MACH_CLOCK_SERVICE_ERROR.");
			break;
		}
		result = clock_get_time(cclock, &mts);
		if (result != KERN_SUCCESS) {
			ret = SPL_LOG_MACH_GETTIME_ERROR;
			spl_console_log("SPL_LOG_MACH_GETTIME_ERROR.");
			break;
		}
		mach_port_deallocate(mach_task_self(), cclock);
		nanosec.tv_sec = mts.tv_sec;
		nanosec.tv_nsec = mts.tv_nsec;
#else
		/* https://linux.die.net/man/3/localtime */
		/* https://linux.die.net/man/3/clock_gettime */
		ret = clock_gettime(CLOCK_REALTIME, &nanosec);
		if (ret) {
			ret = SPL_LOG_TIME_NANO_NULL_ERROR;
			break;
		}

#endif
		stt->year = lt->tm_year;
		stt->month = lt->tm_mon;
		stt->day = lt->tm_mday;

		stt->hour = lt->tm_hour;
		stt->minute = (spl_uchar)lt->tm_min;
		stt->sec = lt->tm_sec;
		stt->nn = (spl_uint)nanosec.tv_nsec;

#endif
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_set_off(int isoff)
{
	int ret = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	spl_mutex_lock(t->mtx_rw);
	do {
		t->off = isoff;
	} while (0);
	spl_mutex_unlock(t->mtx_rw);

	if (isoff) {
		int errCode = 0;
		spl_rel_sem(t->sem_rwfile);
		if (t->isProcessMode) {
			if (!t->is_master) {
				spl_rel_sem(t->sem_off);
			}
		}
#ifndef UNIX_LINUX
		errCode = (int)WaitForSingleObject(t->sem_off, INFINITE);
		if (errCode == WAIT_FAILED) {
			spl_console_log("------- WaitForSingleObject errCode: %d\n", (int)GetLastError());
		}
#else
		errCode = SPL_sem_wait(t->sem_off);
		if (errCode) {
			spl_console_log("------- SPL_sem_wait errCode: %d\n", (int)errCode);
		}
#endif
#ifdef SPL_SHOW_CONSOLE
		spl_console_log("------- errCode: %d\n", (int)errCode);
#endif
	}
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_init_log_parse(char *buff, char *key, char *isEnd)
{
	int ret = SPL_NO_ERROR;
	SIMPLE_LOG_ST *t = (SIMPLE_LOG_ST *)&__simple_log_static__;
	do {
		if (strcmp(key, SPL_LOG_PATHFOLDR) == 0) {
			if (!buff[0]) {
				ret = SPL_INIT_PATH_FOLDER_EMPTY_ERROR;
				break;
			}
			snprintf(t->folder, SPL_PATH_FOLDER, "%s", buff);
			break;
		}
		if (strcmp(key, SPL_LOG_LEVEL) == 0) {
			int n = 0;
			int count = 0;
			count = sscanf(buff, "%d", &n);
			if (n < 0 || count < 1) {
				ret = SPL_LOG_LEVEL_ERROR;
				break;
			}
			/*spl_set_log_levwel(n);*/
			t->llevel = n;
			break;
		}
		if (strcmp(key, SPL_LOG_BUFF_SIZE) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPL_LOG_BUFF_SIZE_ERROR;
				break;
			}
			t->buff_size = n;
			break;
		}
		if (strcmp(key, SPL_MAX_SZ_MSG) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPL_LOG_MAX_SZ_MSG_ERROR;
				break;
			}
			if (n < SPL_MEMO_PADDING) {
				n = SPL_MEMO_PADDING;
			}
			t->max_sz_msg = n;
			break;
		}
		if (strcmp(key, SPL_LOG_ROT_SIZE) == 0) {
			int n = 0;
			int sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (n < 1 || sz < 1) {
				ret = SPL_LOG_ROT_SIZE_ERROR;
				break;
			}
			t->file_limit_size = n;
#ifdef SPL_SHOW_CONSOLE
			spl_console_log(
			    "t->file_limit_size: %d.\n", t->file_limit_size);
#endif
			break;
		}
		if (strcmp(key, SPL_LOG_TOPIC) == 0) {
			int n = 0, count = 0;
			char *p = 0;
			n = (int)strlen(buff);
			if (n < 1) {
				break;
			}
			ret = spl_stdz_topics(buff, &n, &count, &p);
			if (ret) {
				break;
			}
			t->n_topic = count;
			t->topics = p;
			break;
		}
		if (strcmp(key, SPL_LOG_NCPU) == 0) {
			int sz = 0;
			int n = 0;
			sz = sscanf(buff, "%d", &n);
			if (sz < 1) {
				t->ncpu = 1;
				break;
			}
			t->ncpu = n;
			break;
		}
		if (strcmp(key, SPL_LOG_TRIGGER) == 0) {
			int n = 0, sz = 0;
			sz = sscanf(buff, "%d", &n);
			if (sz < 1) {
				t->trigger_thread = 0;
				break;
			}
			t->trigger_thread = n;
			break;
		}
		if (strcmp(key, SPL_LOG_SHARED_KEY) == 0) {
			int n = 0;
			n = (int)strlen(buff);
			if (n < 1) {
				break;
			}
			snprintf(t->shared_key, SPL_SHARED_KEY_LEN, "%s", buff);
		#if SPL_SHOW_CONSOLE
			spl_console_log("t->shared_key: %s.", t->shared_key);
		#endif
			break;
		}
		if (strcmp(key, SPL_LOG_END_CFG) == 0) {
#ifdef SPL_SHOW_CONSOLE
			spl_console_log("End configuration.\n");
#endif
			if (isEnd) {
				*isEnd = 1;
			}
			break;
		}
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int
spl_init_log_ext(SPL_INPUT_ARG *input)
{
	int ret = 0;
	spl_err_txt_init();
	do {
		memcpy(__simple_log_static__.id_name, input->id_name, SPL_IDD_NAME);
		ret = spl_init_log(input->folder);
		;
		if (ret) {
			spl_console_log("Cannot initiate log.");
			break;
		}
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int
spl_init_log(char *pathcfg)
{
	int ret = 0;
	FILE *fp = 0;
	char c = 0;
	int count = 0;
	char buf[1024];
	/*void* obj = 0;*/
	char isEnd = 0;
	__simple_log_static__.ncpu = 1;
	do {
		memset(buf, 0, sizeof(buf));
		/*
		// fp = fopen(pathcfg, "r");
		*/
		FFOPEN(fp, pathcfg, "r");
		if (!fp) {
			ret = SPL_LOG_OPEN_CFG;
			spl_console_log("Cannot open file error.");
			break;
		}
		while (1) {
			c = fgetc(fp);
			if (c == '\r' || c == '\n' || c == EOF) {
				int j = 0;
				char *node = 0;
				if (count < 1) {
					continue;
				}
				while (1) {
					char *pp = 0;
					node = (char *)__splog_pathfolder[j];
					if (!node) {
						break;
					}
					pp = strstr(buf, node);
					if (!pp) {
						++j;
						continue;
					}
					if (pp != buf) {
						++j;
						continue;
					}
					/*// if (pp)*/
					{
						char *p = 0;
						size_t k = 0;
						k = strlen(node);
						p = (buf + k);
#ifdef SPL_SHOW_CONSOLE
						spl_console_log("Find out the keyword: [%s] value [%s].", node, p);
#endif
						ret = spl_init_log_parse(p, node, &isEnd);
						break;
					}
					/*
			j++;
		     */
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

		ret = spl_verify_folder(__simple_log_static__.folder);
		if (ret) {
			break;
		}
		ret = spl_gen_sync_tool();
		if (ret) {
			break;
		}
		ret = spl_simple_log_thread(&__simple_log_static__);
		if (ret) {
			break;
		}
	} while (0);

	if (fp) {
		SPL_FCLOSE(fp, ret);
	}

	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_mutex_lock(void *obj)
{
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD err = 0;
#else
#endif
	do {
		if (!obj) {
			ret = SPL_LOG_MUTEX_NULL_ERROR;
			spl_console_log("Mutex is NULL.");
			break;
		}
#ifndef UNIX_LINUX
#ifndef SPL_USING_SPIN_LOCK
		err = WaitForSingleObject(obj, INFINITE);
		if (err != WAIT_OBJECT_0) {
			ret = SPL_LOG_WIN32_WAIT_OBJECT;
			break;
		}
#else
		SplLockSpinlock(obj);
#endif
#else
#ifndef SPL_USING_SPIN_LOCK
		SPL_pthread_mutex_lock((pthread_mutex_t *)obj, ret);
#else
		pthread_spin_lock((pthread_spinlock_t *)obj);
#endif

#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_mutex_unlock(void *obj)
{
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD done = 0;
#else
#endif
	do {
		if (!obj) {
			ret = SPL_LOG_MUTEX_NULL_ERROR;
			spl_console_log("Mutex is NULL.");
			break;
		}
#ifndef UNIX_LINUX
#ifndef SPL_USING_SPIN_LOCK
		done = ReleaseMutex(obj);
		if (!done) {
			ret = SPL_LOG_WIN32_MUTEX_RELEASE;
			break;
		}
#else
		SplUnlockSpinlock(obj);
#endif
#else
#ifndef SPL_USING_SPIN_LOCK
		SPL_pthread_mutex_unlock((pthread_mutex_t *)obj, ret);
#else
		pthread_spin_unlock((pthread_spinlock_t *)obj);
#endif
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_verify_folder(char *folder)
{
	int ret = 0;
	do {
#ifdef WIN32
		/*
	    https://learn.microsoft.com/en-us/windows/win32/fileio/retrieving-and-changing-file-attributes
	    https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya
	    ERROR_ALREADY_EXISTS, ERROR_PATH_NOT_FOUND
	 */
		BOOL result = CreateDirectoryA(folder, NULL);
		if (!result) {
			DWORD werr = GetLastError();
			if (werr == ERROR_ALREADY_EXISTS) {
				break;
			}
			ret = SPL_LOG_FOLDER_ERROR;
		}
#endif
	} while (0);
	return ret;
}
/*https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemtime*/
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_get_fname_now(char *name)
{
	int ret = 0;
	spl_local_time_st lt;
	spl_local_time_now(&lt);
	if (name) {
		if (__simple_log_static__.id_name[0]) {
			snprintf(name, SPL_FNAME_LEN, "%.4d-%.2d-%.2d-%s", lt.year + YEAR_PADDING,
			    (int)lt.month + MONTH_PADDING, (int)lt.day, (char *)__simple_log_static__.id_name);
		} else {
			snprintf(name, SPL_FNAME_LEN, "%.4d-%.2d-%.2d-simplelog", lt.year + YEAR_PADDING,
			    (int)lt.month + MONTH_PADDING, (int)lt.day);
		}
	}
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef UNIX_LINUX
DWORD WINAPI
spl_written_thread_routine(LPVOID lpParam)
#else
void *
spl_written_thread_routine(void *lpParam)
#endif
{
	int k = 0;
	SIMPLE_LOG_ST *t = (SIMPLE_LOG_ST *)lpParam;
	int ret = 0, sz = 0, err = 0;

	register char is_off = 0;
	register int i = 0, j = 0;

	char **main_src_thrd_buf = 0;
	char ***src_topic_thrd_buf = 0;

#ifndef UNIX_LINUX
	HANDLE trigger_handle_id = 0;
#else
	pthread_t trigger_handle_id = 0;
#endif

	char *only_buf = 0;
	spl_gen_data_st *only_cast = 0;
	spl_malloc((t->buff_size * t->ncpu), only_buf, char);
	/*
	//spl_malloc((t->buff_size * 3), only_buf, char);
	//spl_create_memory((void**)&only_buf, "thread_buff_123", (t->buff_size * t->ncpu), 1);
	*/
	only_cast = SPL_CASTGEN(only_buf);
	only_cast->total = (t->buff_size * t->ncpu);
	only_cast->range = only_cast->total - sizeof(spl_gen_data_st);
	only_cast->pl = only_cast->pc = 0;

	spl_malloc(t->ncpu * sizeof(char *), main_src_thrd_buf, char *);
	for (i = 0; i < t->ncpu; ++i) {
		char *p = (char *)t->buf;
		main_src_thrd_buf[i] = p + t->buff_size * i;
	}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

	if (t->arr_topic) {
		spl_malloc(t->n_topic * sizeof(char *), src_topic_thrd_buf, char **);
		for (i = 0; i < t->n_topic; ++i) {
			char *p = (char *)t->arr_topic[i].buf;
			spl_malloc(t->ncpu * sizeof(char *), src_topic_thrd_buf[i], char *);
			for (j = 0; j < t->ncpu; ++j) {
				src_topic_thrd_buf[i][j] = p + t->buff_size * j;
			}
		}
	}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
	if (t->trigger_thread > 0) {
		spl_create_thread(spl_trigger_routine, t, &trigger_handle_id);
	}
	do {
		if (is_off) {
			break;
		}
		if (!t) {
			exit(1);
		}
		if (!t->sem_rwfile) {
			exit(1);
		}
		/*
		//spl_console_log("Semaphore: 0x%p.\n", t->sem_rwfile);
		*/
		if (!t->mtx_rw) {
			exit(1);
		}
		while (1) {
			if (is_off) {
				break;
			}
#ifndef UNIX_LINUX
			WaitForSingleObject(t->sem_rwfile, INFINITE);
#else
			SPL_sem_wait(t->sem_rwfile);
#endif
			do {
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

				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
				if (!is_off) {
					spl_mutex_lock(t->mtx_rw);
					is_off = t->off;
					spl_mutex_unlock(t->mtx_rw);
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
				for (i = 0; i < t->ncpu; ++i) {
					spl_mutex_lock(t->arr_mtx[i]);
					/* //do { */
					if (SPL_CASTGEN(main_src_thrd_buf[i])->pl > 0) {
						memcpy(only_cast->data + only_cast->pl,
						    SPL_CASTGEN(main_src_thrd_buf[i])->data,
						    SPL_CASTGEN(main_src_thrd_buf[i])->pl);
						only_cast->pl += SPL_CASTGEN(main_src_thrd_buf[i])->pl;
						SPL_CASTGEN(main_src_thrd_buf[i])->pl = 0;
					}
					/* //} while (0); */
					spl_mutex_unlock(t->arr_mtx[i]);
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
				if (only_cast->pl > 0) {
					k = (int)fwrite(only_cast->data, 1, only_cast->pl, t->fp);
					only_cast->pl = 0;
					sz += k;
					SPL_FFLUSH((t->fp), err);
				}
				if (err) {
					ret = SPL_LOG_TOPIC_FLUSH;
					spl_console_log("--fflush, ret: %d --\n", err);
					break;
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
				if (t->n_topic > 0) {
					char *src = 0;
					for (i = 0; i < t->n_topic; ++i) {
						for (j = 0; j < t->ncpu; ++j) {
							src = src_topic_thrd_buf[i][j];
							spl_mutex_lock(t->arr_mtx[j]);
							/*//do */
							if (SPL_CASTGEN(src)->pl > 0) {
								memcpy(only_cast->data + only_cast->pl,
								    SPL_CASTGEN(src)->data, SPL_CASTGEN(src)->pl);
								only_cast->pl += SPL_CASTGEN(src)->pl;
								SPL_CASTGEN(src)->pl = 0;
							}
							/*//} while (0);*/
							spl_mutex_unlock(t->arr_mtx[j]);
						}
						if (only_cast->pl > 0) {
							k = (int)fwrite(
							    only_cast->data, 1, only_cast->pl, (FILE *)(t->arr_topic[i].fp));
							t->arr_topic[i].fizize += k;
							only_cast->pl = 0;
							SPL_FFLUSH((t->arr_topic[i].fp), err);
							if (err) {
								spl_console_log("--fflush, ret: %d --\n", err);
								ret = SPL_LOG_TOPIC_FLUSH;
								break;
							}
						}
					}
				}
				/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
				if (is_off) {
					break;
				}
			} while (0);
		}
		if (t->fp) {
			int werr = 0;
			SPL_FCLOSE(t->fp, werr);
			for (i = 0; i < t->n_topic; ++i) {
				if (t->arr_topic[i].fp) {
					SPL_FCLOSE(t->arr_topic[i].fp, werr);
				}
			}
			spl_mutex_lock(t->mtx_rw);
			/*
			if (t->buf) {
				spl_free(t->buf);
			}
			*/
			for (i = 0; i < t->n_topic; ++i) {
				if (t->arr_topic[i].buf) {
					t->arr_topic[i].buf = 0;
				}
			}
			spl_mutex_unlock(t->mtx_rw);
		}

	} while (0);
	/* spl_free(thrd_buffer); */
	spl_free(main_src_thrd_buf);
	if (t->arr_topic) {
		for (i = 0; i < t->n_topic; ++i) {
			spl_free(src_topic_thrd_buf[i]);
		}
		spl_free(src_topic_thrd_buf);
	}
	spl_free(only_buf);
	/* spl_del_memory((void *) only_buf); */
	/* Send a signal to the waiting thread. */
	if (t->trigger_thread) {
#ifndef UNIX_LINUX
		if (trigger_handle_id) {
			int rs = (int)TerminateThread(trigger_handle_id, 0);
			if (!rs) {
				spl_console_log("TerminateThread error.");
			} else {
				spl_console_log("TerminateThread OK.");
			}
		}
#else
		if (trigger_handle_id) {
			int rs = pthread_cancel(trigger_handle_id);
			if (rs) {
				spl_console_log("pthread_cancel error.");
			} else {
				spl_console_log("pthread_cancel OK.");
			}
		}
#endif
	}
	spl_rel_sem(__simple_log_static__.sem_rwfile);
	if (!t->isProcessMode) {
		spl_rel_sem(__simple_log_static__.sem_off);
	} else {
		if (t->is_master) {
			spl_rel_sem(__simple_log_static__.sem_off);
		}
	}
	return 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_simple_log_thread(SIMPLE_LOG_ST *t)
{
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
		err = pthread_create(&idd, 0, spl_written_thread_routine, t);
		if (err) {
			ret = SPL_LOG_CREATE_THREAD_ERROR;
			break;
		}
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
char *
spl_fmt_now_ext(
    char *fmtt, int len, int lv, const char *filename, const char *funcname, int line, unsigned short *r, int *outlen)
{
	char *p = fmtt;
	int ret = 0;
	spl_local_time_st stt;
	int n = 0;
	LLU threadiid = 0;

	threadiid = (LLU)spl_get_threadid();
	*outlen = 0;
	ret = spl_local_time_now(&stt);
	if (ret) {
		return p;
	}
#ifndef __MODE_STRAIGHT__
	*r = (stt.nn % __simple_log_static__.ncpu);
#else
	*r = (threadiid % __simple_log_static__.ncpu);
#endif
	n = sprintf(fmtt, SPL_FMT_DATE_ADDING_X "[%c] [tid\t%llu]\t", stt.year + YEAR_PADDING, stt.month + MONTH_PADDING,
	    stt.day, stt.hour, stt.minute, stt.sec, (int)stt.nn, spl_text_gb_c[lv % SPL_LOG_PEAK], threadiid);
	if (n < 1) {
		ret = SPL_LOG_PRINTF_ERROR;
		return p;
	}
	*outlen = n;

	/*
	*outlen += snprintf(fmtt + n, len - n, "[%s:%s:%d] [r: %d]\t",
		filename, funcname, line, (int)*r); */
	*outlen += snprintf(fmtt + n, SPL_RL_BUF - n, "[%s:%s:%d] ", filename, funcname, line);
	if (*outlen > len) {
		spl_malloc((*outlen + 1), p, char);
		if (!p) {
			exit(1);
		}
		memcpy(p, fmtt, n);
		*outlen = n;
		/*
			*outlen += sprintf(p + n, "[%s:%s:%d] [r: %d]\t",
			filename, funcname, line, (int)*r);
		*/
		*outlen += snprintf(fmtt + n, SPL_RL_BUF - n, "[%s:%s:%d] ", filename, funcname, line);
	}

	return p;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_fmmt_now(char *fmtt, int len)
{
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
		n = snprintf(fmtt, len, SPL_FMT_MILL_ADDING, buff, buff1, (int)stt.nn);

	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int
spl_gen_file(SIMPLE_LOG_ST *t, int *sz, int limit, int *index)
{
	int ret = 0;
	spl_local_time_st lt, *plt = 0;
	;
	/* int renew = SPL_NO_CHANGE_NAME; */
	char path[SPL_FULLPATH_LEN + 1];
	char fmt_file_name[SPL_FNAME_LEN];
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
				snprintf(
				    path, SPL_FULLPATH_LEN, SPL_FILE_NAME_FMT, t->folder, yearmonth, fmt_file_name, *index);
				snprintf(t->path_template, SPL_TEMPLATE_LEN, SPL_FILE_NAME_FMT_TOPIC, t->folder, yearmonth,
				    fmt_file_name);
				spl_standardize_path(path);
				spl_standardize_path(t->path_template);

				FFOPEN(t->fp, path, "a+");
				if (!t->fp) {
					ret = SPL_LOG_OPEN_FILE_ERROR;
					break;
				}
				FFSEEK(t->fp, 0, SEEK_END);
				cszize = (int)FFTELL(t->fp);
				if (cszize < limit) {
					*sz = cszize;
					break;
				}
				SPL_FCLOSE(t->fp, err);
				if (err) {
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
		snprintf(path, SPL_FULLPATH_LEN, SPL_FILE_NAME_FMT, t->folder, yearmonth, fmt_file_name, *index);
		snprintf(t->path_template, SPL_TEMPLATE_LEN, SPL_FILE_NAME_FMT_TOPIC, t->folder, yearmonth, fmt_file_name);

		SPL_FCLOSE(t->fp, ferr);
		if (ferr) {
			ret = SPL_LOG_CLOSE_FILE_ERROR;
			break;
		}
		spl_standardize_path(path);
		spl_standardize_path(t->path_template);
		/* t->fp = fopen(path, "a+"); */
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

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
LLU
spl_get_threadid()
{
#ifndef UNIX_LINUX
	return (LLU)GetCurrentThreadId();
#else
	return (LLU)pthread_self();
#endif
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_rel_sem(void *sem)
{
	int ret = 0;
#ifndef UNIX_LINUX
#else
#endif
	do {
		if (!sem) {
			ret = SPL_LOG_SEM_NULL_ERROR;
			break;
		}
#ifndef UNIX_LINUX
		ReleaseSemaphore(sem, 1, 0);
#else
#ifdef __MACH__
		SPL_sem_post(sem);
#else
		int val = 0;
		int err = sem_getvalue((sem_t *)sem, &val);
		if (!err) {
			if (val < 1) {
				SPL_sem_post(sem);
			}
		}
#endif
#endif
	} while (0);
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_finish_log()
{
	int ret = 0;
	ret = spl_set_off(1);
	if (ret) {
		spl_console_log("spl_set_off ret: %d", ret);
	}
	ret = spl_clean_sync_tool();
	if (ret) {
		spl_console_log("spl_clean_sync_tool ret: %d", ret);
	}
	memset(&__simple_log_static__, 0, sizeof(__simple_log_static__));
	return ret;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/*https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createdirectorya*/
int
spl_folder_sup(char *folder, spl_local_time_st *lctime, char *year_month)
{
	int ret = 0;
	char path[SPL_FULLPATH_LEN + 1];
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
		snprintf(path, SPL_FULLPATH_LEN, "%s", folder);
#ifndef UNIX_LINUX
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPL_LOG_CHECK_FOLDER_ERROR;
				break;
			}
		}
		snprintf(path, SPL_FULLPATH_LEN, "%s/%.4u", folder, lctime->year + YEAR_PADDING);
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPL_LOG_CHECK_FOLDER_YEAR_ERROR;
				break;
			}
		}
		snprintf(path, SPL_FULLPATH_LEN, "%s/%.4d/%.2d", folder, (int)lctime->year + YEAR_PADDING,
		    (int)lctime->month + MONTH_PADDING);
		result = CreateDirectoryA(path, 0);
		if (!result) {
			DWORD xerr = GetLastError();
			if (xerr != ERROR_ALREADY_EXISTS) {
				ret = SPL_LOG_CHECK_FILE_YEAR_ERROR;
				break;
			}
		}
#else
		/*
			https://linux.die.net/man/3/mkdir
			https://linux.die.net/man/2/stat
		*/
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
		snprintf(path, SPL_FULLPATH_LEN, "%s/%.4u", folder, lctime->year + YEAR_PADDING);
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
		snprintf(path, SPL_FULLPATH_LEN, "%s/%.4d/%.2d", folder, (int)lctime->year + YEAR_PADDING,
		    (int)lctime->month + MONTH_PADDING);
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
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
void
spl_sleep(unsigned int sec)
{
#ifndef UNIX_LINUX
	Sleep(((DWORD)(sec)) * 1000);
#else
	sleep(sec);
#endif
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
void
spl_milli_sleep(unsigned int mill_sec)
{
#ifndef UNIX_LINUX
	Sleep(((DWORD)(mill_sec)));
#else
	usleep(mill_sec * 1000);
#endif
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_standardize_path(char *fname)
{
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
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_stdz_topics(char *buff, int *inoutlen, int *ntopics, char **pchar)
{
	int ret = 0, i = 0;
	int count = 1;
	int n = 0;
	char *p = 0, *tmp = 0, *commas = 0;
	do {
		if (!inoutlen) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		if (!pchar) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		if (!buff) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		if (!ntopics) {
			ret = SPL_LOG_TOPIC_NULL;
			break;
		}
		n = *inoutlen;
		i = n - 1;
		while (i > -1) {
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

	} while (0);
	/*
	//if (p) {
	//	spl_free(p);
	//}
	*/
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_gen_topics(SIMPLE_LOG_ST *t)
{
	int ret = 0;
	char path[SPL_FULLPATH_LEN + 1];
	/*
	//int renew = 0;
	*/
	LLU cszize = 0;
	do {
		int i = 0;
		if (t->n_topic < 1) {
			/*
			//ret = SPL_LOG_TOPIC_ZERO;
			*/
			break;
		}
		for (i = 0; i < t->n_topic; ++i) {
			if (t->arr_topic[i].fp) {
				continue;
			}
			do {
				int err = 0;
				snprintf(path, SPL_FULLPATH_LEN, "%s-%s-%.7d.log", t->path_template, t->arr_topic[i].topic,
				    t->arr_topic[i].index);
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
					snprintf(path, SPL_FULLPATH_LEN, "%s-%s-%.7d.log", t->path_template,
					    t->arr_topic[i].topic, t->arr_topic[i].index);
					/*
					//t->arr_topic[i].fp = fopen(path, "a+");
					*/
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
		for (i = 0; i < t->n_topic;) {
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

				snprintf(path, SPL_FULLPATH_LEN, "%s-%s-%.7d.log", t->path_template, t->arr_topic[i].topic,
				    t->arr_topic[i].index);
				/*
				//t->arr_topic[i].fp = fopen(path, "a+");
				*/
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

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
LLU
spl_milli_now()
{
	LLU ret = 0;
	time_t t0 = time(0);
	do {
#ifndef UNIX_LINUX
		SYSTEMTIME lt;
		GetLocalTime(&lt);
		ret = t0 * 1000 + lt.wMilliseconds;
#else
		struct timespec nanosec;
#ifdef __MACH__
		kern_return_t result;
		clock_serv_t cclock;
		mach_timespec_t mts;
		result = host_get_clock_service(mach_host_self(), REALTIME_CLOCK, &cclock);
		if (result != KERN_SUCCESS) {
			break;
		}
		result = clock_get_time(cclock, &mts);
		if (result != KERN_SUCCESS) {
			break;
		}
		mach_port_deallocate(mach_task_self(), cclock);
		nanosec.tv_sec = mts.tv_sec;
		nanosec.tv_nsec = mts.tv_nsec;
#else
		int err = 0;
		err = clock_gettime(CLOCK_REALTIME, &nanosec);
		if (err) {
			break;
		}
#endif
		ret = t0 * 1000 + (nanosec.tv_nsec / SPL_MILLION);
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_fclose_err(int terr, void *ffp)
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
		spl_console_log("ffp: %p,terr: %d, errno: %d, strerror_r: %s", ffp, terr, (int)errno, buf);
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_fflush_err(int terr, void *ffp)
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
		spl_console_log("ffp: %p,terr: %d, errno: %d, strerror_r: %s", ffp, terr, (int)errno, buf);
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#ifndef UNIX_LINUX
void
splLockSpinlock(volatile long *p)
{
	while (InterlockedCompareExchange(p, 1, 0) != 0) {
	}
}

void
splUnlockSpinlock(volatile long *p)
{
	InterlockedExchange(p, 0);
}
#else
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef UNIX_LINUX
DWORD WINAPI
spl_trigger_routine(void *arg)
#else
void *
spl_trigger_routine(void *arg)
#endif
{
	SIMPLE_LOG_ST *t = (SIMPLE_LOG_ST *)arg;
	while (1) {
		spl_rel_sem(t->sem_rwfile);
		spl_milli_sleep(t->trigger_thread);
	}
	return 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef UNIX_LINUX
int
spl_create_thread(THREAD_ROUTINE f, void *arg, HANDLE *outhd)
#else
int
spl_create_thread(THREAD_ROUTINE f, void *arg, pthread_t *outid)
#endif
{
	int ret = 0;
#ifndef UNIX_LINUX
	DWORD dwThreadId = 0;
	HANDLE hThread = 0;
	hThread = CreateThread(NULL, 0, f, arg, 0, &dwThreadId);
	if (!hThread) {
		ret = SPL_LOG_THREAD_W32_CREATE;
		spl_console_log("CreateThread error: %d", (int)GetLastError());
	}
	*outhd = hThread;
#else
	pthread_t tidd = 0;
	ret = pthread_create(&tidd, 0, f, arg);
	if (ret) {
		ret = SPL_LOG_THREAD_PX_CREATE;
		spl_console_log("pthread_create: ret: %d, errno: %d, text: %s.", ret, errno, strerror(errno));
	}
	*outid = tidd;
#endif
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
#define spl_shm_unlink(__name__, __err__)                                                                                   \
	{                                                                                                                   \
		__err__ = shm_unlink(__name__);                                                                             \
		if (__err__) {                                                                                              \
			spl_console_log("shm_unlink: err: %d, errno: %d, text: %s, name: %s.", __err__, errno,              \
			    strerror(errno), __name__);                                                                     \
		}                                                                                                           \
	}

int
spl_del_memory()
{
	int ret = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	do {
#ifndef UNIX_LINUX
		int isWell = 0;
		isWell = (int)UnmapViewOfFile((void *)t->buf);
		if (!isWell) {
			spl_console_log("UnmapViewOfFile error: %d", (int)GetLastError());
			ret = SPL_LOG_SHM_WIN_UNMAP;
		}
		isWell = (int)CloseHandle((HANDLE)t->hd);
		if (!isWell) {
			spl_console_log("SPL_LOG_WIN_SHM_CLOSE, err: %d", (int)GetLastError());
			ret = SPL_LOG_WIN_SHM_CLOSE;
		}
#else
#ifdef __MACH__
		ret = munmap((void *)t->buf, (size_t)t->map_mem_size);
		if (ret) {
			ret = SPL_LOG_SHM_UNIX_UNMAP;
			spl_console_log(
			    "shm_unlink: err: %d, errno: %d, text: %s, name: %s.", ret, errno, strerror(errno), "__name__");
		}
		spl_shm_unlink(t->shared_key, ret);
#else
		ret = munmap((void *)t->buf, (size_t)t->map_mem_size);
		if (ret) {
			ret = SPL_LOG_SHM_UNIX_UNMAP;
			spl_console_log(
			    "shm_unlink: err: %d, errno: %d, text: %s, name: %s.", ret, errno, strerror(errno), "__name__");
		}
		spl_shm_unlink(t->shared_key, ret);
#endif
#endif
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int
spl_create_memory(void **output, char *shared_key, int size_shared, char isCreating)
{
	int ret = 0;
	char *p = 0;
	do {
#ifndef UNIX_LINUX
		HANDLE hMapFile = 0;
		char *p = 0;
		if (!output) {
			ret = SPL_LOG_SHM_CREATE_NULL;
			break;
		}
		if (isCreating) {
			hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, /* use paging file */
			    NULL, /* // default security */
			    PAGE_READWRITE, /* // read/write access */
			    0, /* // maximum object size (high-order DWORD) */
			    size_shared, /*   // maximum object size (low-order DWORD) */
			    shared_key); /*    // name of mapping object */

			if (!hMapFile) {
				spl_console_log("Cannot create SHM. error: %d\n", (int)GetLastError());
				ret = SPL_LOG_WIN32_MAP_FILE;
				break;
			}
		} else {
			hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, 0, shared_key);
			if (!hMapFile) {
				ret = 2;
				spl_console_log("Cannot open SHM. error: %d\n", (int)GetLastError());
				break;
			}
		}
		if (ret) {
			break;
		}
		p = (char *)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size_shared);
		if (!p) {
			ret = 3;
			spl_console_log("Cannot MapViewOfFile. error: %d\n", (int)GetLastError());
			break;
		}
#else
		int hMapFile = 0;
		int err = 0;
		hMapFile = shm_open(shared_key, SPL_LOG_UNIX_CREATE_MODE, SPL_LOG_UNIX__SHARED_MODE);
		if (hMapFile > 0) {
			break;
		}
		hMapFile = shm_open(shared_key, SPL_LOG_UNIX_OPEN_MODE, SPL_LOG_UNIX__SHARED_MODE);
		if (hMapFile < 1) {
			spl_console_log("SPL_LOG_SHM_UNIX_OPEN option creating");
			ret = SPL_LOG_SHM_UNIX_OPEN;
			break;
		}
		err = ftruncate(hMapFile, size_shared);
		if (err) {
			spl_console_log("SPL_LOG_SHM_UNIX_TRUNC");
			ret = SPL_LOG_SHM_UNIX_TRUNC;
			break;
		}
		p = (char *)mmap(0, size_shared, SPL_LOG_UNIX_PROT_FLAGS, MAP_SHARED, hMapFile, 0);
		if (p == MAP_FAILED || p == 0) {
			ret = SPL_LOG_SHM_UNIX_MAP_FAILED;
			spl_console_log("SPL_LOG_SHM_UNIX_MAP_FAILED");
			p = 0;
			break;
		}
#endif
		memset(p, 0, size_shared);
		*output = (void *)p;

	} while (0);
	return ret = 0;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
/* (SIMPLE_LOG_ST*)&__simple_log_static__, volatile long, pthread_spinlock_t, pthread_mutex_t */

int
spl_calculate_size()
{
	int ret = 0;
	int k = 0;
	int n = 0;
	int mtxsize = 0;
	int semsize = 0;
	int size_arr_mtx = 0;
	char *buff = 0;

#ifndef UNIX_LINUX
#ifdef SPL_USING_SPIN_LOCK
	int i = 0;
	char *p = 0;
	int step_size = 0;
#else
#endif
#else
	int i = 0;
	char *p = 0;
	int step_size = 0;
#endif
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	size_arr_mtx = t->ncpu * sizeof(void *);
	/*k: For buffer.*/
	k = t->buff_size * t->ncpu * (t->n_topic + 1);
	/*k = t->buff_size * t->ncpu + t->buff_size * t->ncpu * t->n_topic;*/
	do {
		if (!t->arr_mtx) {
			spl_malloc(size_arr_mtx, t->arr_mtx, void *);
			if (!t->arr_mtx) {
				ret = SPL_LOG_ARR_MTX_NULL;
				spl_console_log("spl_malloc: SPL_LOG_ARR_MTX_NULL.");
				break;
			}
		}
#ifndef UNIX_LINUX
#ifdef SPL_USING_SPIN_LOCK
		step_size = sizeof(volatile long);
		mtxsize = (1 + t->ncpu) * step_size;
		semsize = 0;
#else
		/*t->mtx_rw: is NamedMutex*/
		mtxsize = 0;
		/*semsize*/
		semsize = 0;
#endif
#else
#ifdef __MACH__
#ifdef SPL_USING_SPIN_LOCK
		step_size = sizeof(pthread_spinlock_t);
#error "not yet implemented."
		/*
			- https://developer.apple.com/documentation/os/os_unfair_lock_t : iOS 10.0+
				iPadOS 10.0+
				Mac Catalyst 13.1+
				macOS 10.12+
				tvOS 10.0+
				visionOS 1.0+
				watchOS 3.0+
			-
		   https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/OSSpinLockTry.3.html
				- May 26, 2004   Darwin
		*/
#else
		step_size = sizeof(pthread_mutex_t);
#endif
		mtxsize = (1 + t->ncpu) * step_size;
		/*1: For semrw.*/
		/*1: For semOnOff.*/
		semsize = 0;
#else
#ifdef SPL_USING_SPIN_LOCK
		step_size = sizeof(pthread_spinlock_t);
#else
		step_size = sizeof(pthread_mutex_t);
#endif
		mtxsize = (1 + t->ncpu) * step_size;
		/*1: For semrw.*/
		/*1: For semOnOff.*/
		semsize = 2 * sizeof(sem_t);
#endif
#endif
		/*k: For buffer.*/
		/*mtxsize: mutex size.*/
		/*semsize: sem size.*/
		/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
		spl_console_log("buf size: %d, mtxsize: %d, semsize: %d", (int)k, (int)mtxsize, (int)semsize);
		n = k + mtxsize + semsize;
		t->map_mem_size = n;
		/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
		/* int spl_create_memory(void** output, char* shared_key, int size_shared, char isCreating) */
		if (t->isProcessMode) {
			spl_create_memory((void **)&buff, t->shared_key, n, t->is_master);
		} else {
			spl_malloc(n, buff, char);
		}
		if (!buff) {
			ret = SPL_LOG_BUFF_MALLOC_ERROR;
			spl_console_log("spl_malloc: SPL_LOG_BUFF_MALLOC_ERROR.");
			break;
		}
		t->buf = (spl_gen_data_st *)buff;

#ifndef UNIX_LINUX
#ifdef SPL_USING_SPIN_LOCK

		t->mtx_rw = (void *)(buff + k);

		step_size = sizeof(volatile long);
		p = (buff + k) + step_size;
		for (i = 0; i < t->ncpu; ++i) {
			t->arr_mtx[i] = (void *)(p + i * step_size);
		}
#else
		/*t->mtx_rw: is NamedMutex*/
		/*t->mtx_rw: is NamedSem*/
#endif
		ret = spl_win32_sync_create();
#else
		t->mtx_rw = (void *)(buff + k);
#ifdef SPL_USING_SPIN_LOCK
		if (t->mtx_rw) {
			pthread_spinlock_t *mtx = (pthread_spinlock_t *)t->mtx_rw;
			if (t->isProcessMode) {
				int err = 0;
				err = pthread_spin_init(mtx, PTHREAD_PROCESS_SHARED);
				if (err) {
					ret = SPL_LOG_SPINLOCK_INIT_SHARED;
					spl_console_log(
					    "pthread_spin_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				}
			} else {
				int err = 0;
				err = pthread_spin_init(mtx, PTHREAD_PROCESS_PRIVATE);
				if (err) {
					ret = SPL_LOG_SPINLOCK_INIT_PRIVATE;
					spl_console_log(
					    "pthread_spin_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				}
			}
		}
		step_size = sizeof(pthread_spinlock_t);
		p = buff + k + step_size;
		for (i = 0; i < t->ncpu; ++i) {
			pthread_spinlock_t *mtx = 0;
			t->arr_mtx[i] = (void *)(p + i * step_size);
			mtx = (pthread_spinlock_t *)t->arr_mtx[i];
			if (t->isProcessMode) {
				pthread_spin_init(mtx, PTHREAD_PROCESS_SHARED);
			} else {
				pthread_spin_init(mtx, PTHREAD_PROCESS_PRIVATE);
			}
		}
#else
		if (t->mtx_rw) {
			pthread_mutex_t *mtx = (pthread_mutex_t *)t->mtx_rw;
			ret = spl_mtx_init(mtx, t->isProcessMode);
		}
		step_size = sizeof(pthread_mutex_t);
		p = buff + k + step_size;
		for (i = 0; i < t->ncpu; ++i) {
			/*pthread_mutex_t* mtx = 0;*/
			t->arr_mtx[i] = (void *)(p + i * step_size);
			/*mtx = (pthread_mutex_t*)t->arr_mtx[i];*/
			ret = spl_mtx_init(t->arr_mtx[i], t->isProcessMode);
			if (ret) {
				break;
			}
		}
#endif
#ifdef __MACH__
		ret = spl_osx_sync_create();
#else
		/*Semaphore UNIX_LINUX*/
		p = buff + k + mtxsize;
		t->sem_rwfile = (void *)p;
		t->sem_off = (void *)(p + sizeof(sem_t));
		/*
		 * https://linux.die.net/man/3/sem_init
		 * #include <semaphore.h>
		 * int sem_init(sem_t *sem, int pshared, unsigned int value);
		 */
		if (t->sem_rwfile && t->sem_off) {
			int err = 0;
			err = sem_init((sem_t *)t->sem_rwfile, (int)t->isProcessMode, 0);
			if (err) {
				ret = SPL_LOG_SEM_INIT_UNIX;
				spl_console_log("sem_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				break;
			}
			err = sem_init((sem_t *)t->sem_off, (int)t->isProcessMode, 0);
			if (err) {
				ret = SPL_LOG_SEM_INIT_UNIX;
				spl_console_log("sem_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				break;
			}
		}
#endif
#endif
	} while (0);

	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

#ifndef UNIX_LINUX
int
spl_win32_sync_create()
{
	int ret = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	char nameobj[SPL_SHARED_NAME_LEN];
	do {
#ifdef SPL_USING_SPIN_LOCK
#else
		if (!t->arr_mtx) {
			int n = t->ncpu * sizeof(void *);
			spl_malloc(n, t->arr_mtx, void *);
			if (!t->arr_mtx) {
				ret = SPL_LOG_ARR_MTX_NULL;
				spl_console_log("spl_malloc: SPL_LOG_ARR_MTX_NULL");
				break;
			}
		}
		if (t->isProcessMode) {
			HANDLE hd = 0;
			int i = 0;
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_MTX_NAME_OFF, t->shared_key);
			hd = CreateMutexA(0, 0, nameobj);
			if (!hd) {
				ret = SPL_LOG_MTX_WIN32_CREATED_ERROR;
				spl_console_log("CreateMutexA, errno: %d.", (int)GetLastError());
				break;
			}
			t->mtx_rw = hd;

			for (i = 0; i < t->ncpu; ++i) {
				snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s_%0.2d", SPL_MTX_NAME_OFF, t->shared_key, i);
				hd = CreateMutexA(0, 0, nameobj);
				if (!hd) {
					ret = SPL_LOG_MTX_WIN32_CREATED_ERROR;
					spl_console_log("CreateMutexA, errno: %d.", (int)GetLastError());
					break;
				}
				t->arr_mtx[i] = hd;
			}
		} else {
			HANDLE hd = 0;
			int i = 0;
			hd = CreateMutexA(0, 0, 0);
			if (!hd) {
				ret = SPL_LOG_MTX_WIN32_CREATED_ERROR;
				spl_console_log("CreateMutexA, errno: %d.", (int)GetLastError());
				break;
			}
			t->mtx_rw = hd;

			for (i = 0; i < t->ncpu; ++i) {
				hd = CreateMutexA(0, 0, 0);
				if (!hd) {
					ret = SPL_LOG_MTX_WIN32_CREATED_ERROR;
					spl_console_log("CreateMutexA, errno: %d.", (int)GetLastError());
					break;
				}
				t->arr_mtx[i] = hd;
			}
		}
#endif
		if (ret) {
			break;
		}
		if (t->isProcessMode) {
			HANDLE hd = 0;
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_RW, t->shared_key);
			hd = CreateSemaphoreA(0, 0, 1, nameobj);
			if (!hd) {
				spl_console_log("CreateSemaphoreA, errno: %d.", (int)GetLastError());
				ret = SPL_LOG_SEM_WIN32_CREATED_ERROR;
				break;
			}
			t->sem_rwfile = hd;
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_OFF, t->shared_key);
			hd = CreateSemaphoreA(0, 0, 1, nameobj);
			if (!hd) {
				spl_console_log("CreateSemaphoreA, errno: %d.", (int)GetLastError());
				ret = SPL_LOG_SEM_WIN32_CREATED_ERROR;
				break;
			}
			t->sem_off = hd;
		} else {
			HANDLE hd = 0;
			hd = CreateSemaphoreA(0, 0, 1, 0);
			if (!hd) {
				spl_console_log("CreateSemaphoreA, errno: %d.", (int)GetLastError());
				ret = SPL_LOG_SEM_WIN32_CREATED_ERROR;
				break;
			}
			t->sem_rwfile = hd;
			hd = CreateSemaphoreA(0, 0, 1, 0);
			if (!hd) {
				spl_console_log("CreateSemaphoreA, errno: %d.", (int)GetLastError());
				ret = SPL_LOG_SEM_WIN32_CREATED_ERROR;
				break;
			}
			t->sem_off = hd;
		}
	} while (0);
	return ret;
}
#else
#ifdef __MACH__
int
spl_osx_sync_del()
{
	int ret = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	char nameobj[SPL_SHARED_NAME_LEN];
	do {
		if (!t->sem_rwfile) {
			spl_console_log("Did clean.");
			break;
		}
		if (t->isProcessMode | 1) {
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_RW, t->shared_key);
			if (sem_close((sem_t *)t->sem_rwfile) == -1) {
				spl_console_log("sem_close, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_OSX_SEM_CLOSE;
			}
			t->sem_rwfile = 0;

			if (sem_unlink(nameobj) == -1) {
				spl_console_log("sem_unlink, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_OSX_SEM_UNLINK;
			}
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_OFF, t->shared_key);
			if (sem_close((sem_t *)t->sem_off) == -1) {
				spl_console_log("sem_close, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_OSX_SEM_CLOSE;
			}
			t->sem_off = 0;

			if (sem_unlink(nameobj) == -1) {
				spl_console_log("sem_unlink, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_OSX_SEM_UNLINK;
			}
		} else {
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_RW, t->shared_key);
			if (sem_close((sem_t *)t->sem_rwfile) == -1) {
				spl_console_log("sem_close, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_OSX_SEM_CLOSE;
			}
			t->sem_rwfile = 0;

			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_OFF, t->shared_key);
			if (sem_close((sem_t *)t->sem_off) == -1) {
				spl_console_log("sem_close, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_OSX_SEM_CLOSE;
			}
			t->sem_off = 0;
		}
	} while (0);
	return ret;
}

int
spl_osx_sync_create()
{
	int ret = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	char nameobj[SPL_SHARED_NAME_LEN];
	do {
#ifdef SPL_USING_SPIN_LOCK
#else
#endif

		if (t->isProcessMode | 1) {
			int retry = 0;
			sem_t *hd = 0;
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_RW, t->shared_key);
			do {
				hd = sem_open(nameobj, SPL_LOG_UNIX_CREATE_MODE, SPL_LOG_UNIX__SHARED_MODE, 1);
				if (hd == SEM_FAILED) {
					spl_console_log("sem_open, errno: %d, errno_text: %s.", errno, strerror(errno));
					ret = SPL_LOG_SEM_OSX_CREATED_ERROR;
					if (retry) {
						break;
					} else {
						ret = sem_unlink(nameobj);
						if (ret) {
							spl_console_log("sem_unlink, errno: %d, errno_text: %s.", errno,
							    strerror(errno));
							ret = SPL_LOG_SEM_OSX_UNLINK_ERROR;
							break;
						}
						retry++;
						continue;
					}
				}
				break;
			} while (1);

			if (ret) {
				break;
			}
			t->sem_rwfile = hd;

			retry = 0;
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_OFF, t->shared_key);
			do {
				hd = sem_open(nameobj, SPL_LOG_UNIX_CREATE_MODE, SPL_LOG_UNIX__SHARED_MODE, 1);
				if (hd == SEM_FAILED) {
					spl_console_log("sem_open, errno: %d, errno_text: %s.", errno, strerror(errno));
					ret = SPL_LOG_SEM_OSX_CREATED_ERROR;
					if (retry) {
						break;
					} else {
						ret = sem_unlink(nameobj);
						if (ret) {
							spl_console_log("sem_unlink, errno: %d, errno_text: %s.", errno,
							    strerror(errno));
							ret = SPL_LOG_SEM_OSX_UNLINK_ERROR;
							break;
						}
						retry++;
						continue;
					}
				}
				break;
			} while (1);
			if (ret) {
				break;
			}
			sem_wait(hd);
			t->sem_off = hd;
		} else {
			sem_t *hd = 0;
			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_RW, t->shared_key);
			hd = sem_open(nameobj, SPL_LOG_UNIX_OPEN_MODE);
			if (hd == SEM_FAILED) {
				spl_console_log("sem_open, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_SEM_OSX_CREATED_ERROR;
				break;
			}
			t->sem_rwfile = hd;

			snprintf(nameobj, SPL_SHARED_NAME_LEN, "%s_%s", SPL_SEM_NAME_OFF, t->shared_key);
			hd = sem_open(nameobj, SPL_LOG_UNIX_OPEN_MODE);
			if (hd == SEM_FAILED) {
				spl_console_log("sem_open, errno: %d, errno_text: %s.", errno, strerror(errno));
				ret = SPL_LOG_SEM_OSX_CREATED_ERROR;
				break;
			}
			t->sem_off = hd;
		}
	} while (0);
	return ret;
}
#endif
int
spl_mtx_init(void *obj, char shared)
{
	int ret = 0;
	int err = 0;
	pthread_mutex_t *mtx = (pthread_mutex_t *)obj;
	do {
		if (shared) {
			pthread_mutexattr_t psharedm;
			err = pthread_mutexattr_init(&psharedm);
			if (err) {
				ret = SPL_LOG_MTX_ATT_SHARED_MODE;
				spl_console_log(
				    "pthread_mutexattr_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				break;
			}
			err = pthread_mutexattr_setpshared(&psharedm, PTHREAD_PROCESS_SHARED);
			if (err) {
				ret = SPL_LOG_MTX_ATT_SHARED_MODE_SET;
				spl_console_log(
				    "pthread_mutexattr_setpshared, errno: %d, errno_text: %s.", errno, strerror(errno));
				break;
			}
			err = pthread_mutex_init(mtx, &psharedm);
			if (err) {
				ret = SPL_LOG_SHM_UNIX_INIT_MUTEX;
				spl_console_log("pthread_mutex_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				break;
			}
		} else {
			err = pthread_mutex_init(mtx, 0);
			if (err) {
				ret = SPL_LOG_MTX_INIT_ERR;
				spl_console_log("pthread_mutex_init, errno: %d, errno_text: %s.", errno, strerror(errno));
				break;
			}
		}
	} while (0);
	return ret;
}
#endif

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
static void
spl_fmt_segment(spl_gen_data_st *sgment)
{
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	sgment->total = t->buff_size;
	sgment->pl = 0;
	sgment->pc = 0;
}
int
spl_init_segments()
{
	int ret = 0;
	char *p = 0;
	char *seg = 0;
	int i = 0;
	int k = 0;
	int step = 0;
	spl_gen_data_st *sgment = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	p = (char *)t->buf;
	if (!t->range) {
		t->range = t->buff_size - (sizeof(spl_gen_data_st) + t->max_sz_msg + SPL_RL_BUF);
		t->krange = t->range + t->max_sz_msg;
	}
	do {
		for (i = 0; i < t->ncpu; ++i) {
			seg = p + i * t->buff_size;
			sgment = (spl_gen_data_st *)seg;
			spl_fmt_segment(sgment);
		}
		step = t->buff_size * t->ncpu;
		for (k = 0; k < t->n_topic; ++k) {
			p += step;
			t->arr_topic[k].buf = (spl_gen_data_st *)p;
			for (i = 0; i < t->ncpu; ++i) {
				seg = p + i * t->buff_size;
				sgment = (spl_gen_data_st *)seg;
				spl_fmt_segment(sgment);
			}
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int
spl_allocate_topics()
{
	int ret = 0;
	int i = 0;
	char *p0 = 0;
	char *p1 = 0;
	int n = 0;
	int szitopics = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	do {
		if (!t->n_topic) {
			break;
		}
		szitopics = sizeof(SIMPLE_LOG_TOPIC_ST) * t->n_topic;
		spl_malloc(szitopics, t->arr_topic, SIMPLE_LOG_TOPIC_ST);
		if (!t->arr_topic) {
			ret = SPL_LOG_TOPIC_MEMORY;
			break;
		}
		p0 = t->topics;
		for (i = 0; i < t->n_topic; ++i) {
			p1 = strstr(p0, ",");
			if (!p1) {
				snprintf(t->arr_topic[i].topic, SPL_TOPIC_SIZE, "%s", p0);
				continue;
			}
			n = (int)(p1 - p0);
			if (n > 0) {
				snprintf(t->arr_topic[i].topic, SPL_MIN_AB(SPL_TOPIC_SIZE, n + 1), "%s", p0);
			}
			p1++;
			p0 = p1;
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
int
spl_gen_sync_tool()
{
	int ret = 0;
	do {
		ret = spl_allocate_topics();
		if (ret) {
			break;
		}
		ret = spl_calculate_size();
		if (ret) {
			break;
		}
		ret = spl_init_segments();
		if (ret) {
			break;
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/

int
spl_clean_sync_tool()
{
	int ret = 0;
	SIMPLE_LOG_ST *t = &__simple_log_static__;
	do {
		if (t->n_topic > 0) {
			spl_free(t->topics);
			spl_free(t->arr_topic);
		}
#ifndef UNIX_LINUX
#ifdef SPL_USING_SPIN_LOCK
#else
		int i = 0;
		SPL_CloseHandle(t->mtx_rw);
		for (i = 0; i < t->ncpu; ++i) {
			SPL_CloseHandle(t->arr_mtx[i]);
		}
#endif
		SPL_CloseHandle(t->sem_rwfile);
		SPL_CloseHandle(t->sem_off);
#else
#ifdef __MACH__
		/* Clear semaphore of MAC OSX. */
		ret = spl_osx_sync_del();
#endif
#endif
		spl_free(t->arr_mtx);
		if (t->isProcessMode) {
			ret = spl_del_memory();
		} else {
			spl_free(t->buf);
		}
	} while (0);
	return ret;
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
static const char *__spl_err_text__[SPL_END_ERROR + 1];

void
spl_err_txt_init()
{
	__spl_err_text__[SPL_NO_ERROR] = "SPL_NO_ERROR";
	__spl_err_text__[SPL_INIT_PATH_FOLDER_EMPTY_ERROR] = "SPL_INIT_PATH_FOLDER_EMPTY_ERROR";
	__spl_err_text__[SPL_LOG_LEVEL_ERROR] = "SPL_LOG_LEVEL_ERROR";
	__spl_err_text__[SPL_ERROR_CREATE_MUTEX] = "SPL_ERROR_CREATE_MUTEX";
	__spl_err_text__[SPL_ERROR_CREATE_SEM] = "SPL_ERROR_CREATE_SEM";
	__spl_err_text__[SPL_LOG_BUFF_SIZE_ERROR] = "SPL_LOG_BUFF_SIZE_ERROR";
	__spl_err_text__[SPL_LOG_BUFF_MALLOC_ERROR] = "SPL_LOG_BUFF_MALLOC_ERROR";
	__spl_err_text__[SPL_LOG_MAX_SZ_MSG_ERROR] = "SPL_LOG_MAX_SZ_MSG_ERROR";
	__spl_err_text__[SPL_LOG_FOLDER_ERROR] = "SPL_LOG_FOLDER_ERROR";
	__spl_err_text__[SPL_LOG_CREATE_THREAD_ERROR] = "SPL_LOG_CREATE_THREAD_ERROR";
	__spl_err_text__[SPL_LOG_FMT_NULL_ERROR] = "SPL_LOG_FMT_NULL_ERROR";
	__spl_err_text__[SPL_LOG_MEM_GEN_FILE_ERROR] = "SPL_LOG_MEM_GEN_FILE_ERROR";
	__spl_err_text__[SPL_LOG_MEM_MALLOC_ERROR] = "SPL_LOG_MEM_MALLOC_ERROR";
	__spl_err_text__[SPL_LOG_OPEN_FILE_ERROR] = "SPL_LOG_OPEN_FILE_ERROR";
	__spl_err_text__[SPL_LOG_OPEN1_FILE_ERROR] = "SPL_LOG_OPEN1_FILE_ERROR";
	__spl_err_text__[SPL_LOG_CLOSE_FILE_ERROR] = "SPL_LOG_CLOSE_FILE_ERROR";
	__spl_err_text__[SPL_LOG_SEM_NULL_ERROR] = "SPL_LOG_SEM_NULL_ERROR";
	__spl_err_text__[SPL_LOG_SEM_WIN32_CREATED_ERROR] = "SPL_LOG_SEM_WIN32_CREATED_ERROR";
	__spl_err_text__[SPL_LOG_MTX_WIN32_CREATED_ERROR] = "SPL_LOG_MTX_WIN32_CREATED_ERROR";
	__spl_err_text__[SPL_LOG_ROT_SIZE_ERROR] = "SPL_LOG_ROT_SIZE_ERROR";
	__spl_err_text__[SPL_LOG_TOPIC_EMPTY] = "SPL_LOG_TOPIC_EMPTY";
	__spl_err_text__[SPL_LOG_TOPIC_NULL] = "SPL_LOG_TOPIC_NULL";
	__spl_err_text__[SPL_LOG_MEM_FILE_MALLOC_ERROR] = "SPL_LOG_MEM_FILE_MALLOC_ERROR";
	__spl_err_text__[SPL_LOG_CHECK_FOLDER_ERROR] = "SPL_LOG_CHECK_FOLDER_ERROR";
	__spl_err_text__[SPL_LOG_CHECK_FOLDER_YEAR_ERROR] = "SPL_LOG_CHECK_FOLDER_YEAR_ERROR";
	__spl_err_text__[SPL_LOG_CHECK_FILE_YEAR_ERROR] = "SPL_LOG_CHECK_FILE_YEAR_ERROR";
	__spl_err_text__[SPL_LOG_CHECK_FOLDER_NULL_ERROR] = "SPL_LOG_CHECK_FOLDER_NULL_ERROR";
	__spl_err_text__[SPL_LOG_MUTEX_NULL_ERROR] = "SPL_LOG_MUTEX_NULL_ERROR";
	__spl_err_text__[SPL_LOG_ST_NAME_NULL_ERROR] = "SPL_LOG_ST_NAME_NULL_ERROR";
	__spl_err_text__[SPL_LOG_TIME_NULL_ERROR] = "SPL_LOG_TIME_NULL_ERROR";
	__spl_err_text__[SPL_LOG_TIME_NANO_NULL_ERROR] = "SPL_LOG_TIME_NANO_NULL_ERROR";
	__spl_err_text__[SPL_LOG_STAT_FOLDER_ERROR] = "SPL_LOG_STAT_FOLDER_ERROR";
	__spl_err_text__[SPL_LOG_PRINTF_ERROR] = "SPL_LOG_PRINTF_ERROR";
	__spl_err_text__[SPL_LOG_TOPIC_ZERO] = "SPL_LOG_TOPIC_ZERO";
	__spl_err_text__[SPL_LOG_TOPIC_MEMORY] = "SPL_LOG_TOPIC_MEMORY";
	__spl_err_text__[SPL_LOG_TOPIC_FOPEN] = "SPL_LOG_TOPIC_FOPEN";
	__spl_err_text__[SPL_LOG_TOPIC_FLUSH] = "SPL_LOG_TOPIC_FLUSH";
	__spl_err_text__[SPL_LOG_TOPIC_BUFF_MEM] = "SPL_LOG_TOPIC_BUFF_MEM";
	__spl_err_text__[SPL_LOG_ALOCK_NUM] = "SPL_LOG_ALOCK_NUM";
	__spl_err_text__[SPL_LOG_ALOCK_NULL] = "SPL_LOG_ALOCK_NULL";
	__spl_err_text__[SPL_LOG_SHM_CREATE_NULL] = "SPL_LOG_SHM_CREATE_NULL";
	__spl_err_text__[SPL_LOG_SHM_WIN_UNMAP] = "SPL_LOG_SHM_WIN_UNMAP";
	__spl_err_text__[SPL_LOG_SHM_UNIX_OPEN] = "SPL_LOG_SHM_UNIX_OPEN";
	__spl_err_text__[SPL_LOG_SHM_UNIX_TRUNC] = "SPL_LOG_SHM_UNIX_TRUNC";
	__spl_err_text__[SPL_LOG_SHM_UNIX_MAP_FAILED] = "SPL_LOG_SHM_UNIX_MAP_FAILED";
	__spl_err_text__[SPL_LOG_WIN_SHM_CLOSE] = "SPL_LOG_WIN_SHM_CLOSE";
	__spl_err_text__[SPL_LOG_SHM_UNIX_UNMAP] = "SPL_LOG_SHM_UNIX_UNMAP";
	__spl_err_text__[SPL_LOG_VAR_NULL] = "SPL_LOG_VAR_NULL";
	__spl_err_text__[SPL_LOG_ARR_MTX_NULL] = "SPL_LOG_ARR_MTX_NULL";
	__spl_err_text__[SPL_LOG_ARR_BUFF_NULL] = "SPL_LOG_ARR_BUFF_NULL";
	__spl_err_text__[SPL_LOG_MTX_ATT_SHARED_MODE] = "SPL_LOG_MTX_ATT_SHARED_MODE";
	__spl_err_text__[SPL_LOG_MTX_ATT_SHARED_MODE_SET] = "SPL_LOG_MTX_ATT_SHARED_MODE_SET";
	__spl_err_text__[SPL_LOG_MTX_INIT_ERR] = "SPL_LOG_MTX_INIT_ERR";
	__spl_err_text__[SPL_LOG_SHM_UNIX_INIT_MUTEX] = "SPL_LOG_SHM_UNIX_INIT_MUTEX";
	__spl_err_text__[SPL_LOG_SPINLOCK_INIT_SHARED] = "SPL_LOG_SPINLOCK_INIT_SHARED";
	__spl_err_text__[SPL_LOG_SPINLOCK_INIT_PRIVATE] = "SPL_LOG_SPINLOCK_INIT_PRIVATE";
	__spl_err_text__[SPL_LOG_SEM_INIT_UNIX] = "SPL_LOG_SEM_INIT_UNIX";
	__spl_err_text__[SPL_LOG_THREAD_W32_CREATE] = "SPL_LOG_THREAD_W32_CREATE";
	__spl_err_text__[SPL_LOG_THREAD_PX_CREATE] = "SPL_LOG_THREAD_PX_CREATE";
	__spl_err_text__[SPL_LOG_MACH_GETTIME_ERROR] = "SPL_LOG_MACH_GETTIME_ERROR";
	__spl_err_text__[SPL_LOG_MACH_CLOCK_SERVICE_ERROR] = "SPL_LOG_MACH_CLOCK_SERVICE_ERROR";
	__spl_err_text__[SPL_LOG_OSX_SEM_CLOSE] = "SPL_LOG_OSX_SEM_CLOSE";
	__spl_err_text__[SPL_LOG_OSX_SEM_UNLINK] = "SPL_LOG_OSX_SEM_UNLINK";
	__spl_err_text__[SPL_LOG_SEM_OSX_CREATED_ERROR] = "SPL_LOG_SEM_OSX_CREATED_ERROR";
	__spl_err_text__[SPL_LOG_SEM_INIT_OSX] = "SPL_LOG_SEM_INIT_OSX";
	__spl_err_text__[SPL_LOG_SEM_OSX_UNLINK_ERROR] = "SPL_LOG_SEM_OSX_UNLINK_ERROR";
	__spl_err_text__[SPL_LOG_OPEN_CFG] = "SPL_LOG_OPEN_CFG";
	__spl_err_text__[SPL_LOG_WIN32_WAIT_OBJECT] = "SPL_LOG_WIN32_WAIT_OBJECT";
	__spl_err_text__[SPL_LOG_WIN32_MUTEX_RELEASE] = "SPL_LOG_WIN32_MUTEX_RELEASE";
	__spl_err_text__[SPL_LOG_WIN32_MAP_FILE] = "SPL_LOG_WIN32_MAP_FILE";

	__spl_err_text__[SPL_END_ERROR] = "SPL_END_ERROR";
}
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
const char *
spl_err_txt(int i)
{
	if (i < 0) {
		return "UNKNOW - less than 0.";
	}
	if (i > SPL_END_ERROR) {
		return "UNKNOW - greater than SPL_END_ERROR.";
	}
	return __spl_err_text__[i];
}
#ifndef UNIX_LINUX
#else
#endif

#ifndef UNIX_LINUX
#ifdef SPL_USING_SPIN_LOCK
#else
#endif
#else
#ifdef SPL_USING_SPIN_LOCK
#else
#endif
#endif
/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-*/
