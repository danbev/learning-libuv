#include <stdio.h>
#include <uv.h>

void callback(uv_idle_t* handle) {
  printf("callback\n");
}

uv_idle_t idleHandler(uv_loop_t* loop) {
    printf("handler...\n");
    uv_idle_t handler;
    uv_idle_init(loop, &handler);
    return handler;
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_idle_t handler = idleHandler(loop);
    uv_idle_start(&handler, callback);
    // run one to have the handler invoked
    uv_run(loop, UV_RUN_ONCE);
    // run once more to have the callback invoked
    uv_loop_close(loop);
    return 0;
}

