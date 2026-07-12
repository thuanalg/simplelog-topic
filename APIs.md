#### **APIs - Focus on Simplicity and Ease of Use**:  
  - **5 simple APIs** for usage:  
    - **Initialization** (at the start of `main`):  
	    		    - `spl_init_log_ext(SPL_INPUT_ARG *input)`  
    - **Logging** (in any thread):  
	    	    - `spllog(level, fmt, ...)`  
	    	    - **Levels**: `SPL_LOG_DEBUG`, `SPL_LOG_INFO`, `SPL_LOG_WARNING`, `SPL_LOG_ERROR`, `SPL_LOG_FATAL`  
    - **Topic-based Logging** (in any thread):  
	    	    - `spllogtopic(level, topic_index, fmt, ...)`  
    - **Bin Topic-based Logging** (in any thread):  
	    	    - `spllogbintopic(level, topic_index, type_id, data, size)`              
    - **Finalization** (at the end of `main`):  
		   - `spl_finish_log()`  
    - [Samples](https://github.com/thuanalg/simplelog-topic/tree/main/tests)
  