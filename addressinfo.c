#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
#include "../libuv/src/idna.c"
#include <locale.h>
#include <wchar.h>

void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res) {
  if (status < 0) {
    fprintf(stderr, "getaddrinfo callback error %s\n", uv_err_name(status));
    return;
  }

  char addr[17] = {'\0'};
  uv_ip4_name((struct sockaddr_in*) res->ai_addr, addr, 16);
  fprintf(stderr, "%s\n", addr);

  /*
  uv_connect_t *connect_req = (uv_connect_t*) malloc(sizeof(uv_connect_t));
  uv_tcp_t *socket = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
  uv_tcp_init(loop, socket);

  uv_tcp_connect(connect_req, socket, (const struct sockaddr*) res->ai_addr, on_connect);
  */
  uv_freeaddrinfo(res);
}

int main(int argc, char** argv) {
  char *locale;
  locale = setlocale(LC_ALL, "");

  uv_loop_t* loop = uv_default_loop();

  const char* p;
  char b[2];
  p = b;
  b[0] = 0xF0;
  b[1] = 0x0;
  //b[2] = 0x0;
  //b[3] = 0x0;
  /*
  b[0] = 0xC2;
  b[1] = 0xA2;
  b[2] = 0xC2;
  b[3] = 0xA2;
  */
  /*
  const char* se = b+1;
  while (p - se) {
    unsigned value = uv__utf8_decode1(&p, se);
    wprintf(L"%lc\n", value);
  }
  */

  struct addrinfo hints;
  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = 0;

  uv_getaddrinfo_t resolver;
  fprintf(stderr, "\u0397\n");
  int r = uv_getaddrinfo(loop, &resolver, on_resolved, "\xF9\xc0", "80", &hints);

  if (r) {
    fprintf(stderr, "getaddrinfo call error %s\n", uv_err_name(r));
    return 1;
  }
  return uv_run(loop, UV_RUN_DEFAULT);
}

