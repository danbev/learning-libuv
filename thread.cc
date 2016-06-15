#include <uv.h>
#include <stdio.h>
#include <unistd.h>

uv_loop_t* loop;
uv_async_t async;

void t1(void *handler) {
    sleep(1);
    printf("t1: uv_async_send(t1)\n");
    async.data = (void *) "t1";
    uv_async_send(&async);
}

void t2(void *handler) {
    sleep(3);
    printf("t2: uv_async_send(t2)\n");
    async.data = (void *) "t2";
    uv_async_send(&async);
}

int counter = 0;
void asyncCallback(uv_async_t* handle) {
    printf("asyncCallback: %s\n", (char*) handle->data);
    counter++;
    if (counter == 2) {
        printf("All callbacks done. uv_unref used to stop the event loop\n");
        uv_unref((uv_handle_t*)handle);
    } 
}

void loopThread(void *a) {
    uv_async_init(loop, &async, asyncCallback);
    uv_run(loop, UV_RUN_DEFAULT);
}


int main() {
    loop = uv_default_loop();
    uv_thread_t loopThread_id;
    uv_thread_t t1_id;
    uv_thread_t t2_id;
    uv_thread_create(&loopThread_id, loopThread, &async);
    uv_thread_create(&t1_id, t1, &async);
    uv_thread_create(&t2_id, t2, &async);

    uv_thread_join(&t1_id);
    uv_thread_join(&t2_id);
    uv_thread_join(&loopThread_id);

    uv_loop_close(loop);
    return 0;
}
