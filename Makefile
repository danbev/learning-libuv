LIBUV_BUILD_DIR=./libuv/out/Debug/
server: server.c libuv/libuv.la
	clang -g -o server server.c ${LIBUV_BUILD_DIR}/libuv.a -Ilibuv/include

hello: helloworld.c libuv/libuv.la
	clang -g -o hello helloworld.c ${LIBUV_BUILD_DIR}/libuv.a -Ilibuv/include

tick: tick.cc libuv/libuv.la
	clang -g -o tick tick.cc ${LIBUV_BUILD_DIR}/libuv.a -Ilibuv/include

prepare: prepare.cc libuv/libuv.la
	clang -g -o prepare prepare.cc ${LIBUV_BUILD_DIR}/libuv.a -Ilibuv/include

thread: thread.cc libuv/libuv.la
	clang -g -o thread thread.cc ${LIBUV_BUILD_DIR}/libuv.a -Ilibuv/include

configure: configure.cc libuv/libuv.la
	clang -g -o configure configure.cc ${LIBUV_BUILD_DIR}/libuv.a -Ilibuv/include

libuv/libuv.la:
	$(MAKE) -C libuv

.PHONY: clean

clean: 
	rm -f server hello tick prepare thread configure
