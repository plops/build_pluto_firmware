#ifndef GLOBALS_H

#define GLOBALS_H

#include <chrono>
struct State {
  typeof(std::chrono::high_resolution_clock::now().time_since_epoch().count())
      _start_time;
  std::string _code_license;
  std::string _code_author;
  std::string _code_generation_time;
  std::string _code_repository;
  std::string _code_version;
};
typedef struct State State;

#endif
