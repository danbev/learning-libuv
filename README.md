## Learning libuv
The sole purpose of this project is to help understand how libuv works.

### Installation

#### Clone and build libuv:

    $ git clone git@github.com:libuv/libuv.git
    $ cd libuv

Next create a copy libuv HEAD:

    $ git archive --prefix="libuv/" HEAD | tar -xvfC /path/to/learning-libuv

### Building 
Build with debugging symbols:

    $ ./gyp_uv.py -f make
    $ make -C out

### Running the tests

    $ ./autoconf.sh
    $ ./configure
    $ make check

You might want to add libuv/test/.libs/run-tests to your firewall settings.
   
### Programs 

## server
This program is a simple server that will print out what the clients sends it.

    $ ./server

And in a different terminal:

    $ telnet localhost 7777


### Internals
The following are just notes taken while stepping through tick.cc.

#### uv_default_loop

    27     uv_loop_t* loop = uv_default_loop();

    (lldb) bt
    * thread #1: tid = 0x429854, 0x0000000100014279 tick`uv__kqueue_init(loop=0x0000000100018800) + 9 at kqueue.c:41, queue = 'com.apple.main-thread', stop reason = step in
      * frame #0: 0x0000000100014279 tick`uv__kqueue_init(loop=0x0000000100018800) + 9 at kqueue.c:41 [opt]
        frame #1: 0x0000000100012634 tick`uv__platform_loop_init(loop=<unavailable>) + 20 at darwin.c:43 [opt]
        frame #2: 0x0000000100009920 tick`uv_loop_init(loop=0x0000000100018800) + 336 at loop.c:62 [opt]
        frame #3: 0x0000000100004121 tick`uv_default_loop + 33 at uv-common.c:589 [opt]
        frame #4: 0x0000000100001167 tick`main + 23 at tick.cc:27
        frame #5: 0x00007fff92f135ad libdyld.dylib`start + 1

Taking a look at kqueue.c:41 we find:

       40 int uv__kqueue_init(uv_loop_t* loop) {
    -> 41   loop->backend_fd = kqueue();
       42   if (loop->backend_fd == -1)
       43     return -errno;
       44 

We can see that libuv is calling kqueue which creates a new queue.
I'm not sure what loop->backend_fd represents yet but this will be set to the file descriptor 
returned by kqueue. ioctl is later used to close this file descriptor on exec before returning 
to the caller. More about this below

uv__closeexec:
r = ioctl(fd, set ? FIOCLEX : FIONCLEX);
FIOCLEX  = File IOctl CLose On Exec
Sets the close-on-exec flag which causes the file descriptor to be closed when the calling 
process executes a new program.
If an exec call is made the current process image will be replaced with a new one but the file descriptors
will be left open. The above call will cause the file descriptors to be close in such a case and this is 
something that is recommended for library implementors. 

This will leave us back in loop.c and the line after uv__platform_loop_init(loop)

Next, a call to uv_signal_init_loop->child_watcher is performed.

#### uv_run_loop

