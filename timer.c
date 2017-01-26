#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

void timer_callback(uv_timer_t* handle) {
    printf("bajja\n");
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_loop_init(loop);

    uv_timer_t timer;
    uv_timer_init(loop, &timer);
    uv_timer_start(&timer, timer_callback, 15000, 0);

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    return 0;
}
