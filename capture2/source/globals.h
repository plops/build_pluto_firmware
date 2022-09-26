#ifndef GLOBALS_H

#define GLOBALS_H

#include "vis_02_tsqueue.hpp"
#define FMT_HEADER_ONLY
#include "/home/martin/src/fmt/include/fmt/format.h"
void lprint(std::string msg, std::initializer_list<std::string> il,
            std::string func, std::string file, int line);

extern std::chrono::time_point<std::chrono::high_resolution_clock> g_start_time;

#include <chrono>
struct State {
  typeof(std::chrono::high_resolution_clock::now().time_since_epoch().count())
      _start_time;
  bool _server_keep_running;
  tsqueue<uint16_t> _iq_out;
  std::string _code_license;
  std::string _code_author;
  std::string _code_generation_time;
  std::string _code_repository;
  std::string _code_version;
};
typedef struct State State;

#endif
