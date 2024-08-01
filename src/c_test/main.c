#include "simplelog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef UNIX_LINUX
	#include <Windows.h>
	DWORD WINAPI win32_thread_routine(LPVOID lpParam);
#else
#	include <unistd.h>
	#include <pthread.h>
	void * posix_thread_routine(void* lpParam);
#endif // !UNIX_LINUX


void dotest();
void* main_mtx = 0;
int off_process = 0;
int set_off_process(int val) {
	int ret = 0;
	spl_mutex_lock(main_mtx);
	do {
		off_process = val;
	} while (0);
	spl_mutex_unlock(main_mtx);
	return ret;
}

int get_off_process() {
	int ret = 0;
	spl_mutex_lock(main_mtx);
	do {
		ret = off_process;
	} while (0);
	spl_mutex_unlock(main_mtx);
	return ret;
}

int number = 2;
int main(int argc, char* argv[]) {
	int n = 0, ret = 0, i = 0;
	main_mtx = spl_mutex_create(0);
	if (argc > 1) {
		n = sscanf(argv[1], "%d", &number);
	}
	spl_console_log("Main thread.\n");
	char pathcfg[1024];
	char* path = "simplelog.cfg";
	char nowfmt[64];
	snprintf(pathcfg, 1024, path);
	n = strlen(pathcfg);
	for (i = 0; i < n; ++i) {
		if (pathcfg[i] == '\\') {
			pathcfg[i] = '/';
		}
	}
	ret = spl_init_log(pathcfg, 1);
	if (ret) {
		spl_console_log("spl_init_log ret: %d", ret);
		exit(1);
	}
	spl_fmt_now(nowfmt, 64);
	spllog(SPL_LOG_INFO, "%s", "<<-->> Wait for loop.");
	n = 0;
	dotest();
	while (1) {
		FILE* fp = 0;
		spl_sleep(20);
		spllog(SPL_LOG_INFO, "%s", "<<-->> Wait for loop.");
		fp = fopen("trigger.txt", "r");
		if (fp) {
			fclose(fp);
			break;
		}

	}
	spllog(SPL_LOG_INFO, "%s", "<<-->> End of loop.");
	set_off_process(1);
	spl_sleep(1);
	spl_console_log("--Main close--");
	spl_finish_log();
	//spl_sleep(1000);
	spl_console_log("--Main close--");
	return EXIT_SUCCESS;
}
void dotest() {
	int i = 0;
#ifndef UNIX_LINUX

	DWORD dwThreadId = 0;
	HANDLE hThread = 0;
	for (i = 0; i < number; ++i) {
		hThread = CreateThread(NULL, 0, win32_thread_routine, 0, 0, &dwThreadId);
	}
#else
	pthread_t idd = 0;
	for (i = 0; i < number; ++i) {
		int err = pthread_create(&idd, 0, posix_thread_routine, 0);
	}
#endif
}

//topic=sys,lib,exe,nayax,sksgn
#define spllogsys(__level__, __fmt__, ...)					spllogtopic(__level__, 0, __fmt__, ##__VA_ARGS__);
#define splloglib(__level__, __fmt__, ...)					spllogtopic(__level__, 1, __fmt__, ##__VA_ARGS__);
#define spllogexe(__level__, __fmt__, ...)					spllogtopic(__level__, 2, __fmt__, ##__VA_ARGS__);
#define spllognaxyax(__level__, __fmt__, ...)				spllogtopic(__level__, 3, __fmt__, ##__VA_ARGS__);
#define spllogsksgn(__level__, __fmt__, ...)				spllogtopic(__level__, 4, __fmt__, ##__VA_ARGS__);

#ifndef UNIX_LINUX
DWORD WINAPI win32_thread_routine(LPVOID lpParam)
#else
void* posix_thread_routine(void* lpParam) 
#endif // !UNIX_LINUX
{
	int k = 0;
	int tpic = 0;
	while (1) {
		k = get_off_process();
		if (k) {
			break;
		}
		spllog(SPL_LOG_INFO, "test log: %llu", (LLU)time(0));
		tpic = (spl_milli_now() % 3);
		spllogsys(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
		splloglib(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
		spllogexe(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
		spllognaxyax(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
		spllogsksgn(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), tpic);
		//spl_sleep(1);
	}
	return 0;
}
