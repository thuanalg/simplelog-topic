# Add main.cpp file of the project root directory as a source file
project(simplelog_topic_project)
set(SOURCE_LIB_FILES src/simplelog.c )
set(SOURCE_BIN_FILES tests/c_test/main.c)
set(SOURCE_BINCPP_FILES tests/cpp_test/main.cpp)
set(SOURCE_BINCPP_FILES_DIRECT tests/cpp_test/main.cpp src/simplelog.c)

# Add executable target with source files listed in SOURCE_FILES variable

#add_compile_definitions(EXPORT_DLL_API_SIMPLE_LOG simplelog)
#add_compile_definitions(_CRT_SECURE_NO_WARNINGS simplelog)


include_directories(./include)
add_library(simplelog SHARED ${SOURCE_LIB_FILES})
if(DEFINED UNIX_LINUX)
	target_compile_definitions(simplelog PUBLIC UNIX_LINUX=1)
	if(DEFINED MACOSX)
		target_compile_definitions(simplelog PUBLIC __MACH__=1)
	endif()
	set_target_properties(simplelog PROPERTIES  
		VERSION 1.0 
		SOVERSION 1 
	)
else()
	target_compile_definitions(simplelog PRIVATE EXPORT_DLL_API_SIMPLE_LOG=1)
	target_compile_definitions(simplelog PUBLIC _CRT_SECURE_NO_WARNINGS=1)
endif()





include_directories(./include)
add_executable(simple_c_example ${SOURCE_BIN_FILES})
if(DEFINED UNIX_LINUX)
	target_compile_definitions(simple_c_example PUBLIC UNIX_LINUX=1)
	if(DEFINED MACOSX)
		target_compile_definitions(simplelog PUBLIC __MACH__=1)
	endif()
else()
	target_compile_definitions(simple_c_example PUBLIC _CRT_SECURE_NO_WARNINGS=1)
endif()



include_directories(./include)
add_executable(simple_cpp_example ${SOURCE_BINCPP_FILES})
target_compile_definitions(simple_cpp_example PUBLIC _CRT_SECURE_NO_WARNINGS=1)
if(DEFINED UNIX_LINUX)
	target_compile_definitions(simple_cpp_example PUBLIC __UNIX_LINUX_CPP11_AND_NEWERS__=1 UNIX_LINUX=1)
	target_compile_definitions(simple_cpp_example PUBLIC  UNIX_LINUX=1)
	if(DEFINED MACOSX)
		target_compile_definitions(simplelog PUBLIC __MACH__=1)
	endif()
else()
	target_compile_definitions(simple_cpp_example PUBLIC _HAS_STD_BYTE=0)
endif()






include_directories(./include)
if(DEFINED UNIX_LINUX)
	#add_executable(simple_cpp_example_direct ${SOURCE_BINCPP_FILES_DIRECT})
	#target_compile_definitions(simple_cpp_example_direct PUBLIC  UNIX_LINUX=1)
	#target_compile_definitions(simple_cpp_example_direct PUBLIC __UNIX_LINUX_CPP11_AND_NEWERS__=1 )
	#target_compile_features(simplelog.c PRIVATE cxx_std_17)
	#set(CMAKE_C_COMPILER g++)
else()
	add_executable(simple_cpp_example_direct ${SOURCE_BINCPP_FILES_DIRECT})
	target_compile_definitions(simple_cpp_example_direct PRIVATE __SIMPLE_STATIC_LOG__=1)
	target_compile_definitions(simple_cpp_example_direct PUBLIC _CRT_SECURE_NO_WARNINGS=1)
	target_compile_definitions(simple_cpp_example_direct PUBLIC _HAS_STD_BYTE=0)
endif()


if(DEFINED UNIX_LINUX)
	install(TARGETS simplelog LIBRARY DESTINATION lib)
	install(FILES include/simplelog.h DESTINATION include)
endif()


target_link_libraries(simple_c_example 
	simplelog
)




target_link_libraries(simple_cpp_example 
	simplelog
)


if(DEFINED UNIX_LINUX)
target_link_libraries(simplelog 
	pthread
)
endif()

