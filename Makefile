server: server.c libuv/libuv.la
	clang -g -o server server.c libuv/.libs/libuv.a -Ilibuv/include

hello: helloworld.c uv/libuv.la
	clang -g -o hello helloworld.c libuv/.libs/libuv.a -Ilibuv/include

libuv/libuv.la:
	$(MAKE) -C libuv

.PHONY: clean

clean: 
	rm -f server hello
