#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

void idle_callback(uv_idle_t* handle) {
    printf("Idle handler...\n");
}

void check_callback(uv_check_t* handle) {
    printf("check handler...\n");
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_loop_init(loop);

    uv_idle_t idle;
    uv_idle_init(loop, &idle);

    uv_check_t check;
    uv_check_init(loop, &check);

    uv_ref((uv_handle_t*)&check);
    uv_idle_start(&idle, idle_callback);
    int r = uv_check_start(&check, check_callback);
    printf("returned %d.\n", r);

    printf("Idle and Check example.\n");

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    return 0;
}
