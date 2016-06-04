#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

uv_loop_t *loop;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char*) malloc(suggested_size);
  buf->len = suggested_size;
}

void accept_callback(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
  if (nread < 0) {
    if (nread != UV_EOF) {
      fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    }
    uv_close((uv_handle_t*) client, NULL);
  } else if (nread > 0) {
    printf("Read data: %s, data=%s\n", buf->base, client->data);
  }

  if (buf->base) {
    free(buf->base);
  }
}

void new_connection_callback(uv_stream_t *server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    return;
  }

  // client "socket"
  uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  client->data = "test";
  uv_tcp_init(loop, client);
  if (uv_accept(server, (uv_stream_t*) client) == 0) {
    uv_read_start((uv_stream_t*) client, alloc_buffer, accept_callback);
  } else {
    uv_close((uv_handle_t*) client, NULL);
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
  
  // handle of type uv_tcp_t
  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  struct sockaddr_in addr;
  int port = 7777;
  uv_ip4_addr("0.0.0.0", port, &addr);

  int r = uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0); 
  if (r) {
    printf("Could not bind server to port %d. Reason: %s\n", port, uv_strerror(r));
    return 1;
  } else {
    printf("libuv Server bound to port %d\n", port);
  }

  r = uv_listen((uv_stream_t*) &server, 128, new_connection_callback);
  if (r) {
    printf("Could not listen. Reason: %s\n", uv_strerror(r));
    return 2;
  } else {
    printf("Libvu Server listening...\n");
  }

  uv_run(loop, UV_RUN_DEFAULT);
  free(loop);
  return 0;
}
