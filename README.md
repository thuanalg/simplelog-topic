### **SimpleLog-Topic: A Fast, Lightweight, and Flexible C/C++ Logger (Introduction written by ChatGPT, and I accepted.)**


Logging is an essential part of any software system, helping developers gain insights into application behavior, debug issues, and monitor production systems. However, many logging libraries often introduce unnecessary complexity and performance overhead. **SimpleLog-Topic** was designed to address these issues, offering a **fast, efficient, and easy-to-use logging solution** for both **C** and **C++** applications. 

**SimpleLog-Topic** is built to support **C89** through to **C++20**, making it a versatile choice for a wide variety of projects. Whether you're working with legacy C code or using modern C++ features, SimpleLog-Topic integrates seamlessly into your application. This logger is not just about speed—it's about **simplicity, performance**, and **flexibility**, designed with both **embedded systems** and **large-scale applications** in mind.

At its core, SimpleLog-Topic follows the **Unix Philosophy** of **KISS (Keep It Simple, Stupid)** and aims to be a tribute to the incredible work of **W. Richard Stevens**, whose **Unix Network Programming** books and contributions have profoundly influenced systems programming and network design. This tool is built with his principles of **clarity**, **elegance**, and **efficiency** in mind, ensuring that developers can achieve powerful logging without unnecessary complexity or bloat.

### **Key Features of SimpleLog-Topic**

1. **Universal Compatibility: From C89 to C++20**  
   SimpleLog-Topic is compatible with both **C89** (ANSI C) and modern **C++20**, making it an ideal logging solution for a wide range of applications, whether they are legacy C projects or cutting-edge C++ systems.

2. **Cross-Platform Support**  
   SimpleLog-Topic works across multiple platforms, including **Windows**, **Linux**, **macOS**, **Android**, and various **embedded systems**. This ensures your logging infrastructure is consistent regardless of the underlying operating system or hardware architecture.

3. **Optimized for Multithreading**  
   With **multithreaded logging support**, SimpleLog-Topic excels in high-concurrency environments. It allows multiple threads to log messages simultaneously without significant performance degradation, making it suitable for both small-scale and distributed systems.

4. **Memory-Efficient and Configurable**  
   SimpleLog-Topic is highly configurable, allowing developers to adjust memory usage according to the system’s resources. Whether running on **low-memory embedded devices** or **high-performance servers**, it can be optimized for minimal resource consumption.

5. **Nanosecond Precision for Timestamps**  
   SimpleLog-Topic provides **nanosecond-level timestamp precision**, essential for applications that require **high-precision timing**, such as **real-time systems**, **network applications**, and **performance profiling**.

6. **Superior Performance**  
   SimpleLog-Topic is engineered to be extremely fast and efficient. Its performance has been carefully benchmarked to ensure minimal overhead and **high throughput** under demanding conditions.

7. **No dependence**  
   Just use POSIX APIs and Win32 APIs.

8. **Message safety feature**  
   The **message safety feature** in your logger allows users to configure a size limit for each log message (e.g., 10,000 bytes). If a message exceeds this size, it, sometimes, **may be truncated**, but the first 10,000 bytes will still be logged. This ensures that large messages do not cause memory overflow or uncontrolled log writing, while still preserving the beginning of the message for logging. This feature is particularly useful in resource-constrained environments like embedded devices, providing both flexibility and safety in log management. [max_sz_msg=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg)
   
### **Performance Highlights: Speed and Efficiency**

One of the core objectives of SimpleLog-Topic is to provide exceptional **performance** while maintaining **simplicity**. Here’s a detailed look at its performance in different scenarios:

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

#### **Optimized for Low Latency and High Throughput**

- **Minimal Latency:**  
  SimpleLog-Topic is designed to minimize latency in logging, even when the system is under heavy load. Whether you’re dealing with **real-time systems** or large volumes of logs, SimpleLog-Topic ensures fast log writing with minimal delay. [simplelog.cfg](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg)

- **Low Memory Usage:**  
  The library is optimized to use **minimal memory**. Memory buffers are dynamically adjusted based on the available system resources, allowing SimpleLog-Topic to be used in **resource-constrained embedded systems** without consuming unnecessary memory.[simplelog.cfg](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg)

- **Asynchronous Logging Support:**  
  SimpleLog-Topic offers **asynchronous logging** capabilities, allowing logs to be written in the background without blocking the main application flow. This ensures that logging does not interfere with the critical performance of the application.

- **Log Rotation and Buffering:**  
  With configurable **log rotation** and **buffering** options, SimpleLog-Topic can handle **large volumes of log data** efficiently without causing performance degradation. You can control when log files are rotated or how much data should be kept in memory at any given time.

### **Why SimpleLog-Topic?**

- **Performance-Focused:** SimpleLog-Topic provides **fast, low-latency logging** without sacrificing efficiency, even under heavy load or in high-concurrency applications.
  
- **Flexible and Configurable:** It adapts to various system environments, from **small embedded devices** with **limited memory** to **high-performance mainframes**. You can configure buffer sizes, logging levels, and memory usage according to your system’s needs.

- **Scalable for Large-Scale Systems:** Whether you’re processing millions or billions of log records, SimpleLog-Topic scales seamlessly without compromising performance.

- **Simple and Easy to Use:** The **minimalistic API** keeps things simple, adhering to the Unix philosophy of **KISS**, making it easy to integrate and maintain.

### **A Tribute to W. Richard Stevens (1951 - 1999)**

**W. Richard Stevens** was a pioneer in the field of **network programming** and **systems design**, best known for his influential work, *Unix Network Programming*. His writing, characterized by clarity, precision, and practical insight, has shaped the way developers approach system-level programming. **SimpleLog-Topic** is inspired by Stevens' philosophy of simplicity, efficiency, and clarity.

By following the **Unix philosophy** of **KISS**, SimpleLog-Topic strives to provide developers with a straightforward, efficient logging tool that doesn’t get in the way of your application. It’s designed to be **fast**, **minimal**, and **reliable**—exactly the qualities Stevens emphasized in his work. 

This tool is a **tribute to his legacy**—a small way to express **gratitude** for the profound impact his work has had on the software development community. We honor **Richard Stevens** not only through the design of SimpleLog-Topic but also by continuing his tradition of writing tools that are **practical**, **elegant**, and **powerful**.

### **Use Cases for SimpleLog-Topic**

- **Embedded Systems**: With low memory consumption and flexible configuration options, SimpleLog-Topic is an excellent choice for **embedded devices** with limited resources.
  
- **Multithreaded and Distributed Systems**: SimpleLog-Topic handles **high-concurrency environments** efficiently, making it ideal for **multithreaded applications** and **distributed systems** where logging overhead needs to be minimized.

- **Real-Time Systems**: For **real-time applications** that require precise timestamps and low-latency logging, SimpleLog-Topic’s **nanosecond precision timestamps** are crucial for accurate tracking and performance monitoring.

### **Conclusion**

**SimpleLog-Topic** is more than just a logging library; it is a tribute to **Richard Stevens** and his dedication to clear, efficient systems programming. By following the **Unix Philosophy** of **KISS**, it provides a **fast**, **lightweight**, and **scalable** logging solution that is easy to integrate and maintain. Its **superior performance**, **multithreaded optimizations**, and **minimal resource consumption** make it an ideal choice for both **small embedded devices** and **large-scale applications**.

**SimpleLog-Topic** isn't just about logging—it's about building a better, more efficient foundation for your application, with a tool that allows you to focus on what's important: your core functionality.

[simplelog-topic](https://github.com/thuanalg/simplelog-topic/)
SimpleLog-Topic is open-source and available on GitHub. Feel free to contribute, modify, or integrate it into your projects, and be part of a community that values **simplicity**, **performance**, and the principles of **Unix**.

By using **SimpleLog-Topic**, you’re not just choosing a powerful logging solution—you’re embracing the legacy of **Richard Stevens**, honoring his work in **Unix network programming**, and following his principles of creating clear, efficient, and practical tools.

---

**APIs - Focus on Simplicity and Ease of Use**:  
  - **4 simple APIs** for usage:  
    - **Initialization** (at the start of `main`):  
	    		    - `spl_init_log_ext(SPL_INPUT_ARG *input)`  
    - **Logging** (in any thread):  
	    	    - `spllog(level, fmt, ...)`  
	    	    - **Levels**: `SPL_LOG_DEBUG`, `SPL_LOG_INFO`, `SPL_LOG_WARNING`, `SPL_LOG_ERROR`, `SPL_LOG_FATAL`  
    - **Topic-based Logging**:  
	    	    - `spllogtopic(level, topic_index, fmt, ...)`  
    - **Finalization** (at the end of `main`):  
		   - `spl_finish_log()`  
    - [Samples](https://github.com/thuanalg/simplelog-topic/tree/main/tests)
---

**Build and install with cmake**:	  
	- Install **cmake**.     
	- Come to root folder, **mkdir build && cd build**.  
	- For windows-32: **cmake .. -G "Visual Studio 17 2022" -A win32**.  
	- For windows-64: **cmake .. -G "Visual Studio 17 2022"**.  
	- For Linux: **cmake .. -DUNIX_LINUX=1**.  
	- For MAC-OSX/Apple: **cmake .. -DUNIX_LINUX=1 -DMACOSX=1**.  
	  
 

**Video Install/Demo(old way):**  
	- For [Windows 10 64bit](https://drive.google.com/file/d/1Ls4aD8fd65qcc7qgRKns2SlWw6RAhH5l/view?usp=sharing)  
	- For [VM Linux CentOs9 64bit](https://drive.google.com/file/d/1wxBjI6654wZqL7vTCZtvyiIRkJk-68HF/view?usp=sharing)  
	- For MacOSX 64bit, come to **src/mach**, and do similar steps of Linux.

---

**Thanks a lot to:**

1. My family, friends.
2. My colleagues: Lê Duy Cường, Bùi Khánh Duy, Nguyễn Công Đức , ... .
2. Jan Flik from [Intel Corp.](https://www.intel.com)

---

### Reference & Dedication:
   - [UNIX Network Programming, Volume 2: Interprocess Communications, Second Edition](https://www.amazon.com/UNIX-Network-Programming-Interprocess-Communications/dp/0130810819)
   - [Unix Network Programming: The Sockets Networking API](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551)
  
### Contact:
- [Email](mailto:nguyenthaithuanalg@gmail.com)

### Used by:
- [meta-voltumna](https://layers.openembedded.org/layerindex/branch/master/layer/meta-voltumna/) from [Elettra-Sincrotrone Trieste](https://www.elettra.eu/)
