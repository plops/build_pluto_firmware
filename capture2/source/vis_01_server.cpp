
#include "utils.h"

#include "globals.h"

extern State state;
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
// http://www.linuxhowtos.org/data/6/server.c
;
void create_server() {
  state._server_keep_running = true;
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  auto portno = 1234;
  struct sockaddr_in server_addr = {};
  struct sockaddr_in client_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);
  lprint("attempt bind", {" portno='", fmt::format("{}", portno), "'"},
         __func__, __FILE__, __LINE__);
  if ((bind(fd, reinterpret_cast<struct sockaddr *>(&server_addr),
            sizeof(server_addr))) < (0)) {
    lprint("bind failed", {}, __func__, __FILE__, __LINE__);
  }
  listen(fd, 5);
  lprint("initiate accept", {}, __func__, __FILE__, __LINE__);
  while (state._server_keep_running) {
    auto client_len = sizeof(client_addr);
    auto fd1 = accept(fd, reinterpret_cast<struct sockaddr *>(&client_addr),
                      &client_len);
    if ((fd1) < (0)) {
      lprint("accept failed", {}, __func__, __FILE__, __LINE__);
    }
    char client_addr_buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_buffer,
              sizeof(client_addr_buffer));
    lprint("accept",
           {" client_addr_buffer='", fmt::format("{}", client_addr_buffer), "'",
            " client_addr.sin_port='", fmt::format("{}", client_addr.sin_port),
            "'"},
           __func__, __FILE__, __LINE__);
    lprint("enter transmission loop",
           {" state._iq_out.empty()='",
            fmt::format("{}", state._iq_out.empty()), "'",
            " state._iq_out.front()='",
            fmt::format("{}", state._iq_out.front()), "'",
            " state._iq_out.back()='", fmt::format("{}", state._iq_out.back()),
            "'"},
           __func__, __FILE__, __LINE__);
    if (state._iq_out.empty()) {
      state._iq_out.wait_while_empty();
    }
    while (!(state._iq_out.empty())) {
      auto msg = state._iq_out.pop_front();
      auto n = write(fd1, reinterpret_cast<uint8_t *>(&msg), 2);
      if ((n) < (0)) {
        lprint("write failed", {}, __func__, __FILE__, __LINE__);
      }
    }
    close(fd1);
  }
  close(fd);
}
std::thread run_server_in_new_thread() {
  lprint("attempt to start a thread with the server", {}, __func__, __FILE__,
         __LINE__);
  return std::thread(create_server);
}