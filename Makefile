LIBUV_DIR=../libuv
LIBUV_BUILD_DIR=$(LIBUV_DIR)/build
LIBUV_INCLUDE_DIR=$(LIBUV_DIR)/include
LIBUV_AR=$(LIBUV_BUILD_DIR)/libuv_a.a

server: server.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR}

client: client.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR}

hello: helloworld.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR}

tick: tick.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR}

prepare: prepare.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR}

thread: thread.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

configure: configure.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

idle: idle.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

walk: walk.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

once: once.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

timer: timer.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

check: check.c $(LIBUV_AR)
	g++ -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR} -lpthread -ldl

queue: queue.c $(LIBUV_AR)
	gcc -Wall -g -O0 -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR} -lpthread -ldl

tty: tty.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

udp-server: udp-server.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

udp-client: udp-client.c $(LIBUV_AR)
	clang -g -o $@ $< ${LIBUV_BUILD_DIR}/libuv.a -I${LIBUV_INCLUDE_DIR}

work: work.c $(LIBUV_AR)
	g++ -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR} -lpthread -ldl

addressinfo: addressinfo.c $(LIBUV_AR)
	${CC} -g -o $@ $< ${LIBUV_AR} -I${LIBUV_INCLUDE_DIR} -lpthread -ldl

$(LIBUV_AR):
	$(MAKE) -C ${LIBUV_BUILD_DIR}

.PHONY: clean

clean: 
	rm -f server hello tick prepare thread configure idle walk once timer
