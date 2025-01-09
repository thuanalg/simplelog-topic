#include "simplelog.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifndef UNIX_LINUX

#include <Windows.h>
	DWORD WINAPI win32_thread_routine(LPVOID lpParam);
#else
	#include <unistd.h>
	#include <pthread.h>
	void* posix_thread_routine(void* lpParam);
#endif // !UNIX_LINUX

void dotest();
int num_threads = 10;
int loop_count = 1000 * 1000;

#define		TNUMBEER_OF_THREADS					"--nthread="	
#define		TCONFIG_FILE						"--cfg="	
#define		TLOOP_COUNT							"--loopcount="	

int main(int argc, char* argv[]) {
	int ret = 0, i = 0;
	char cfgpath[1024];
	for (i = 1; i < argc; ++i) {
		if (strstr(argv[i], TNUMBEER_OF_THREADS) == argv[i]) {
			ret = sscanf(argv[i], TNUMBEER_OF_THREADS"%d", &num_threads);
			continue;
		}
		if (strstr(argv[i], TLOOP_COUNT) == argv[i]) {
			ret = sscanf(argv[i], TLOOP_COUNT"%d", &loop_count);
			continue;
		}
	}
#ifndef UNIX_LINUX
	snprintf(cfgpath, 1024, "C:/z/simplelog-topic/win64/Debug/simplelog.cfg");
#else
	snprintf(cfgpath, 1024, "simplelog.cfg");
#endif
	ret = spl_init_log(cfgpath);

	spl_console_log("====================Start.\n");
	dotest();
	spl_console_log("==================End.\n");
	spl_finish_log();
	return EXIT_SUCCESS;
}
void dotest() {
	int i = 0;
#ifndef UNIX_LINUX

	DWORD *dwpThreadId = 0, dwEvent = 0;
	HANDLE *hpThread = 0;

	dwpThreadId = (DWORD*)malloc(num_threads * sizeof(DWORD));
	if (!dwpThreadId) {
		exit(1);
	}
	memset(dwpThreadId, 0, num_threads * sizeof(DWORD));

	hpThread = (HANDLE*)malloc(num_threads * sizeof(HANDLE));
	if (!hpThread) {
		exit(1);
	}
	memset(hpThread, 0, num_threads * sizeof(HANDLE));
	
	for (i = 0; i < num_threads; ++i) {
		hpThread[i] = CreateThread(NULL, 0, win32_thread_routine, 0, 0, (dwpThreadId + i));
	}
	//https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects
	//https://learn.microsoft.com/en-us/windows/win32/sync/waiting-for-multiple-objects
	dwEvent = WaitForMultipleObjects(
			num_threads,           // number of objects in array
			hpThread,				// array of objects
			TRUE,					// wait for any object
			INFINITE);				// five-second wait
	free(dwpThreadId);
	free(hpThread);
#else
	//https://man7.org/linux/man-pages/man3/pthread_create.3.html
	pthread_t *pidds = 0;
	pidds = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
	if(!pidds) {
		exit(1);
	}
	for (i = 0; i < num_threads; ++i) {
		int err = pthread_create(pidds + i, 0, posix_thread_routine, 0);
	}
	for (int i = 0; i < num_threads; i++) {
		int s = pthread_join(pidds[i], 0);
		if (s != 0) {
			spl_console_log("pthread_join error.\n");
		}
	}
	free(pidds);

#endif
}

//topic=sys,lib,exe,nayax,sksgn
#define spllogsys(__level__, __fmt__, ...)					spllogtopic(__level__, 0, __fmt__, ##__VA_ARGS__);
#define splloglib(__level__, __fmt__, ...)					spllogtopic(__level__, 1, __fmt__, ##__VA_ARGS__);
#define spllogexe(__level__, __fmt__, ...)					spllogtopic(__level__, 2, __fmt__, ##__VA_ARGS__);
#define spllognaxyax(__level__, __fmt__, ...)				spllogtopic(__level__, 3, __fmt__, ##__VA_ARGS__);
#define spllogsksgn(__level__, __fmt__, ...)				spllogtopic(__level__, 4, __fmt__, ##__VA_ARGS__);
//https://github.com/gabime/spdlog, 10 thread
#ifndef UNIX_LINUX
DWORD WINAPI win32_thread_routine(LPVOID lpParam) {
#else
void* posix_thread_routine(void* lpParam) {
#endif // !UNIX_LINUX
	int k = 0;
	int tpic = 0;
	//while (1) {
		int count = 0;
		
		while (count < loop_count) {
			//spllog(SPL_LOG_INFO, "test log: %d", count);
			spllog(SPL_LOG_INFO, "test log test log test log: %d", count);
			//spllogsys(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "sys");
			//splloglib(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "lib");
			//spllogexe(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "exe");
			//spllognaxyax(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "nayax");
			//spllogsksgn(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "sksg");
			++count;
		}
		//spl_console_log("Main close: End.\n");
		//break;
	//}
	return 0;
}

int main___(int argc, char* argv[]) {
	int ret = 0;
	SPL_INPUT_ARG input;
	int count = 2;
	memset(&input, 0, sizeof(input));
	snprintf(input.id_name, SPL_IDD_NAME, "testlog");
	//int ret = spl_init_log((char *)"C:/z/simplelog-topic/win64/Debug/simplelog.cfg");
#ifndef UNIX_LINUX
	//ret = spl_init_log((char*)"C:/z/simplelog-topic/win64/Debug/simplelog.cfg");
	snprintf(input.folder, SPL_PATH_FOLDER, "C:/z/simplelog-topic/win64/Debug/simplelog.cfg");
#else
	//ret = spl_init_log((char*)"simplelog.cfg");
	snprintf(input.folder, SPL_PATH_FOLDER, "simplelog.cfg");
#endif
	ret = spl_init_log_ext(&input);
	
	//spl_milli_sleep(1000 * 5);
	for (int i = 0; i < count; ++i) {
		//spl_console_log("spl_milli_sleep ------------------------------ ");
		//spllogsys(SPL_LOG_INFO, "test log: %llu, topic: %d.", (LLU)time(0), 0);
		spllog(SPL_LOG_INFO, "test log test log test log test log %d", i);
		spllogsys(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "sys");
		splloglib(SPL_LOG_INFO, "test log: %llu, topic: %s", time(0), "lib");
		spllogexe(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "exe");
		spllognaxyax(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "nayax");
		spllogsksgn(SPL_LOG_INFO, "test log: %llu, topic: %s.", (LLU)time(0), "sksg");
	}
	//spl_milli_sleep( 1000 * 100);
	spl_finish_log();
	//spl_milli_sleep(100 * 1000);
	return 0;
}
//#include <stdio.h>
//
//int main____() {
//	//LARGE_INTEGER frequency;
//	LARGE_INTEGER counter;
//
//	// Query the performance frequency (ticks per second)
//	//if (QueryPerformanceFrequency(&frequency)) {
//		// Get the current value of the performance counter
//	for (int i = 0; i < 10; ++i) {
//		QueryPerformanceCounter(&counter);
//
//		// Convert ticks to nanoseconds
//		// frequency.QuadPart gives the number of ticks per second
//		// counter.QuadPart gives the number of ticks that have passed since the system was booted
//		// To get nanoseconds, we calculate (counter * 10^9) / frequency
//		short nanoseconds = (counter.QuadPart & 0xFF );
//#ifdef DEBUG_MODE_0
//		printf("debug Current time in nanoseconds: %d\n", nanoseconds % 8);
//		//open trigger socket, subprogram
//		//socket(UDP, 20000);
//#else
//		printf("Current time in nanoseconds: %d\n", nanoseconds % 7);
//#endif
//		
//	}
//
//
//	return 0;
//}
//int quick_sort(int* arr, int n);
//int validate_quick_sort(int* arr, int n);
//int main__1__() {
//	LARGE_INTEGER counter;
//	int arr[10];
//	////......
//	//arr = (int*)malloc(sizeof(int) * n);
//	//if (!arr) {
//	//	exit(1);
//	//}
//	for (int i = 0; i < 10; i++)
//	{
//		int tmp = 0;
//		QueryPerformanceCounter(&counter);
//		tmp = (counter.QuadPart & 0xFF)%7;
//		arr[i] = tmp;
//	}
//	quick_sort(arr, 10);
//#ifdef VALIDATE
//	int ret = validate_quick_sort(arr, 10);
//	if (ret) {
//		fprintf(stderr, "sort err at: %d\n", ret - 1);
//	}
//#endif
//	return 0;
//}

int quick_sort(int* arr, int n) {
	return 0;
}

int validate_quick_sort(int* arr, int n) {
	int ret = 0;
	for (int i = 0; i < n - 1; ++i) {
		if (arr[i] > arr[i + 1]) {
			ret = i + 1;
			break;
		}
	}
	spl_console_log("jhgfjgf");
	return ret;
}