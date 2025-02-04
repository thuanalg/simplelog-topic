target=libsimplelog.dylib
debugFlag=-g -fPIC -Wall -shared 
releaseFlag=-fPIC -Wall -shared
LIBS+=-lpthread
src=../simplelog.c
INC=-I../
#CFLAGS=-ansi
CFLAGS=-std=gnu89 -D UNIX_LINUX -D __MACH__
#CPPFLAGS=-std=c++98 
#CPPFLAGS=-std=c++98 -D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS
CPPFLAGS=-D UNIX_LINUX -D __UNIX_LINUX_CPP11_AND_NEWERS__ -D __MACH__

debug:
	gcc ${debugFlag} -o ${target} ${src} ${LIBS} ${CFLAGS}
	gcc -g -o simplelog_sample  ../c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -g -o simplelog_cpp_sample  ../cpp_test/main.cpp ${LIBS} ${CPPFLAGS} ${INC} -L./ -lsimplelog
	g++ -g -o simplelog_cpp_direct_sample  ../cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}

release:
	gcc ${releaseFlag} -o ${target} ${src} ${LIBS} ${CFLAGS}
	gcc -o simplelog_sample  ../c_test/main.c ${src} ${LIBS} ${INC} ${CFLAGS}
	g++ -o simplelog_cpp_sample  ../cpp_test/main.cpp ${LIBS} ${INC} ${CPPFLAGS} -L./ -lsimplelog
	g++ -o simplelog_cpp_direct_sample  ../cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}

clean:
	rm -rf ${target}  simplelog_sample simplelog_cpp_sample simplelog_cpp_direct_sample
