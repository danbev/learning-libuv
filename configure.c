#include <uv.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * Just an example of using uv_loop_configure. Mainly showing how it is called
 * and I've not using it in a real sense yet.
 */
int main() {
    uv_loop_t* loop = uv_default_loop();
    // currently the only signal that is implemented is SIGPROF which is a signal
    // the kernel generates this signal upon the expiration of a profiling timer set by a call to settimer()
    int r = uv_loop_configure(loop, UV_LOOP_BLOCK_SIGNAL, SIGPROF);
    printf("uv_loop_configure returned %d\n", r);
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    return 0;
}
