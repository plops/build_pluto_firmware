
#include "utils.h"

#include "globals.h"

extern State state;
#include <chrono>
#include <iostream>
#include <thread>
using namespace std::chrono_literals;

// implementation
int main(int argc, char **argv) {
  state._code_version = "206621dcbbd7565d377810038ea2785fd5b9d184";
  state._code_repository = "https://github.com/plops/cl-cpp-generator2/tree/"
                           "master/example/27_sparse_eigen_hydrogen";
  state._code_generation_time = "11:11:17 of Sunday, 2020-10-25 (GMT+1)";
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