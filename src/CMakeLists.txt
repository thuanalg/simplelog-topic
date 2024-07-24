# Add main.cpp file of the project root directory as a source file
project(simplelog_project)
set(SOURCE_LIB_FILES simplelog.c simplelog.h simplelog_config.h)
set(SOURCE_BIN_FILES c_test/main.c)
set(SOURCE_BINCPP_FILES cpp_test/main.cpp)
set(SOURCE_BINCPP_FILES_DIRECT cpp_test/main.cpp simplelog.c simplelog.h simplelog_config.h)

# Add executable target with source files listed in SOURCE_FILES variable

#add_compile_definitions(EXPORT_DLL_API_SIMPLE_LOG simpleloglib)
#add_compile_definitions(_CRT_SECURE_NO_WARNINGS simpleloglib)
add_library(simpleloglib SHARED ${SOURCE_LIB_FILES})
target_compile_definitions(simpleloglib PRIVATE EXPORT_DLL_API_SIMPLE_LOG=1)
target_compile_definitions(simpleloglib PUBLIC _CRT_SECURE_NO_WARNINGS=1)






include_directories(./)
add_executable(simple_c_example ${SOURCE_BIN_FILES})
target_compile_definitions(simple_c_example PUBLIC _CRT_SECURE_NO_WARNINGS=1)

include_directories(./)
add_executable(simple_cpp_example ${SOURCE_BINCPP_FILES})
target_compile_definitions(simple_cpp_example PUBLIC _CRT_SECURE_NO_WARNINGS=1)

include_directories(./)
add_executable(simple_cpp_example_direct ${SOURCE_BINCPP_FILES_DIRECT})
target_compile_definitions(simple_cpp_example_direct PUBLIC _CRT_SECURE_NO_WARNINGS=1)
target_compile_definitions(simple_cpp_example_direct PRIVATE __SIMPLE_STATIC_LOG__=1)

target_link_libraries(simple_c_example 
	simpleloglib
)

target_link_libraries(simple_cpp_example 
	simpleloglib
)