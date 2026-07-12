#include "simplelog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "simplelog_bin_parser.h"
#ifndef UNIX_LINUX

#include <Windows.h>
DWORD WINAPI
win32_thread_routine(LPVOID lpParam);
#else
#include <unistd.h>
#include <pthread.h>
void *
posix_thread_routine(void *lpParam);
#endif

void
dotest();
int num_threads = 10;
int loop_count = 1000 * 1000;
int topicindex = -1;
int topicBindex = -1;
int mainindex = -1;

#define TNUMBEER_OF_THREADS "--nthread="
#define TCONFIG_FILE        "--cfg="
#define TLOOP_COUNT         "--loopcount="
#define TMASTER_MODE        "--is_master="
#define TTOPIC_INDEX        "--topic_index="
#define TTOPIC_BINDEX       "--topic_Bindex="
#define MAIN_INDEX          "--mainindex="

int
main(int argc, char *argv[])
{
	int ret = 0, i = 0;
	int len = 0;
	char cfgpath[1024];
	SPL_INPUT_ARG input = {0};
	snprintf(cfgpath, 1024, "simplelog.cfg");
	for (i = 1; i < argc; ++i) {
		if (strstr(argv[i], TNUMBEER_OF_THREADS) == argv[i]) {
			ret = sscanf(argv[i], TNUMBEER_OF_THREADS "%d", &num_threads);
			if (!ret) {
				return 1;
			}
			continue;
		}
		if (strstr(argv[i], TLOOP_COUNT) == argv[i]) {
			ret = sscanf(argv[i], TLOOP_COUNT "%d", &loop_count);
			continue;
		}
		if (strstr(argv[i], MAIN_INDEX) == argv[i]) {
			ret = sscanf(argv[i], MAIN_INDEX "%d", &mainindex);
			continue;
		}
		if (strstr(argv[i], TTOPIC_INDEX) == argv[i]) {
			ret = sscanf(argv[i], TTOPIC_INDEX "%d", &topicindex);
			continue;
		}
		if (strstr(argv[i], TTOPIC_BINDEX) == argv[i]) {
			ret = sscanf(argv[i], TTOPIC_BINDEX "%d", &topicBindex);
			continue;
		}

		if (strstr(argv[i], TCONFIG_FILE) == argv[i]) {
			memset(cfgpath, 0, sizeof(cfgpath));
			snprintf(cfgpath, 1024, "%s", argv[i] + sizeof(TCONFIG_FILE) - 1);
			continue;
		}
	}
	len = SPL_MIN_AB(SPL_PATH_FOLDER, strlen(cfgpath) + 1);
	spl_console_log("cfgpath: %s.\n", cfgpath);
	snprintf(input.folder, len, "%s", cfgpath);
	spl_console_log("input.folder: %s.\n", input.folder);
	ret = spl_init_log_ext(&input);

	spl_console_log("====================Start.\n");
	dotest();
	spl_console_log("==================End.\n");
	spl_finish_log();
	return EXIT_SUCCESS;
}
void
dotest()
{
	int i = 0;
#ifndef UNIX_LINUX

	DWORD *dwpThreadId = 0, dwEvent = 0;
	HANDLE *hpThread = 0;

	dwpThreadId = (DWORD *)malloc(num_threads * sizeof(DWORD));
	if (!dwpThreadId) {
		exit(1);
	}
	memset(dwpThreadId, 0, num_threads * sizeof(DWORD));

	hpThread = (HANDLE *)malloc(num_threads * sizeof(HANDLE));
	if (!hpThread) {
		exit(1);
	}
	memset(hpThread, 0, num_threads * sizeof(HANDLE));

	for (i = 0; i < num_threads; ++i) {
		hpThread[i] = CreateThread(NULL, 0, win32_thread_routine, 0, 0, (dwpThreadId + i));
	}
#if 0	
	// https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects
	// https://learn.microsoft.com/en-us/windows/win32/sync/waiting-for-multiple-objects
#endif
	dwEvent = WaitForMultipleObjects(num_threads, // number of objects in array
	    hpThread, // array of objects
	    TRUE, // wait for any object
	    INFINITE); // five-second wait
	free(dwpThreadId);
	free(hpThread);
#else
#if 0
	// https://man7.org/linux/man-pages/man3/pthread_create.3.html
#endif
	pthread_t *pidds = 0;
	pidds = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
	if (!pidds) {
		exit(1);
	}
	for (i = 0; i < num_threads; ++i) {
		int err = pthread_create(pidds + i, 0, posix_thread_routine, 0);
		if (err) {
			exit(1);
		}
	}
	for (i = 0; i < num_threads; i++) {
		int s = pthread_join(pidds[i], 0);
		if (s != 0) {
			spl_console_log("pthread_join error.\n");
		}
	}
	free(pidds);

#endif
}
#if 0
// topic=sys,lib,exe,nayax,sksgn
#endif

#define spllogsys(__level__, __fmt__, ...) spllogtopic(__level__, 0, __fmt__, ##__VA_ARGS__);
#define splloglib(__level__, __fmt__, ...) spllogtopic(__level__, 1, __fmt__, ##__VA_ARGS__);
#define spllogexe(__level__, __fmt__, ...) spllogtopic(__level__, 2, __fmt__, ##__VA_ARGS__);
#define spllognaxyax(__level__, __fmt__, ...) spllogtopic(__level__, 3, __fmt__, ##__VA_ARGS__);
#define spllogsksgn(__level__, __fmt__, ...) spllogtopic(__level__, 4, __fmt__, ##__VA_ARGS__);

#ifndef UNIX_LINUX
DWORD WINAPI
win32_thread_routine(LPVOID lpParam)
{
#else
void *
posix_thread_routine(void *lpParam)
{
#endif
	int count = 0;
	if (lpParam) {
		spl_console_log("%p\n", lpParam);
	}
	/*#define SPL_TEST_FMT			"test log test log test log: %d"*/
#define SPL_TEST_FMT        "My test log : %d"
	if (mainindex > 0) {
		while (count < loop_count) {
			spllog(SPL_LOG_INFO, SPL_TEST_FMT, count);
			++count;
		}
	}
	if (topicindex > -1 || topicBindex > -1) {
		count = 0;
		if (topicindex > -1) {
			while (count < loop_count) {
				spllogtopic(SPL_LOG_INFO, topicindex, SPL_TEST_FMT, count);
				++count;
			}
		}
		count = 0;
		if (topicBindex > -1) {
			while (count < loop_count) {
				char data[32] = {0};
				SPL_BIN_GEO loc = {0};
				loc.longitute = 106.37026;
				loc.latitude = 10.53447;
				snprintf(data, 32, "Hello binary simplelog.");
				spllogbintopic(0, topicBindex, SPL_PARSER_TEXT, data, 32);
				spllogbintopic(0, topicBindex, SPL_PARSER_GPS, &loc, sizeof(loc));
				++count;
			}
		}
	}
	return 0;
}
