#### **Benchmarking Performance**

1. **Windows10/MSVC 2022 with 10 Threads and 10 Million Records:**

   - [SimpleLog-Topic: 16 seconds](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/MSVC-2022-performance.txt)
   - **spdlog:** **66 seconds**

   In this test, **SimpleLog-Topic** logged **10 million records** in just **16 seconds**, compared to **66 seconds** taken by **spdlog**. This demonstrates **4x faster performance** in multithreaded environments, showing that SimpleLog-Topic efficiently manages logging tasks without unnecessary performance bottlenecks.

2. **Linux Performance Benchmark (10 Threads and 10 Million Records), VM CentOs9/PC:**  
	
	**With CPU-3 Cores / VM:**
   - **SimpleLog-Topic:** **15-16 seconds**
   - [spdlog: 15 seconds](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/LinuxCentOs09-performance.txt)  
	
	**With CPU-5 Cores / VM:**	
	- [SimpleLog-Topic: ~13 seconds](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/LinuxCentOs09-performance.txt)
	- **spdlog: ~17 seconds**  
   
	**With CPU-8 Cores / PC:**	
	- [SimpleLog-Topic: ~3.331 seconds](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/CentOS-09-performance-8-Core.txt) Here, **simplelog-topic** is faster **2x** spdlog. Speed: **3 million records a second**. simplelog-topic is faster 2x spdlog.
	- **spdlog: ~7.051 seconds**. Speed: **1.4 million records a second**.
   
   On **Linux** is faster than Windows. If **number of CPUs** are more, then SimpleLog-Topic becomes more effective.

3. **Large-Scale Logging Test (1 Billion Records, 10 Threads):** 
	
	**Windows 10 - CPU 8 Cores** 
   - **Time Taken:** **36 minutes and 3 seconds** (**2163 seconds**)
   - [Log Size: 113 GB or refer to another case](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/One_Billion_records-performance.txt)

	**CentOs 09 - CPU 8 Cores**
   - **Time Taken:** **15 minutes** (**900 seconds**)
   - [Log Size:107GB, 114888893030 bytes](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/CentOS-09-performance-8-Core-1Billion.txt)

   In an extreme scenario, SimpleLog-Topic processed **1 billion records** across **10 threads**, generating **113 GB of log data** in just **36 minutes**. This performance indicates that SimpleLog-Topic can scale to handle **massive volumes** of log data, making it suitable for both **high-frequency** and **large-scale logging** applications.

4. **OSX/Apple clang version 11.0.3 (clang-1103.0.32.29) with 10 Threads and 10 Million Records ( Total Number of Cores: 2, Model Name: Mac mini(Late 2012)):**

   - **SimpleLog-Topic: 37.1 seconds** 
   - [spdlog: 26 seconds](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/MacOSX-performance-2-Core.txt) 

   In this environment **spdlog** is faster at **11.1s (~43%)** than **simplelog-topic**. However, I shall re-verify with PC with more CPU cores.