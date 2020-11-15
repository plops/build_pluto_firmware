#ifndef VIS_03_GUI_H
#define VIS_03_GUI_H
#include "utils.h"
;
#include "globals.h"
;
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#include <imgui.h>
;
#include <algorithm>
#include <string>
;
#include <fstream>
#include <iostream>
;
void initGui();
void cleanupGui();
void drawGui();
#endif