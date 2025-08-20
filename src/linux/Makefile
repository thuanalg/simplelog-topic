target=libsimplelog.so
debugFlag=-g -fPIC -Wall -shared 
releaseFlag=-fPIC -Wall -shared
LIBS+=-lpthread
src=../simplelog.c
INC=-I../ -I../../include
PREFIX_SRC=../../tests
#CFLAGS=-ansi
CFLAGS=-std=gnu89 -D UNIX_LINUX -Werror=format
#CPPFLAGS=-std=c++98 
#CPPFLAGS=-std=c++98 -D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS
CPPFLAGS=-D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS__ -Wno-register -Werror=format

debug:
	gcc ${debugFlag} -o ${target} ${src} ${LIBS} ${CFLAGS} ${INC}
	gcc -g -o simple_c_example  ${PREFIX_SRC}/c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -g -o simple_cpp_example  ${PREFIX_SRC}/cpp_test/main.cpp ${LIBS} ${CPPFLAGS} ${INC} -L./ -lsimplelog
	g++ -g -o simple_cpp_example_direct  ${PREFIX_SRC}/cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}
	g++ -g -o verify_tool  ${PREFIX_SRC}/cpp_test/verify_tool.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS} --std=c++20

release:
	gcc ${releaseFlag} -o ${target} ${src} ${LIBS} ${CFLAGS} ${INC}
	gcc -o simple_c_example  ${PREFIX_SRC}/c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -o simple_cpp_example  ${PREFIX_SRC}/cpp_test/main.cpp ${LIBS} ${INC} ${CPPFLAGS} -L./ -lsimplelog
	g++ -o simple_cpp_example_direct  ${PREFIX_SRC}/cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}
	g++ -o verify_tool  ${PREFIX_SRC}/cpp_test/verify_tool.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS} --std=c++20

clean:
	rm -rf ${target} libsimple* simple_c*
