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