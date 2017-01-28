LIBUV_DIR=../libuv
LIBUV_BUILD_DIR=../libuv/out/Debug/
LIBUV_INCLUDE_DIR=$LIBUV_DIR/include

server: server.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

client: client.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

hello: helloworld.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

tick: tick.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

prepare: prepare.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

thread: thread.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

configure: configure.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

idle: idle.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

walk: walk.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

once: once.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

timer: timer.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

check: check.c libuv/libuv.la
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

libuv/libuv.la:
	$(MAKE) -C ${LIBUV_BUILD_DIR}

.PHONY: clean

clean: 
	rm -f server hello tick prepare thread configure idle walk once timer
