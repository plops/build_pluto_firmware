
#include "utils.h"

#include "globals.h"

extern State state;
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
// http://www.linuxhowtos.org/data/6/server.c
;
void create_server(uint8_t *header, size_t nbytes_header, uint8_t *buf,
                   size_t nbytes) {
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  auto portno = 1234;
  struct sockaddr_in server_addr = {};
  struct sockaddr_in client_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(portno);
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
  auto client_len = sizeof(client_addr);
  auto fd1 = accept(fd, reinterpret_cast<struct sockaddr *>(&client_addr),
                    &client_len);
  if ((fd1) < (0)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("accept failed") << (" ") << (std::endl) << (std::flush);
  }
  auto nh = write(fd1, header, nbytes_header);
  if ((nh) < (0)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("writing header failed") << (" ") << (std::endl)
                << (std::flush);
  }
  auto n = write(fd1, buf, nbytes);
  if ((n) < (0)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("write failed") << (" ") << (std::endl) << (std::flush);
  }
  close(fd1);
  close(fd);
}