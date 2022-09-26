
#include "utils.h"

#include "globals.h"

extern State state;
#include "vis_01_server.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <complex>
#include <iio.h>
#include <iostream>
#include <math.h>
#include <thread>
#include <unistd.h>
#include <vector>
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
struct __attribute__((packed)) sdriq_header_t {
  uint32_t samplerate;
  uint64_t center_frequency;
  uint64_t timestamp;
  uint32_t samplesize;
  uint32_t padding;
  uint32_t crc32;
};
// This function is generates log output including wall clock time, source file
// and line, function and optionally some variables that will be submitted as
// strings in an initializer_list. Arbitrary values are converted to strings
// using fmt::format
;
// requires https://github.com/fmtlib/fmt as a header only library using the
// macro FMT_HEADER_ONLY
;
std::chrono::time_point<std::chrono::high_resolution_clock> g_start_time;
void lprint(std::string msg, std::initializer_list<std::string> il,
            std::string func, std::string file, int line) {
  std::chrono::duration<double> timestamp =
      std::chrono::high_resolution_clock::now() - g_start_time;
  (std::cout) << (std::setw(10)) << (timestamp.count()) << (" ")
              << (std::this_thread::get_id()) << (" ") << (file) << (":")
              << (line) << (" ") << (func) << (" ") << (msg) << (" ");
  for (const auto &elem : il) {
    (std::cout) << (elem);
  }
  (std::cout) << (std::endl) << (std::flush);
}
int main(int argc, char **argv) {
  g_start_time = std::chrono::high_resolution_clock::now();
  setlocale(LC_ALL, "");
  state._code_version = "d9b6927f712760ec1a467e57c0bbeb278e7ab332";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/capture";
  state._code_author = "Martin Kielhorn <kielhorn.martin@gmail.com>";
  state._code_license = "GPL v3";
  state._code_generation_time = "20:09:32 of Monday, 2022-09-26 (GMT+1)";
  state._start_time =
      std::chrono::high_resolution_clock::now().time_since_epoch().count();
  lprint("start",
         {" argc='", fmt::format("{}", argc), "'", " argv[0]='",
          fmt::format("{}", argv[0]), "'"},
         __func__, __FILE__, __LINE__);
  lprint("",
         {" state._start_time='", fmt::format("{}", state._start_time), "'"},
         __func__, __FILE__, __LINE__);
  lprint(
      "",
      {" state._code_version='", fmt::format("{}", state._code_version), "'"},
      __func__, __FILE__, __LINE__);
  lprint("",
         {" state._code_repository='",
          fmt::format("{}", state._code_repository), "'"},
         __func__, __FILE__, __LINE__);
  lprint("",
         {" state._code_generation_time='",
          fmt::format("{}", state._code_generation_time), "'"},
         __func__, __FILE__, __LINE__);
  lprint("",
         {" state._code_author='", fmt::format("{}", state._code_author), "'"},
         __func__, __FILE__, __LINE__);
  lprint(
      "",
      {" state._code_license='", fmt::format("{}", state._code_license), "'"},
      __func__, __FILE__, __LINE__);
  auto ctx = iio_create_default_context();
  if ((nullptr) == (ctx)) {
    lprint("iio_create_default_context failed", {}, __func__, __FILE__,
           __LINE__);
  }
  auto major = uint(0);
  auto minor = uint(0);
  char git_tag[8];
  iio_library_get_version(&major, &minor, git_tag);
  lprint("libiio version",
         {" major='", fmt::format("{}", major), "'", " minor='",
          fmt::format("{}", minor), "'", " git_tag='",
          fmt::format("{}", git_tag), "'"},
         __func__, __FILE__, __LINE__);
  lprint("",
         {" iio_context_get_devices_count(ctx)='",
          fmt::format("{}", iio_context_get_devices_count(ctx)), "'"},
         __func__, __FILE__, __LINE__);
  auto rx = iio_context_find_device(ctx, "cf-ad9361-lpc");
  if ((nullptr) == (rx)) {
    lprint("error: rx = iio_context_find_device(ctx, 'cf-ad9361-lpc')", {},
           __func__, __FILE__, __LINE__);
  }
  lprint("rx",
         {" iio_device_get_attrs_count(rx)='",
          fmt::format("{}", iio_device_get_attrs_count(rx)), "'"},
         __func__, __FILE__, __LINE__);
  auto phy = iio_context_find_device(ctx, "ad9361-phy");
  if ((nullptr) == (phy)) {
    lprint("error: phy = iio_context_find_device(ctx, 'ad9361-phy')", {},
           __func__, __FILE__, __LINE__);
  }
  lprint("phy",
         {" iio_device_get_attrs_count(phy)='",
          fmt::format("{}", iio_device_get_attrs_count(phy)), "'"},
         __func__, __FILE__, __LINE__);
  auto rx_lo_freq = 2467000000ULL;
  auto rx_lo_freq_MHz = ((rx_lo_freq) / ((1.0e+6f)));
  lprint("", {" rx_lo_freq_MHz='", fmt::format("{}", rx_lo_freq_MHz), "'"},
         __func__, __FILE__, __LINE__);
  iio_channel_attr_write_longlong(
      iio_device_find_channel(phy, "altvoltage0", true), "frequency",
      rx_lo_freq);
  auto rx_rate = 61440000UL;
  auto rx_rate_MSps = ((rx_rate) / ((1.0e+6f)));
  lprint("", {" rx_rate_MSps='", fmt::format("{}", rx_rate_MSps), "'"},
         __func__, __FILE__, __LINE__);
  iio_channel_attr_write_longlong(
      iio_device_find_channel(phy, "voltage0", false), "sampling_frequency",
      rx_rate);
  lprint("get channels count..", {}, __func__, __FILE__, __LINE__);
  auto n_chan = iio_device_get_channels_count(rx);
  lprint("", {" n_chan='", fmt::format("{}", n_chan), "'"}, __func__, __FILE__,
         __LINE__);
  auto rx_i = iio_device_get_channel(rx, 0);
  lprint("rx_i 0",
         {" iio_channel_get_attrs_count(rx_i)='",
          fmt::format("{}", iio_channel_get_attrs_count(rx_i)), "'"},
         __func__, __FILE__, __LINE__);
  auto rx_q = iio_device_get_channel(rx, 1);
  lprint("rx_q 1",
         {" iio_channel_get_attrs_count(rx_q)='",
          fmt::format("{}", iio_channel_get_attrs_count(rx_q)), "'"},
         __func__, __FILE__, __LINE__);
  iio_channel_enable(rx_i);
  iio_channel_enable(rx_q);
  lprint("iq channels enabled", {}, __func__, __FILE__, __LINE__);
  auto const nbuf = ((48) * (64) * (4096));
  lprint("create buffer", {}, __func__, __FILE__, __LINE__);
  auto rxbuf = iio_device_create_buffer(rx, nbuf, false);
  auto sample_and_compute_start =
      std::chrono::high_resolution_clock::now().time_since_epoch();
  auto sample_start = sample_and_compute_start;
  auto compute_start = sample_and_compute_start;
  auto server_thread = run_server_in_new_thread();
  lprint("server started", {}, __func__, __FILE__, __LINE__);
  auto count = 0;
  while (true) {
    sample_start = std::chrono::high_resolution_clock::now().time_since_epoch();
    auto nbytes = iio_buffer_refill(rxbuf);
    auto time_now =
        std::chrono::high_resolution_clock::now().time_since_epoch();
    auto sample_dur = ((time_now) - (sample_start)).count();
    auto step = iio_buffer_step(rxbuf);
    auto end = iio_buffer_end(rxbuf);
    auto start = static_cast<uint8_t *>(iio_buffer_first(rxbuf, rx_i));
    auto i = 0;
    compute_start =
        std::chrono::high_resolution_clock::now().time_since_epoch();
    auto ma = 0;
    auto old = (0.f);
    auto trig = 0;
    auto trig1 = 0;
    //  the ivdep pragma asserts that there are no loop-carried dependencies
    //  which would prevent that consecutive iterations of the following loop
    //  can be executed concurrently with SIMD (single instruction multiple
    //  data) instructions.
    ;
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
    lprint("line604",
           {" count='", fmt::format("{}", count), "'", " nbytes='",
            fmt::format("{}", nbytes), "'", " compute_perc='",
            fmt::format("{}", compute_perc), "'", " sample_perc='",
            fmt::format("{}", sample_perc), "'"},
           __func__, __FILE__, __LINE__);
  }
  state._server_keep_running = false;
  if (server_thread.joinable()) {
    server_thread.join();
  }
  iio_context_destroy(ctx);
  return 0;
}