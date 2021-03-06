
#include "utils.h"

#include "globals.h"

extern State state;
#include <arpa/inet.h>
#include <chrono>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
// http://www.linuxhowtos.org/data/6/server.c
// http://www.linuxhowtos.org/data/6/client.c
;
void initClient() {
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  auto portno = 1234;
  auto server = gethostbyname("localhost");
  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portno);
  bcopy(static_cast<char *>(server->h_addr),
        reinterpret_cast<char *>(&server_addr.sin_addr.s_addr),
        server->h_length);
  char server_addr_buffer[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &server_addr.sin_addr, server_addr_buffer,
            sizeof(server_addr_buffer));

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("try to connect")
      << (" ") << (std::setw(8)) << (" server_addr_buffer='")
      << (server_addr_buffer) << ("::") << (typeid(server_addr_buffer).name())
      << ("'") << (std::setw(8)) << (" server_addr.sin_port='")
      << (server_addr.sin_port) << ("::")
      << (typeid(server_addr.sin_port).name()) << ("'") << (std::endl)
      << (std::flush);
  if ((connect(fd, reinterpret_cast<struct sockaddr *>(&server_addr),
               sizeof(server_addr))) < (0)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("connect failed") << (" ") << (std::endl) << (std::flush);
  }
  auto bytes_remaining = ((2) * (state._iqdata.size()));
  auto offset_bytes = 0;
  while (((bytes_remaining))) {
    auto n = read(
        fd,
        ((reinterpret_cast<uint8_t *>(state._iqdata.data())) + (offset_bytes)),
        bytes_remaining);

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("read returned") << (" ") << (std::setw(8)) << (" n='")
                << (n) << ("::") << (typeid(n).name()) << ("'")
                << (std::setw(8)) << (" bytes_remaining='") << (bytes_remaining)
                << ("::") << (typeid(bytes_remaining).name()) << ("'")
                << (std::setw(8)) << (" offset_bytes='") << (offset_bytes)
                << ("::") << (typeid(offset_bytes).name()) << ("'")
                << (std::endl) << (std::flush);
    if ((n) == (0)) {

      (std::cout) << (std::setw(10))
                  << (std::chrono::high_resolution_clock::now()
                          .time_since_epoch()
                          .count())
                  << (" ") << (std::this_thread::get_id()) << (" ")
                  << (__FILE__) << (":") << (__LINE__) << (" ") << (__func__)
                  << (" ") << ("socket empty?") << (" ") << (std::endl)
                  << (std::flush);
      break;
    }
    if ((n) < (0)) {

      (std::cout) << (std::setw(10))
                  << (std::chrono::high_resolution_clock::now()
                          .time_since_epoch()
                          .count())
                  << (" ") << (std::this_thread::get_id()) << (" ")
                  << (__FILE__) << (":") << (__LINE__) << (" ") << (__func__)
                  << (" ") << ("socket read failed") << (" ") << (std::endl)
                  << (std::flush);
      break;
    }
    (bytes_remaining) -= (n);
    (offset_bytes) += (n);
    state._iqdata_bytes =
        ((((2) * (state._iqdata.size()))) - (bytes_remaining));
  }
  close(fd);
}