#include <stdio.h>
#include <uv.h>

/**
 * Callback which will be called upon each event loop tick.
 */
void callback(uv_idle_t* handle) {
    int* counter = (int*) handle->data;
    if (*counter == 10) {
        uv_idle_stop(handle);
    } else {
        printf("tick callback.counter = %d\n", *counter);
        (*counter)++;
        handle->data = counter;
    }
}

uv_idle_t idleHandler(uv_loop_t* loop) {
    uv_idle_t handler;
    uv_idle_init(loop, &handler);
    int* p = 0;
    handler.data = &p;
    return handler;
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_idle_t handler = idleHandler(loop);
    uv_idle_start(&handler, callback);
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    return 0;
}

