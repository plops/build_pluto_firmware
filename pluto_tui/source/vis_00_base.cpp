
#include "utils.h"

#include "globals.h"

extern State state;
#include <array>
#include <chrono>
#include <complex>
#include <fftw3.h>
#include <iio.h>
#include <iostream>
#include <math.h>
#include <thread>
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
  state._code_version = "90eba7a3311e4f941c71bd1ed8a7ef3b58638f01";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/pluto_tui";
  state._code_author = "Martin Kielhorn <kielhorn.martin@gmail.com>";
  state._code_license = "GPL v3";
  state._code_generation_time = "22:48:15 of Monday, 2020-10-26 (GMT+1)";
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
      << (std::setw(8)) << (" major='") << (major) << ("'") << (std::setw(8))
      << (" minor='") << (minor) << ("'") << (std::setw(8)) << (" git_tag='")
      << (git_tag) << ("'") << (std::endl) << (std::flush);
  if (!(ctx)) {

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("create_default") << (" ") << (std::setw(8)) << (" ctx='")
                << (ctx) << ("'") << (std::endl) << (std::flush);
  }

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" iio_context_get_devices_count(ctx)='")
      << (iio_context_get_devices_count(ctx)) << ("'") << (std::endl)
      << (std::flush);
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
      << (iio_device_get_attrs_count(rx)) << ("'") << (std::endl)
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
      << (iio_device_get_attrs_count(phy)) << ("'") << (std::endl)
      << (std::flush);
  auto n_chan = iio_device_get_channels_count(rx);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" n_chan='") << (n_chan) << ("'") << (std::endl)
      << (std::flush);
  auto rx_i = iio_device_get_channel(rx, 0);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("rx_i 0") << (" ")
      << (std::setw(8)) << (" iio_channel_get_attrs_count(rx_i)='")
      << (iio_channel_get_attrs_count(rx_i)) << ("'") << (std::endl)
      << (std::flush);
  auto rx_q = iio_device_get_channel(rx, 1);

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("rx_q 1") << (" ")
      << (std::setw(8)) << (" iio_channel_get_attrs_count(rx_q)='")
      << (iio_channel_get_attrs_count(rx_q)) << ("'") << (std::endl)
      << (std::flush);
  iio_channel_enable(rx_i);
  iio_channel_enable(rx_q);
  auto const nbuf = 1024;
  auto input = static_cast<fftwf_complex *>(
      fftwf_malloc(((nbuf) * (sizeof(fftwf_complex)))));
  auto output = static_cast<fftwf_complex *>(
      fftwf_malloc(((nbuf) * (sizeof(fftwf_complex)))));
  auto plan =
      fftwf_plan_dft_1d(nbuf, input, output, FFTW_FORWARD, FFTW_MEASURE);
  auto rxbuf = iio_device_create_buffer(rx, nbuf, false);
  auto sample_and_compute_start =
      std::chrono::high_resolution_clock::now().time_since_epoch();
  auto sample_start = sample_and_compute_start;
  auto compute_start = sample_and_compute_start;
  for (auto j = 0; (j) < (100); (j) += (1)) {
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
    for (uint8_t *p = start; (p) < (end); (p) += (step)) {
      auto si = reinterpret_cast<int16_t *>(p)[0];
      auto sq = reinterpret_cast<int16_t *>(p)[1];
      input[i][0] = si;
      input[i][1] = sq;
      (i)++;
    }
    fftwf_execute(plan);
    auto compute_end =
        std::chrono::high_resolution_clock::now().time_since_epoch();
    auto compute_dur = ((compute_end) - (compute_start)).count();
    auto compute_samp_dur = ((compute_end) - (sample_start)).count();
    auto compute_perc = ((((100) * (compute_dur))) / (compute_samp_dur));
    auto sample_perc = ((((100) * (sample_dur))) / (compute_samp_dur));

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ") << ("")
                << (" ") << (std::setw(8)) << (" compute_perc='")
                << (compute_perc) << ("'") << (std::setw(8))
                << (" sample_perc='") << (sample_perc) << ("'")
                << (std::setw(8)) << (" compute_samp_dur='")
                << (compute_samp_dur) << ("'") << (std::endl) << (std::flush);
  }
  fftwf_destroy_plan(plan);
  fftwf_free(input);
  fftwf_free(output);
  iio_context_destroy(ctx);
  return 0;
}