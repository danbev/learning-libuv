#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

void check_callback(uv_check_t* handle) {
    printf("check handler...\n");
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_loop_init(loop);

    uv_check_t check;
    uv_check_init(loop, &check);

    int r = uv_check_start(&check, check_callback);
    printf("Check example.\n");

    uv_run(loop, UV_RUN_NOWAIT);

    uv_loop_close(loop);
    return 0;
}
