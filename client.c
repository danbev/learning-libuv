#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t loop;
uv_tcp_t tcp_client;
uv_connect_t connect_req;
uv_buf_t buf = {"Bajja", 5};

static void write_cb(uv_write_t *req, int status) {
  printf("write done...\n");
  uv_close((uv_handle_t*) req->handle, NULL);
}

static void connect_cb(uv_connect_t* req, int status) {
  printf("connect callback status: %d\n", status);
  uv_write_t write_request;
  uv_write(&write_request, (uv_stream_t *)&tcp_client, &buf, 1, write_cb);
}


int main(void) {
  struct sockaddr_in addr;
  int port = 7777;

  uv_ip4_addr("0.0.0.0", port, &addr);

  uv_loop_init(&loop);

  int r = uv_tcp_init(&loop, &tcp_client);
  if (r) {
    printf("Could not init to port %d. Reason: %s\n", port, uv_strerror(r));
    return 1;
  } 

  r = uv_tcp_connect(&connect_req,
                     &tcp_client,
                     (const struct sockaddr*) &addr,
                     connect_cb);
  if (r) {
    printf("Could not connect. Reason: %s\n", uv_strerror(r));
    return 2;
  } 

  uv_run(&loop, UV_RUN_DEFAULT);

  return 0;
}
