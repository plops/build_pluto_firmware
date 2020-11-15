
#include "utils.h"

#include "globals.h"

extern State state;
#include <chrono>
#include <iostream>
#include <thread>

#include "vis_01_glfw_window.hpp"
#include "vis_02_draw.hpp"
#include "vis_03_gui.hpp"
#include "vis_04_client.hpp"
using namespace std::chrono_literals;

// implementation
State state = {};
void mainLoop() {
  while (!(glfwWindowShouldClose(state._window))) {
    glfwPollEvents();
    glfwGetCursorPos(state._window, &(state._cursor_xpos),
                     &(state._cursor_ypos));
    drawFrame();
    drawGui();
    glfwSwapBuffers(state._window);
  }
}
int main(int argc, char **argv) {
  state._code_repository = "github.com/plops/build_pluto_firmware";
  state._code_generation_time = "22:01:05 of Sunday, 2020-11-15 (GMT+1)";
  state._start_time =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("start") << (" ")
      << (std::setw(8)) << (" argc='") << (argc) << ("'") << (std::setw(8))
      << (" argv[0]='") << (argv[0]) << ("'") << (std::endl) << (std::flush);
  initWindow();
  initGui();
  initDraw();
  initClient();
  mainLoop();
  cleanupDraw();
  cleanupGui();
  cleanupWindow();
  return 0;
}