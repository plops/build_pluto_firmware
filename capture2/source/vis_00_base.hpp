#ifndef VIS_00_BASE_H
#define VIS_00_BASE_H
#include "utils.h"
;
#include "globals.h"
;
#include <chrono>
#include <iostream>
#include <thread>
;
#include <algorithm>
#include <array>
#include <complex>
#include <iio.h>
#include <math.h>
#include <unistd.h>
#include <vector>
;
#include "vis_01_server.hpp"
;
// header;
void lprint(std::string msg, std::initializer_list<std::string> il,
            std::string func, std::string file, int line);
int main(int argc, char **argv);
#endif