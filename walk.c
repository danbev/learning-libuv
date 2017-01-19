#include <stdio.h>
#include <uv.h>

void walk(uv_handle_t* handle, void* arg) {
  if (handle->type ==UV_IDLE) {
    printf("walking idle handle %d\n", handle->type);
  } else {
    printf("walking\n");
  }
}

uv_idle_t idleHandler(uv_loop_t* loop) {
    printf("idlehandler...");
    uv_idle_t handler;
    uv_idle_init(loop, &handler);
    return handler;
}

int main() {
    uv_loop_t* loop = uv_default_loop();
    uv_idle_t handler = idleHandler(loop);
    uv_walk(loop, walk, NULL);  
    uv_loop_close(loop);
    return 0;
}

