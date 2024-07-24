target=libsimplelog.so
debugFlag=-g -fPIC -Wall -shared 
releaseFlag=-fPIC -Wall -shared
LIBS+=-lpthread
src=../simplelog.c ../simplelog.h ../simplelog_config.h
INC=-I../
#CFLAGS=-ansi
CFLAGS=-std=gnu89
CPPFLAGS=-std=c++98

debug:
	gcc ${debugFlag} -o ${target} ${src} ${LIBS} ${CFLAGS}
	gcc -g -o simpplelog_sample  ../c_test/main.c ${src} ${LIBS} ${INC}
	g++ -g -o simpplelog_cpp_sample  ../cpp_test/main.cpp ${LIBS} ${CPPFLAGS} ${INC} -L./ -lsimplelog
	g++ -g -o simpplelog_cpp_direct_sample  ../cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}

release:
	gcc ${releaseFlag} -o ${target} ${src} ${LIBS} ${CFLAGS}
	gcc -o simpplelog_sample  ../c_test/main.c ${src} ${LIBS} ${INC}
	g++ -o simpplelog_cpp_sample  ../cpp_test/main.cpp ${LIBS} ${INC} ${CPPFLAGS} -L./ -lsimplelog
	g++ -o simpplelog_cpp_direct_sample  ../cpp_test/main.cpp ${src} ${LIBS} ${INC} ${CPPFLAGS}

clean:
	rm -rf ${target} libsimplelog.so simpplelog_sample simpplelog_cpp_sample simpplelog_cpp_direct_sample
