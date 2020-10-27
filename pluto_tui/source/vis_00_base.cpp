
#include "utils.h"

#include "globals.h"

extern State state;
#include "imtui-demo.h"
#include "imtui/imtui-impl-ncurses.h"
#include "imtui/imtui.h"
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
  state._code_version = "9ec148cd0350e6025463a11baeeafae413d76634";
  state._code_repository =
      "https://github.com/plops/build_pluto_firmware/tree/master/pluto_tui";
  state._code_author = "Martin Kielhorn <kielhorn.martin@gmail.com>";
  state._code_license = "GPL v3";
  state._code_generation_time = "19:18:36 of Tuesday, 2020-10-27 (GMT+1)";
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
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  auto screen = ImTui_ImplNcurses_Init(true);
  ImTui_ImplText_Init();
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
  auto plan_start =
      std::chrono::high_resolution_clock::now().time_since_epoch();
  auto plan =
      fftwf_plan_dft_1d(nbuf, input, output, FFTW_FORWARD, FFTW_ESTIMATE);
  auto plan_end = std::chrono::high_resolution_clock::now().time_since_epoch();
  auto plan_duration = ((plan_end) - (plan_start)).count();

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("") << (" ")
      << (std::setw(8)) << (" nbuf='") << (nbuf) << ("'") << (std::setw(8))
      << (" plan_duration='") << (plan_duration) << ("'") << (std::setw(8))
      << (" ((8)*(sizeof(fftwf_complex)))='")
      << (((8) * (sizeof(fftwf_complex)))) << ("'") << (std::endl)
      << (std::flush);
  auto rxbuf = iio_device_create_buffer(rx, nbuf, false);
  auto sample_and_compute_start =
      std::chrono::high_resolution_clock::now().time_since_epoch();
  auto sample_start = sample_and_compute_start;
  auto compute_start = sample_and_compute_start;
  bool demo = true;
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
#pragma GCC ivdep
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
    ImTui_ImplNcurses_NewFrame();
    ImTui_ImplText_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(4, 2), ImGuiCond_Once);
    ImGui::SetNextWindowSize(ImVec2((50.f), (10.f)), ImGuiCond_Once);
    ImGui::Begin("pluto rx");
    ImGui::Text("compute_perc     : %2lld%%", compute_perc);
    ImGui::Text("sample_perc      : %2lld%%", sample_perc);
    ImGui::Text("compute_samp_dur : %lld ns", compute_samp_dur);
    ImGui::Text(u8"(▒▓)");
    ImGui::End();
    ImTui::ShowDemoWindow(&demo);
    ImGui::Render();
    ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
    ImTui_ImplNcurses_DrawScreen();
  }
  fftwf_destroy_plan(plan);
  fftwf_free(input);
  fftwf_free(output);
  ImTui_ImplText_Shutdown();
  ImTui_ImplNcurses_Shutdown();
  iio_context_destroy(ctx);
  return 0;
}