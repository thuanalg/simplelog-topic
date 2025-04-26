# SimpleLog-Topic

**High-Performance, Cross-Platform Logging Library for C/C++**



Author: [Thuan Nguyen Thai](mailto:nguyenthaithuanalg@gmail.com)  
Other contributors: None  
Created: April 25, 2025  


  
  

  
### Problem Statement  
- Providing a logger which is simple-STABLE-powerful for embedded devices to PC and mainframe with flexible configuration.  
- Multi-thread C/C++ logger, portable.  
- A meaningful demo for Systems Programming inspired by W. Richard Stevens(1951-1999).  

  
  
### Requirements
- POSIX thread/POSIX APIs in UNIX-Like.  
- Win32 APIs in Windows.  

### Background and References  
- No.  


  
### 🧩 Proposed Design


| Component | Description |
|-----------|-------------|
| `Log folder` | **[Mandatory]** [pathfolder=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg)|
| `Daily` | **[Mandatory]** |
| `Log rotation` | **[Mandatory]** [rotation_size=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg)|
| `Time` | **[Mandatory]** `[yyyy-mm-dd hh-mm-ss.nano]`|
| `Info` | **[Mandatory]** `[tid] [pid] [file:func:line]`|
| `Number of CPU` | **[Mandatory]** [ncpu=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg)|
| `topic` | **[Optional]** [topic=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg), can comment this feature.|
| `Log level` | **[Mandatory]** [level=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg).|
| `Buffer size` | **[Mandatory]** [buffsize=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg).|
| `Timer trigger` | **[Mandatory]** [trigger=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg).|
| `Safe size of message` | **[Mandatory]** [max_sz_msg=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg).|
| `Shared memory key` | **[Optional]** [shared_key=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg).|
  
  
### 🛠️ Exported APIs

SimpleLog-Challenge provides 5 key APIs for initializing, logging, and process control:

| APIs | Description |
|--------------------|-------------|
| `int spl_init_log_ext(SPC_INPUT_ARG *input);` | **[Mandatory]** Initialize the logger with extended input settings. |
| `int spl_finish_log();` | **[Mandatory]** Finalize and clean up the logger. |
| `spllog(log_level, fmt, ...);` | **[Mandatory]** Log a message with the specified level. |
| `spllogtopic(log_level, topic_index, fmt, ...);` | **[Optional]** Log by [topics=](https://github.com/thuanalg/simplelog-topic/blob/main/src/simplelog.cfg) from 1 to n. |


### Alternatives Considered
- No. It should be one more option. It should NOT alternate any libraries, but is really worthy to try with new applications.

### Impact
- No.  

### Organizational
- Does this proposal require a new repository? No.  
- Which repositories are expected to be modified to execute this design? `phosphor-logging`  

### Testing and Validation
- [Benchmarks on Windows, Linux, macOS vs spdlog](https://github.com/thuanalg/simplelog-topic/blob/main/README.md#benchmarking-performance)
- [1B record stress test Windows](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/One_Billion_records-performance.txt)
- [1B record stress test Linux](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/CentOS-09-performance-8-Core-1Billion.txt)
- However, you can try with [Benchmarking Linux 8 cores](https://github.com/thuanalg/simplelog-topic/blob/main/x-test-result/CentOS-09-performance-8-Core.txt) first, and find [How to run](https://github.com/thuanalg/simplelog-topic/blob/main/src/linux/xrun.sh) . And do same with Windows/MAC OSX.  

### References
- [UNIX Network Programming, Volume 2: Interprocess Communications, Second Edition](https://www.amazon.com/UNIX-Network-Programming-Interprocess-Communications/dp/0130810819)  
- [Unix Network Programming: The Sockets Networking API](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551) 

---

