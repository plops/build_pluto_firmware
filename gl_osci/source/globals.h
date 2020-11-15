#ifndef GLOBALS_H

#define GLOBALS_H

#include <chrono>
struct State {
  typeof(std::chrono::high_resolution_clock::now().time_since_epoch().count())
      _start_time;
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
  std::string _code_generation_time;
  std::string _code_repository;
  std::string _main_version;
};
typedef struct State State;

#endif
