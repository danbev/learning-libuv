#include <stdio.h>
#include <uv.h>

/**
 * Callback which will be called upon each event loop tick, 
 * before polling for I/O.
 */
void callback(uv_prepare_t* handle) {
    printf("prepare callback\n");
    uv_prepare_stop(handle);
}

uv_prepare_t prepareHandler(uv_loop_t* loop) {
    uv_prepare_t handler;
    uv_prepare_init(loop, &handler);
    return handler;
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_prepare_t handler = prepareHandler(loop);
    uv_prepare_start(&handler, callback);
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    return 0;
}

