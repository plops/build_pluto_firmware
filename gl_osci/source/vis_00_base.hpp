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
#include "vis_01_glfw_window.hpp"
#include "vis_02_draw.hpp"
#include "vis_03_gui.hpp"
;
// header;
void mainLoop();
int main(int argc, char **argv);
#endif