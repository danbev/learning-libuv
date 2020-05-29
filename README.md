## Learning libuv
The sole purpose of this project is to help understand how libuv works.

### Installation

#### Clone and build libuv:
```console
    $ git clone git@github.com:libuv/libuv.git
    $ cd libuv
```

### Building libuv
Build with debugging symbols:
```console
    $ mkdir build && cd build
    $ cmake -DCMAKE_BUILD_TYPE=Debug ..
    $ make
```

### Running the tests
```console
    $ make test
```

You might want to add libuv/test/.libs/run-tests to your firewall settings.

### Introduction
Libuv provides the following features:
* Event loop
* Timers
* TCP/UDP sockets
* Named pipes
* Filesystem operations
* Signal handling
* Child processes
* TTY
* Threading utilities

Libuv uses the best available options for different operating systems to perform
operations related to process, signal, timer and file descriptors. On Linux
epoll is used and on MacOSX kqueue, and on Windows GetQueuedCompletionStatusEx.

### Inheritance in C
Libuv uses structs to implement inheritance. Here is an non-libuv [example](https://github.com/danbev/learning-c/blob/master/inherit.c).
For example, in libuv we have structs that inherit from uv_stream_t, like uv_tcp_t, uv_pipe_t:
```c
struct uv_stream_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
};

struct uv_tcp_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
  UV_TCP_PRIVATE_FIELDS
};

struct uv_pipe_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
  int ipc; /* non-zero if this pipe is used for passing handles */
  UV_PIPE_PRIVATE_FIELDS
};
```

### uv_loop_t 
This type is the main object where things happen. It runs in a single thread. If
you want more threads you would run one instance of this type per thread.

### uv_handle_t
This type represents a resource. For example, a timer handle would be reponsible
for calling a callback when the timer expires. Or a tcp handle would be
responsible for reading/writing.

### uv_request_t
This type represents operations. These request can use handles, for example a
request to write to a stream would use a stream handle to do so. But there does
not have to be a handle.

### init

```c
  uv_loop_t* loop = uv_default_loop();
  uv_loop_init(loop);
```
In `src/unix/loop.c` we have:
```c
int uv_loop_init(uv_loop_t* loop) {
  ...
  QUEUE_INIT(&loop->wq);
  ...
}
```
Notice that the address of loop->wq is being passed into the macro (src/queue.h)
as its sole argument. And `wq` is defined as:
```c
void* wq[2];
```
So `wq` is an array with two elements which are of type pointer to void. So,
they can contain any pointer but will need to be casted to the correct type when
used.
```c
#define QUEUE_INIT(q)                                                         \
  do {
    QUEUE_NEXT(q) = (q);
    QUEUE_PREV(q) = (q);
  }
```
So `QUEUE_NEXT` would be expanded to:
```c
QUEUE_NEXT(&loop->wq) = (&loop->wq);
```
And `QUEUE_NEXT` looks like this:
```c
#define QUEUE_NEXT(q)               (*(QUEUE **) &((*(q))[0]))
```
And if we replace q with the passed in argument `&loop->wp` we get:
```c
#define QUEUE_NEXT(&loop->wp)       (*(QUEUE **) &((*(&loop->wp))[0]))
```
So the first line of the `QUEUE_INIT` macro will become:
```c
    (*(QUEUE **) &((*(&loop->wq))[0])) = &loop->wp;
```
First, the we dereferece the passed-in `&loop->wp` which gives the address of
the array.
```c
    (*(QUEUE **) &(loop->wq[0])) = &loop->wp;
```
Now, `&(loop->wq[0]) is the address of first member of the array and this
will be of type `void*`, and we are getting the address of that:
```c
  auto first = &((*(&s))[0]);
(lldb) expr first
(void ** ) $0 = 0x00007fffffffd1d0
```
We are then casting that to QUEUE** and then dereferencing.
I'm not understading the reason for this cast. If the `first` above is already
of type void** the additional task does nothing. I'm probably missing something
here, but there is an example in [queue.c](./queue.c) that tries to sort
this out.

```c
  QUEUE q = {NULL, NULL};
  QUEUE_INIT(&q);
```
```console
(lldb) expr q
(QUEUE) $0 = ([0] = 0x0000000000000000, [1] = 0x0000000000000000)
(lldb) s
(lldb) expr q
(QUEUE) $1 = ([0] = 0x00007fffffffd190, [1] = 0x00007fffffffd190)
(lldb) expr &q
(QUEUE *) $2 = 0x00007fffffffd190
```
Notice that both entries are set to the address of of `q`.
Next, we insert two elements into the queue:
```c
  QUEUE_INSERT_TAIL(&q, &fletch.wp);
  QUEUE_INSERT_TAIL(&q, &rosen.wp);
```
```console
(lldb) expr q
(QUEUE) $4 = ([0] = 0x00007fffffffd180, [1] = 0x00007fffffffd180)
(lldb) s
(lldb) expr q
(QUEUE) $5 = ([0] = 0x00007fffffffd180, [1] = 0x00007fffffffd160)
(lldb) expr &fletch.wp
(void *(*)[2]) $9 = 0x00007fffffffd180
(lldb) expr &rosen.wp
(void *(*)[2]) $10 = 0x00007fffffffd160
```
Notice that the struct has a queue memember (or in this cast just a member
of type void* wt[2].


To initialize a handle there is a common pattern. For example, if we look
at the [check](./check.c) example we find:
```c
  uv_loop_t* loop = uv_default_loop();
  uv_loop_init(loop);
  uv_check_t check;
  uv_check_init(loop, &check);
```
What does `uv_check_init` actually do?  
We have to look in `src/unix/loop-watcher.c
```c
UV_LOOP_WATCHER_DEFINE(check, CHECK)
```
Now, the `UV_LOOP_WATCHER_DEFINE` macro is defined in the same file:
```c
int uv_check_init(uv_loop_t* loop, uv_check_t* handle) {
    uv__handle_init(loop, (uv_handle_t*)handle, UV_CHECK);
    handle->check_cb = NULL;
    return 0;
}
```
So that looks simple enough, but we need to keep in mind that `uv__handle_init`
is also a macro which is defined in `src/uv-common.h`.
```c
#define uv__handle_init(loop_, h, type_)                                      \
  do {                                                                        \
    (h)->loop = (loop_);                                                      \
    (h)->type = (type_);                                                      \
    (h)->flags = UV_HANDLE_REF;  /* Ref the loop when active. */              \
    QUEUE_INSERT_TAIL(&(loop_)->handle_queue, &(h)->handle_queue);            \
    uv__handle_platform_init(h);
  }
  while (0)
```
And `QUEUE_INSERT_TAIL` is also a macro (can be found in `src/queue.h`):
```c
#define QUEUE_INSERT_TAIL(h, q)                                               \
  do {                                                                        \
    QUEUE_NEXT(q) = (h);                                                      \
    QUEUE_PREV(q) = QUEUE_PREV(h);                                            \
    QUEUE_PREV_NEXT(q) = (q);                                                 \
    QUEUE_PREV(h) = (q);                                                      \
  }                                                                           \
  while (0)
```
Notice that `h` is the *(loop_)->handle_queue, and that `q` is the &(h)->handle_queue.

Lets first take a look at `QUEUE_NEXT(h)` and see what it looks like:
```c
#define QUEUE_NEXT(q)       (*(QUEUE **) &((*(q))[0])) = (h);
```
Which will expand to:
```c
(*(QUEUE **) &((*(&((uv_handle_t*)handle)->handle_queue))[0])) = (&(loop)->handle_queue);

(*(QUEUE **) &(( value_of_handle_queue[0] ))) = (&(loop)->handle_queue);
(*(QUEUE **) &(value_of_handle_queue[0])) = (&(loop)->handle_queue);
(*(QUEUE **) address_value_of_handle_queue[0]) = (&(loop)->handle_queue);
(* (QUEUE **) address_value_of_handle_queue[0]) = (&(loop)->handle_queue);
(defref value as (QUEUE **) = (&(loop)->handle_queue);
```
```c
  (uv_handle_t*)handle)->handle_queue
```
The cast is there because the what is passed to the macro is:
```c
    uv__handle_init(loop, (uv_handle_t*)handle, UV_CHECK);
```
And the the preprocessor just does a substition so call usages of the `h` parameter
will be (uv_handle_t*)handle. And it then dereferences the `handle_queue`. 
The handle_queue is defined in 'UV_HANDLE_FIELDS` as:
```c
  void* handle_queue[2];
```
TODO: add details about the queue and that they are members of a struct...
Next,
```c
  &((uv_handle_t*)handle)->handle_queue))[0])
  &((*(&((uv_handle_t*)handle)->handle_queue))[0])
```

We can see the complete function expanded by the preprocesor using the following
command:
```console
$ gcc -I./include -I./src -E src/unix/loop-watcher.c
``` 
```c
int uv_check_init(uv_loop_t* loop, uv_check_t* handle) {
  do {
    ((uv_handle_t*)handle)->loop = (loop);
    ((uv_handle_t*)handle)->type = (UV_CHECK);
    ((uv_handle_t*)handle)->flags = UV_HANDLE_REF;
    do {
      (*(QUEUE **) &((*(&((uv_handle_t*)handle)->handle_queue))[0])) = (&(loop)->handle_queue);
      (*(QUEUE **) &((*(&((uv_handle_t*)handle)->handle_queue))[1])) = (*(QUEUE **) &((*(&(loop)->handle_queue))[1]));
      ((*(QUEUE **) &((*((*(QUEUE **) &((*(&((uv_handle_t*)handle)->handle_queue))[1]))))[0]))) = (&((uv_handle_t*)handle)->handle_queue);
      (*(QUEUE **) &((*(&(loop)->handle_queue))[1])) = (&((uv_handle_t*)handle)->handle_queue);
    } while (0);
    (((uv_handle_t*)handle)->next_closing = ((void *)0);
  } while (0);
  handle->check_cb = ((void *)0);
  return 0;
}
```

```c
struct uv_loop_s {
  /* User data - use this for whatever. */
  void* data;
  /* Loop reference counting. */
  unsigned int active_handles;
  void* handle_queue[2];
  union {
    void* unused[2];
    unsigned int count;
  } active_reqs;
  /* Internal flag to signal loop stop. */
  unsigned int stop_flag;
  UV_LOOP_PRIVATE_FIELDS
};

#define UV_HANDLE_FIELDS                                                      \
  /* public */                                                                \
  void* data;                                                                 \
  /* read-only */                                                             \
  uv_loop_t* loop;                                                            \
  uv_handle_type type;                                                        \
  /* private */                                                               \
  uv_close_cb close_cb;                                                       \
  void* handle_queue[2];                                                      \
  union {                                                                     \
    int fd;                                                                   \
    void* reserved[4];                                                        \
  } u;                                                                        \
  UV_HANDLE_PRIVATE_FIELDS
```

### Network I/O
```
+------------+ +----------+ +-----------+         
|  uv_tcp_t  | | uv_pipe_t| | uv_tty_t  |
+------------+ +----------+ +-----------+
                  /|\
+---------------------------------------+ +----------+ +-----------+
|            uv_stream_t                | | uv_udp_t | | uv_poll_t |
+---------------------------------------+ +----------+ +-----------+
+------------------------------------------------------------------+
|                  uv__io_t                                        |
+------------------------------------------------------------------+
+------------------------------------------------------------------+
|               Operating System                                   |
+------------------------------------------------------------------+
```

`uv_tcp_t`, `uv_pipe_t`, `uv_tty_t`, `uv_udp_t`, and `uv_poll_t` is what an
application uses.

`uv_tcp_t`, `uv_pipe_t`, `uv_tty_t` are all streams. The common functionality
for these can be found in `src/unix/stream.c`.

### File I/0
```
+------------+ +----------+ +-----------------+ +------------------+        
|  uv_fs_t   | | uv_work_t| | uv_getaddrinfo_t| | uv_getnameinfo_t |
+------------+ +----------+ +-----------------+ +------------------+
+------------------------------------------------------------------+
|                  uv_work_t                                       |
+------------------------------------------------------------------+
+------------------------------------------------------------------+
|                  thread pool                                     |
+------------------------------------------------------------------+
```

`uv_fs_t`, `uv_work_t`, `uv_getaddrsinfo_t`, `uv_getnameinfo_t` is what an
application uses.  These all run on a thread pool to perform there operations.
The thread pool is only used for file I/0 and for getaddrinfo!
The default size of the thread pool is 4 (uv_threadpool_size). 
Just to be clear, the libuv Event Loop is single threaded. The thread pool is
used for file I/O operations.

http://blog.libtorrent.org/2012/10/asynchronous-disk-io/


#### Kqueue (Kernel Queues)
Kqueue is a mechanism for registering and responding to process, signal, timer,
and file descriptor events in the kernel.

To see how libuv utilizes kqueue you can look in `src/unix/darwin.c`


### Thread pool
What uses the thread pool?
* files
* dns.lookup() and getaddrinfo
Uses the system resolver library and makes blocking socket calls.


### The event loop

For macosx the event loop source can be found in `src/unix/core.c`
```c
    uv__update_time(loop);
    uv__run_timers(loop);
    uv__run_pending(loop); //pending handlers are retuned
    uv__run_idle(loop); // uv_idle are run at this stage
    uv__run_prepare(loop); // uv_prepare are run at this stage

    uv__io_poll(loop, timeout);
    uv__run_check(loop);
    uv__run_closing_handles(loop);
```
   
### Programs 

## server
This program is a simple server that will print out what the clients sends it.
```console
    $ ./server
```

And in a different terminal:
```console
    $ telnet localhost 7777
```

## uv_idle and uv_check
uv_checks are performed after polling for I/O.

[idle.cc](./idle.cc) shows and example of using uv_idle. 
```console
    $ lldb idle
    (lldb) br s -f idle.c -l 14
```

### Internals
The following are just notes taken while stepping through tick.cc.

### uv_loop_t
```c
    struct uv_loop_s {
      /* User data - use this for whatever. */
      void* data;
      /* Loop reference counting. */
      unsigned int active_handles;
      void* handle_queue[2];
      void* active_reqs[2];
      /* Internal flag to signal loop stop. */
      unsigned int stop_flag;
      UV_LOOP_PRIVATE_FIELDS
```
};

UV_LOOP_PRIVATE_FIELDS can be found in include/uv-unix.h.

#### uv_default_loop
```console
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
```

We can see that libuv is calling kqueue which creates a new queue. For a
standalone kqueue example see 
[kqueue-example](https://github.com/danbev/learning-c/blob/master/kqueue.c).

I'm not sure what loop->backend_fd represents yet but this will be set to the
file descriptor returned by kqueue. ioctl is later used to close this file
descriptor on exec before returning to the caller. More about this below

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


### self-pipe
Libuv uses self-pipes which is the reason for adding this section.
The file system is not pollable so instead a thread is used from the thread pool. The blocking call is performed
by the thread and when it is completed it will signal the event loop about this using either eventfd (linux) or a self-pipe. There is no way to wait on a thread in a kqueue/epoll loop so when the thread is done, it will write a byte into the pipe (self-pipe) and the kqueue/epoll loop side will wait for this and wakes up when it arrives.

Signals can be sent to any process at any time and each process and have there own signal handlers to handle
the various signals. Your application code will get interupted and then resume when the signal handler has run.

Recall that a pipe is one way, one end if for writing and the other for reading. And it handles a stream of 
bytes. Anything writting to the pipe will be left there until something reads from the pipe (or the pipe will
become full and block)

Normally a pipe is shared with subprocesses but a that is not the case with a shared pipe, it is not shared.
