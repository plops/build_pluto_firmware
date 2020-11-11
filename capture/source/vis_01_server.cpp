
#include "utils.h"

#include "globals.h"

extern State state;
#include <chrono>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
void create_server(uint8_t *buf, size_t nbytes) {
  auto fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  bzero(reinterpret_cast<uint8_t *>(&server_addr), sizeof(server_addr));
}