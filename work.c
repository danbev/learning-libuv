#include <stdio.h>
#include <uv.h>

/**
 * Run on a thread from the thread pool.
 */
void work(uv_work_t* w) {
  printf("[work thread] work function. data=%d\n", w->data);
}

/**
 * Called on the main thread after the work has completed.
 */
void after_work(uv_work_t* w, int status) {
  printf("[main thread] after work funtion. status: %d\n", status);
}

int main() {
  uv_loop_t* loop = uv_default_loop();
  uv_work_t req;
  req.data = (void*)18;
  // request that work be performed on a thread from the thread pool.
  uv_queue_work(loop, &req, work, after_work);

  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);
  return 0;
}

