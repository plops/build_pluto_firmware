#ifndef GLOBALS_H

#define GLOBALS_H

#include <mutex>
#include <thread>

#include <GLFW/glfw3.h>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <chrono>
struct State {
  typeof(std::chrono::high_resolution_clock::now().time_since_epoch().count())
      _start_time;
  int _iqdata_bytes;
  std::array<int16_t, 1024 * 128 * 2> _iqdata;
  bool _gui_request_diff_reset;
  std::mutex _gui_mutex;
  glm::vec2 _snapped_world_cursor;
  float _screen_grid;
  float _screen_scale;
  glm::vec2 _screen_start_pan;
  glm::vec2 _screen_offset;
  float _draw_marker_x;
  float _draw_alpha;
  float _draw_scale_y;
  float _draw_scale_x;
  float _draw_offset_y;
  float _draw_offset_x;
  bool _draw_display_log;
  std::mutex _draw_mutex;
  GLuint _fontTex;
  bool _framebufferResized;
  GLFWwindow *_window;
  double _cursor_ypos;
  double _cursor_xpos;
  std::string _code_generation_time;
  std::string _code_repository;
  std::string _main_version;
};
typedef struct State State;

#endif
