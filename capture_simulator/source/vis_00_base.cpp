
#include "utils.h"

#include "globals.h"

extern State state;
#include "vis_01_server.hpp"
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <complex>
#include <iostream>
#include <math.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace std::chrono_literals;

// implementation
State state;
int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  state._code_version = "b7d4de9a07e60747a47afd7c28c6e4f40588bd4e";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/capture";
  state._code_author = "Martin Kielhorn <kielhorn.martin@gmail.com>";
  state._code_license = "GPL v3";
  state._code_generation_time = "18:21:02 of Friday, 2020-11-20 (GMT+1)";
  state._start_time =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("start") << (" ")
      << (std::setw(8)) << (" argc='") << (argc) << ("::")
      << (typeid(argc).name()) << ("'") << (std::setw(8)) << (" argv[0]='")
      << (argv[0]) << ("::") << (typeid(argv[0]).name()) << ("'") << (std::endl)
      << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._start_time='") << (state._start_time)
      << ("::") << (typeid(state._start_time).name()) << ("'") << (std::endl)
      << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_version='") << (state._code_version)
      << ("::") << (typeid(state._code_version).name()) << ("'") << (std::endl)
      << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_repository='")
      << (state._code_repository) << ("::")
      << (typeid(state._code_repository).name()) << ("'") << (std::endl)
      << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_generation_time='")
      << (state._code_generation_time) << ("::")
      << (typeid(state._code_generation_time).name()) << ("'") << (std::endl)
      << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_author='") << (state._code_author)
      << ("::") << (typeid(state._code_author).name()) << ("'") << (std::endl)
      << (std::flush);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" state._code_license='") << (state._code_license)
      << ("::") << (typeid(state._code_license).name()) << ("'") << (std::endl)
      << (std::flush);
  auto fn = "o_4channels";
  auto file_length = ([fn]() {
    struct stat st;
    stat(fn, &st);
    return st.st_size;
  })();
  auto fd = ([fn]() {
    auto fd = open(fn, O_RDONLY, 0);
    assert((fd) != (-1));
    return fd;
  })();
  auto iq_input_data = ([file_length, fd]() {
    auto m = mmap(nullptr, file_length, PROT_READ,
                  ((MAP_PRIVATE) | (MAP_POPULATE)), fd, 0);
    assert((MAP_FAILED) != (m));
    return static_cast<uint16_t *>(m);
  })();
  auto sample_and_compute_start =
      std::chrono::high_resolution_clock::now().time_since_epoch();
  auto sample_start = sample_and_compute_start;
  auto compute_start = sample_and_compute_start;
  auto server_thread = run_server_in_new_thread();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("server started")
      << (" ") << (std::endl) << (std::flush);
  auto count = 0;
  while (true) {
    sample_start = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto time_now =
        std::chrono::high_resolution_clock::now().time_since_epoch();
    auto sample_dur = ((time_now) - (sample_start)).count();
    auto step = ((2) * (2));
    auto start = reinterpret_cast<uint8_t *>(iq_input_data);
    auto end = ((start) + (file_length));
    auto i = 0;
    compute_start =
        std::chrono::high_resolution_clock::now().time_since_epoch();
    auto ma = 0;
    auto old = (0.f);
    auto trig = 0;
    auto trig1 = 0;
#pragma GCC ivdep
    for (uint8_t *p = start; (p) < (end); (p) += (step)) {
      auto si = reinterpret_cast<int16_t *>(p)[0];
      auto sq = reinterpret_cast<int16_t *>(p)[1];
      auto m = ((((si) * (si))) + (((sq) * (sq))));
      auto mlow = ([](float xn) -> float {
        // filter_2_low_01_real
        ;
        static float yn1 = (0.f);
        static float yn2 = (0.f);
        static float xn1 = (0.f);
        static float xn2 = (0.f);
        float yn =
            (((((8.664870e-4f)) * (xn))) + ((((1.7326780e-3f)) * (xn1))) +
             ((((8.663387e-4f)) * (xn2))) + ((((1.919129f)) * (yn1))) +
             ((((-0.9225943f)) * (yn2))));
        xn2 = xn1;
        xn1 = xn;
        yn2 = yn1;
        yn1 = yn;
        return yn;
      })(m);
      if ((ma) < (mlow)) {
        ma = mlow;
      }
      if ((((trig) == (0)) && ((old) < (4000)) && ((4000) <= (mlow)))) {
        trig = i;
      }
      if ((0) < (trig)) {
        state._iq_out.push_back(si);
        state._iq_out.push_back(sq);
        if ((((2000) < (old)) && ((mlow) <= (2000)))) {
          trig1 = i;
          auto pulse_ms = ((((trig1) - (trig))) / ((6.1440e+4f)));

          (std::cout) << (std::setw(10))
                      << (std::chrono::high_resolution_clock::now()
                              .time_since_epoch()
                              .count())
                      << (" ") << (std::this_thread::get_id()) << (" ")
                      << (__FILE__) << (":") << (__LINE__) << (" ")
                      << (__func__) << (" ") << ("") << (" ") << (std::setw(8))
                      << (" ma='") << (ma) << ("::") << (typeid(ma).name())
                      << ("'") << (std::setw(8)) << (" trig='") << (trig)
                      << ("::") << (typeid(trig).name()) << ("'")
                      << (std::setw(8)) << (" trig1='") << (trig1) << ("::")
                      << (typeid(trig1).name()) << ("'") << (std::setw(8))
                      << (" pulse_ms='") << (pulse_ms) << ("::")
                      << (typeid(pulse_ms).name()) << ("'") << (std::endl)
                      << (std::flush);
          trig = 0;
        }
      }
      (i)++;
      old = mlow;
    }
    auto compute_end =
        std::chrono::high_resolution_clock::now().time_since_epoch();
    auto compute_dur = ((compute_end) - (compute_start)).count();
    auto compute_samp_dur = ((compute_end) - (sample_start)).count();
    auto compute_perc = ((((100) * (compute_dur))) / (compute_samp_dur));
    auto sample_perc = ((((100) * (sample_dur))) / (compute_samp_dur));
    (count)++;

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("374") << (" ") << (std::setw(8)) << (" count='") << (count)
                << ("::") << (typeid(count).name()) << ("'") << (std::setw(8))
                << (" compute_perc='") << (compute_perc) << ("::")
                << (typeid(compute_perc).name()) << ("'") << (std::setw(8))
                << (" sample_perc='") << (sample_perc) << ("::")
                << (typeid(sample_perc).name()) << ("'") << (std::endl)
                << (std::flush);
  }
  state._server_keep_running = false;
  if (server_thread.joinable()) {
    server_thread.join();
  }
  return 0;
}