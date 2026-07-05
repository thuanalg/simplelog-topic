target=libsimplelog.so
debugFlag=-g -fPIC -Wall -shared 
releaseFlag=-fPIC -Wall -shared -s
LIBS+=-lpthread
src=../simplelog.c
src_bin_parser=../simplelog_bin_parser.c ../simplelog.c
INC=-I../ -I../../include
PREFIX_SRC=../../tests

CFLAGS=-std=c89 -D UNIX_LINUX -Wall -Werror=format -Wextra -D _GNU_SOURCE -D __LINUX__

#CFLAGS=-std=c89 -D UNIX_LINUX -Wall -Werror=format -D _GNU_SOURCE -D __LINUX__
#CFLAGS=-std=gnu89 -D UNIX_LINUX -Werror=format -Wextra
#CPPFLAGS=-std=c++98 
#CPPFLAGS=-std=c++98 -D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS

CPPFLAGS=-D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS__ -Wno-register -Werror=format -D _GNU_SOURCE -D __LINUX__

debug:
	gcc ${debugFlag} -o ${target} ${src} ${LIBS} ${CFLAGS} ${INC}
	gcc -g -o simple_c_example  ${PREFIX_SRC}/c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -g -o simple_cpp_example  ${PREFIX_SRC}/cpp_test/main.cpp ${LIBS} ${CPPFLAGS} ${INC} -L./ -lsimplelog
	g++ -g -o simple_cpp_example_direct  ${PREFIX_SRC}/cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}
	gcc -g -o simplelog_bin_parser ${src_bin_parser} ${INC} ${CFLAGS} ${debugFlag}

release:
	gcc ${releaseFlag} -o ${target} ${src} ${LIBS} ${CFLAGS} ${INC}
	gcc -o simple_c_example  ${PREFIX_SRC}/c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -o simple_cpp_example  ${PREFIX_SRC}/cpp_test/main.cpp ${LIBS} ${INC} ${CPPFLAGS} -L./ -lsimplelog
	g++ -o simple_cpp_example_direct  ${PREFIX_SRC}/cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}
	gcc -g -o simplelog_bin_parser ${src_bin_parser} ${INC} ${CFLAGS} ${releaseFlag} 

clean:
	rm -rf ${target} libsimple* simple_c* simplelog_bin_parser verify_tool
