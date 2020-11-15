#ifndef VIS_04_CLIENT_H
#define VIS_04_CLIENT_H
#include "utils.h"
;
#include "globals.h"
;
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
;
#include <chrono>
#include <iostream>
#include <thread>
;
void initClient(uint8_t *header, size_t nbytes_header, uint8_t *buf,
                size_t nbytes);
#endif