#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

void send_cb(uv_udp_send_t* req, int status) {
  printf("Send callback %d\n", status);
}

int main(void) {
  struct sockaddr_in addr;
  int port = 8888;
  uv_udp_t send_socket;

  int r = uv_ip4_addr("127.0.0.1", port, &addr);
  if (r) {
    printf("Could not resolve ip4 addr. Reason: %s\n", uv_strerror(r));
    return 2;
  } 

  r = uv_udp_init(uv_default_loop(), &send_socket);
  if (r) {
    printf("Could not init to port %d. Reason: %s\n", port, uv_strerror(r));
    return 1;
  } 

  uv_udp_send_t req;
  uv_buf_t buf = {"Bajja", 5};
  r = uv_udp_send(&req,
                  &send_socket,
                  &buf,
                  1,
                  (const struct sockaddr *)&addr,
                  send_cb);
  

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  return 0;
}
