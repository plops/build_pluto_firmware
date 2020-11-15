
#include "utils.h"

#include "globals.h"

extern State state;
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
  auto server = gethostbyname("192.168.2.1");
  struct sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(portno);
  bcopy(static_cast<char *>(server->h_addr),
        reinterpret_cast<char *>(&server_addr.sin_addr.s_addr),
        server->h_length);
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
  auto n = read(fd, reinterpret_cast<uint8_t *>(state._iqdata.data()),
                ((2) * (state._iqdata.size())));

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("read returned")
      << (" ") << (std::setw(8)) << (" n='") << (n) << ("'") << (std::endl)
      << (std::flush);
  state._iqdata_bytes = n;
  if ((n) < (0)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("socket read failed") << (" ") << (std::endl)
                << (std::flush);
  }
  close(fd);
}