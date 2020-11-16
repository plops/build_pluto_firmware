#ifndef GLOBALS_H

#define GLOBALS_H

#include "vis_02_tsqueue.hpp"

#include <chrono>
struct State {
  typeof(std::chrono::high_resolution_clock::now().time_since_epoch().count())
      _start_time;
  tsqueue<uint16_t> _iq_out;
  std::string _code_license;
  std::string _code_author;
  std::string _code_generation_time;
  std::string _code_repository;
  std::string _code_version;
};
typedef struct State State;

#endif
