#ifndef VIS_04_CLIENT_H
#define VIS_04_CLIENT_H
#include "utils.h"
;
#include "globals.h"
;
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
;
#include <chrono>
#include <iostream>
#include <thread>
;
void initClient();
#endif