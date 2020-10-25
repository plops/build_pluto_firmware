
#include "utils.h"

#include "globals.h"

extern State state;
#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

// implementation
int main(int argc, char **argv) {
  state._code_version = "93ee5e309ebb09fcd1332acd9a44fadf89617ea4";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/pluto_tui";
  state._code_generation_time = "11:12:37 of Sunday, 2020-10-25 (GMT+1)";
  state._start_time =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("start") << (" ")
      << (std::setw(8)) << (" argc='") << (argc) << ("'") << (std::setw(8))
      << (" argv[0]='") << (argv[0]) << ("'") << (std::setw(8))
      << (" state._start_time='") << (state._start_time) << ("'") << (std::endl)
      << (std::flush);
  return 0;
}