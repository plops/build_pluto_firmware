#ifndef VIS_01_SERVER_H
#define VIS_01_SERVER_H
#include "utils.h"
;
#include "globals.h"
;
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
;
#include <chrono>
#include <iostream>
#include <thread>
;
void create_server(uint8_t *buf, size_t nbytes);
#endif