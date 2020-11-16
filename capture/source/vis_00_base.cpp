
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
int main(int argc, char **argv) {
  setlocale(LC_ALL, "");
  state._code_version = "067b9063bae5f92d1b2d59fcad7ebff0667fd6cd";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/capture";
  state._code_author = "Martin Kielhorn <kielhorn.martin@gmail.com>";
  state._code_license = "GPL v3";
  state._code_generation_time = "19:43:52 of Monday, 2020-11-16 (GMT+1)";
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
  auto ctx = iio_create_default_context();
  auto major = uint(0);
  auto minor = uint(0);
  char git_tag[8];
  iio_library_get_version(&major, &minor, git_tag);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" major='") << (major) << ("::")
      << (typeid(major).name()) << ("'") << (std::setw(8)) << (" minor='")
      << (minor) << ("::") << (typeid(minor).name()) << ("'") << (std::setw(8))
      << (" git_tag='") << (git_tag) << ("::") << (typeid(git_tag).name())
      << ("'") << (std::endl) << (std::flush);
  if (!(ctx)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("create_default") << (" ") << (std::setw(8)) << (" ctx='")
                << (ctx) << ("::") << (typeid(ctx).name()) << ("'")
                << (std::endl) << (std::flush);
  }

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" iio_context_get_devices_count(ctx)='")
      << (iio_context_get_devices_count(ctx)) << ("::")
      << (typeid(iio_context_get_devices_count(ctx)).name()) << ("'")
      << (std::endl) << (std::flush);
  auto rx = iio_context_find_device(ctx, "cf-ad9361-lpc");
  if (!(rx)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ") << ("rx")
                << (" ") << (std::endl) << (std::flush);
  }

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("rx") << (" ")
      << (std::setw(8)) << (" iio_device_get_attrs_count(rx)='")
      << (iio_device_get_attrs_count(rx)) << ("::")
      << (typeid(iio_device_get_attrs_count(rx)).name()) << ("'") << (std::endl)
      << (std::flush);
  auto phy = iio_context_find_device(ctx, "ad9361-phy");
  if (!(phy)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("phy") << (" ") << (std::endl) << (std::flush);
  }

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("phy") << (" ")
      << (std::setw(8)) << (" iio_device_get_attrs_count(phy)='")
      << (iio_device_get_attrs_count(phy)) << ("::")
      << (typeid(iio_device_get_attrs_count(phy)).name()) << ("'")
      << (std::endl) << (std::flush);
  auto rx_lo_freq = 2420000000ULL;
  auto rx_lo_freq_MHz = ((rx_lo_freq) / ((1.0e+6f)));

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" rx_lo_freq_MHz='") << (rx_lo_freq_MHz) << ("::")
      << (typeid(rx_lo_freq_MHz).name()) << ("'") << (std::endl)
      << (std::flush);
  iio_channel_attr_write_longlong(
      iio_device_find_channel(phy, "altvoltage0", true), "frequency",
      rx_lo_freq);
  auto rx_rate = 61440000UL;
  auto rx_rate_MSps = ((rx_rate) / ((1.0e+6f)));

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" rx_rate_MSps='") << (rx_rate_MSps) << ("::")
      << (typeid(rx_rate_MSps).name()) << ("'") << (std::endl) << (std::flush);
  iio_channel_attr_write_longlong(
      iio_device_find_channel(phy, "voltage0", false), "sampling_frequency",
      rx_rate);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("get channels count..")
      << (" ") << (std::endl) << (std::flush);
  auto n_chan = iio_device_get_channels_count(rx);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" n_chan='") << (n_chan) << ("::")
      << (typeid(n_chan).name()) << ("'") << (std::endl) << (std::flush);
  auto rx_i = iio_device_get_channel(rx, 0);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("rx_i 0") << (" ")
      << (std::setw(8)) << (" iio_channel_get_attrs_count(rx_i)='")
      << (iio_channel_get_attrs_count(rx_i)) << ("::")
      << (typeid(iio_channel_get_attrs_count(rx_i)).name()) << ("'")
      << (std::endl) << (std::flush);
  auto rx_q = iio_device_get_channel(rx, 1);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("rx_q 1") << (" ")
      << (std::setw(8)) << (" iio_channel_get_attrs_count(rx_q)='")
      << (iio_channel_get_attrs_count(rx_q)) << ("::")
      << (typeid(iio_channel_get_attrs_count(rx_q)).name()) << ("'")
      << (std::endl) << (std::flush);
  iio_channel_enable(rx_i);
  iio_channel_enable(rx_q);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("iq channels enabled")
      << (" ") << (std::endl) << (std::flush);
  auto const nbuf = ((48) * (64) * (4096));

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("create buffer")
      << (" ") << (std::endl) << (std::flush);
  auto rxbuf = iio_device_create_buffer(rx, nbuf, false);
  auto sample_and_compute_start =
      std::chrono::high_resolution_clock::now().time_since_epoch();
  auto sample_start = sample_and_compute_start;
  auto compute_start = sample_and_compute_start;
  state._iq_out.push_back(42);
  state._iq_out.push_back(43);
  auto server_thread = run_server_in_new_thread();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("server started")
      << (" ") << (std::endl) << (std::flush);
  if (server_thread.joinable()) {
    server_thread.join();
  }
  iio_context_destroy(ctx);
  return 0;
}