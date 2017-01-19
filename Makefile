LIBUV_BUILD_DIR=../libuv/out/Debug/
server: server.c libuv/libuv.la
	clang -g -o server server.c ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

hello: helloworld.c libuv/libuv.la
	clang -g -o hello helloworld.c ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

tick: tick.cc libuv/libuv.la
	clang -g -o tick tick.cc ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

prepare: prepare.cc libuv/libuv.la
	clang -g -o prepare prepare.cc ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

thread: thread.cc libuv/libuv.la
	clang -g -o thread thread.cc ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

configure: configure.cc libuv/libuv.la
	clang -g -o configure configure.cc ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

idle: idle.cc libuv/libuv.la
	clang -g -o idle idle.cc ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

walk: walk.cc libuv/libuv.la
	clang -g -o walk walk.cc ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

once: once.c libuv/libuv.la
	clang -g -o once once.c ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_BUILD_DIR}/include

libuv/libuv.la:
	$(MAKE) -C ${LIBUV_BUILD_DIR}

.PHONY: clean

clean: 
	rm -f server hello tick prepare thread configure idle walk once
