
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
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  auto portno = 1234;
  struct sockaddr_in server_addr = {};
  struct sockaddr_in client_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("attempt bind") << (" ")
      << (std::setw(8)) << (" portno='") << (portno) << ("::")
      << (typeid(portno).name()) << ("'") << (std::endl) << (std::flush);
  if ((bind(fd, reinterpret_cast<struct sockaddr *>(&server_addr),
            sizeof(server_addr))) < (0)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("bind failed") << (" ") << (std::endl) << (std::flush);
  }
  listen(fd, 5);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("initiate accept")
      << (" ") << (std::endl) << (std::flush);
  while (true) {
    auto client_len = static_cast<unsigned int>(sizeof(client_addr));
    auto fd1 = accept(fd, reinterpret_cast<struct sockaddr *>(&client_addr),
                      &client_len);
    if ((fd1) < (0)) {

      (std::cout) << (std::setw(10))
                  << (std::chrono::high_resolution_clock::now()
                          .time_since_epoch()
                          .count())
                  << (" ") << (std::this_thread::get_id()) << (" ")
                  << (__FILE__) << (":") << (__LINE__) << (" ") << (__func__)
                  << (" ") << ("accept failed") << (" ") << (std::endl)
                  << (std::flush);
    }
    char client_addr_buffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr, client_addr_buffer,
              sizeof(client_addr_buffer));

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("accept") << (" ") << (std::setw(8))
                << (" client_addr_buffer='") << (client_addr_buffer) << ("::")
                << (typeid(client_addr_buffer).name()) << ("'")
                << (std::setw(8)) << (" client_addr.sin_port='")
                << (client_addr.sin_port) << ("::")
                << (typeid(client_addr.sin_port).name()) << ("'") << (std::endl)
                << (std::flush);

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("enter transmission loop") << (" ") << (std::setw(8))
                << (" state._iq_out.empty()='") << (state._iq_out.empty())
                << ("::") << (typeid(state._iq_out.empty()).name()) << ("'")
                << (std::setw(8)) << (" state._iq_out.front()='")
                << (state._iq_out.front()) << ("::")
                << (typeid(state._iq_out.front()).name()) << ("'")
                << (std::setw(8)) << (" state._iq_out.back()='")
                << (state._iq_out.back()) << ("::")
                << (typeid(state._iq_out.back()).name()) << ("'") << (std::endl)
                << (std::flush);
    if (state._iq_out.empty()) {
      state._iq_out.wait_while_empty();
    }
    while (!(state._iq_out.empty())) {
      auto msg = state._iq_out.pop_front();
      auto n = write(fd1, reinterpret_cast<uint8_t *>(&msg), 2);
      if ((n) < (0)) {

        (std::cout) << (std::setw(10))
                    << (std::chrono::high_resolution_clock::now()
                            .time_since_epoch()
                            .count())
                    << (" ") << (std::this_thread::get_id()) << (" ")
                    << (__FILE__) << (":") << (__LINE__) << (" ") << (__func__)
                    << (" ") << ("write failed") << (" ") << (std::endl)
                    << (std::flush);
      }

      (std::cout) << (std::setw(10))
                  << (std::chrono::high_resolution_clock::now()
                          .time_since_epoch()
                          .count())
                  << (" ") << (std::this_thread::get_id()) << (" ")
                  << (__FILE__) << (":") << (__LINE__) << (" ") << (__func__)
                  << (" ") << ("bytes written: ") << (" ") << (std::setw(8))
                  << (" n='") << (n) << ("::") << (typeid(n).name()) << ("'")
                  << (std::endl) << (std::flush);
    }
    close(fd1);
  }
  close(fd);
}
std::thread run_server_in_new_thread() {

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ")
      << ("attempt to start a thread with the server") << (" ") << (std::endl)
      << (std::flush);
  return std::thread(create_server);
}