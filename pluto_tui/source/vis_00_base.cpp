
#include "utils.h"

#include "globals.h"

extern State state;
#include "locale.h"
#include "ncurses.h"
#include "wchar.h"
#include <array>
#include <chrono>
#include <complex>
#include <fftw3.h>
#include <iio.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <unistd.h>
#define MHz(x) ((long long)(x * 1000000.0 + .5))
#define GHz(x) ((long long)(x * 1000000000.0 + .5))
enum iodev { RX, TX };
struct stream_cfg {
  long long bw_hz;
  long long fs_hz;
  long long lo_hz;
  const char *rfport;
};
typedef struct stream_cfg stream_cfg;
using namespace std::chrono_literals;

// implementation
State state;
int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  state._code_version = "e2a2628e73f315feabd22814ab210b685be5c4c1";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/pluto_tui";
  state._code_author = "Martin Kielhorn <kielhorn.martin@gmail.com>";
  state._code_license = "GPL v3";
  state._code_generation_time = "16:28:24 of Saturday, 2020-10-31 (GMT+1)";
  state._start_time =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("start") << (" ")
      << (std::setw(8)) << (" argc='") << (argc) << ("'") << (std::setw(8))
      << (" argv[0]='") << (argv[0]) << ("'") << (std::endl) << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._start_time='") << (state._start_time)
      << ("'") << (std::endl) << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_version='") << (state._code_version)
      << ("'") << (std::endl) << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_repository='")
      << (state._code_repository) << ("'") << (std::endl) << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_generation_time='")
      << (state._code_generation_time) << ("'") << (std::endl) << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_author='") << (state._code_author)
      << ("'") << (std::endl) << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_license='") << (state._code_license)
      << ("'") << (std::endl) << (std::flush);
  auto count = 0;
  while (true) {
    (count)++;
    if (!(count % 12)) {
      usleep(16000);
      (std::cout) << ("\x1b[H\x1b[J");
    }
    (std::cout) << (count) << (std::endl);
  }
  return 0;
}