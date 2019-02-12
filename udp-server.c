#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t *loop;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char*) malloc(suggested_size);
  buf->len = suggested_size;
}

void on_read(uv_udp_t *req,
             ssize_t nread,
             const uv_buf_t *buf,
             const struct sockaddr *addr,
             unsigned flags) {
  if (nread < 0) {
    fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    uv_close((uv_handle_t*) req, NULL); // NULL is the uv_close_cb callback
    free(buf->base);
    return;
  }
  if (nread != 0) {
    printf("on_read: %s\n", buf->base);
    free(buf->base);
  }
}

int main(void) {
  // we need to allocate memory for the loop struct because libuv can be embedded and its
  // memory allocation might be externally managed. Also, we are just allocating memory
  // not doing anything with that memmory
  loop = malloc(sizeof(uv_loop_t));

  // malloc only reserves memory. That memory may or may not be filled with anything.
  // So we initialize the loop
  uv_loop_init(loop);
  
  // handle of type uv_udp_t
  uv_udp_t recv_socket;

  struct sockaddr_in addr;
  int port = 8888;
  //int r = uv_ip4_addr("0.0.0.0", port, &addr);
  int r = uv_ip4_addr("127.0.0.1", port, &addr);
  if (r) {
    printf("Could not resolve ip4 addr. Reason: %s\n", uv_strerror(r));
    return 2;
  } 

  uv_udp_init(loop, &recv_socket);

  //r = uv_udp_bind(&recv_socket, (const struct sockaddr *)&addr, UV_UDP_REUSEADDR);
  r = uv_udp_bind(&recv_socket, (const struct sockaddr *)&addr, 0);
  if (r) {
    printf("Could not bind. Reason: %s\n", uv_strerror(r));
    return 2;
  } 
  r = uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);
  if (r) {
    printf("Could not start recieving. Reason: %s\n", uv_strerror(r));
    return 2;
  } 

  uv_run(loop, UV_RUN_DEFAULT);

  free(loop);
  return 0;
}
