#ifndef VIS_01_GLFW_WINDOW_H
#define VIS_01_GLFW_WINDOW_H
#include "utils.h"
;
#include "globals.h"
;
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods);
void errorCallback(int err, const char *description);
static void framebufferResizeCallback(GLFWwindow *window, int width,
                                      int height);
void initWindow();
void cleanupWindow();
#endif