
#include "utils.h"

#include "globals.h"

extern State state;
// https://youtu.be/nVaQuNXueFw?t=317
// https://blog.conan.io/2019/06/26/An-introduction-to-the-Dear-ImGui-library.html
#include <algorithm>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl2.h>
#include <fstream>
#include <imgui.h>
#include <iostream>
#include <string>
void initGui() {

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("initGui") << (" ")
      << (std::endl) << (std::flush);
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(state._window, true);
  ImGui_ImplOpenGL2_Init();
  ImGui::StyleColorsDark();
}
void cleanupGui() {
  ImGui_ImplOpenGL2_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
void drawGui() {
  ImGui_ImplOpenGL2_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  ImGui::Begin("snapped_cursor");
  ImGui::Text("x: %04d y: %04d",
              static_cast<int>(state._snapped_world_cursor[0]),
              static_cast<int>(state._snapped_world_cursor[1]));
  ImGui::Text("iqdata_bytes: %d", state._iqdata_bytes);
  ImGui::Text("sample_offset: %f", state._sample_offset);
  ImGui::Text("sample_threshold: %f", state._sample_threshold);
  ImGui::Text("sample_binary size: %zu", state._sample_binary.size());
  auto first_valid_byte = 0;
  auto first_valid_bit = 0;
  auto zero_count = 0;
  auto byte_count = 0;
  auto fsm_state = 0;
  if ((12) < (state._sample_valid.size())) {
    for (auto i = 0; (i) < (state._sample_valid.size()); (i) += (1)) {
      auto v = state._sample_valid[i];
      switch (fsm_state) {
      case 0: {
        if ((0) == (v)) {
          fsm_state = 1;
          zero_count = 1;
        }
        break;
      }
      case 1: {
        if ((0) == (v)) {
          (zero_count)++;
        } else {
          fsm_state = 0;
        }
        if ((zero_count) == (8)) {
          fsm_state = 2;
        }
        break;
      }
      case 2: {
        if ((0) < (v)) {
          fsm_state = 3;
        }
        break;
      }
      }
      (byte_count)++;
      if ((3) == (fsm_state)) {
        break;
      }
    }
  }
  ImGui::Text("first valid byte: %d", byte_count);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[((byte_count) + (0))],
      state._sample_binary[((byte_count) + (1))],
      state._sample_binary[((byte_count) + (2))],
      state._sample_binary[((byte_count) + (3))],
      state._sample_binary[((byte_count) + (4))],
      state._sample_binary[((byte_count) + (5))],
      state._sample_binary[((byte_count) + (6))],
      state._sample_binary[((byte_count) + (7))],
      state._sample_binary[((byte_count) + (8))],
      state._sample_binary[((byte_count) + (9))],
      state._sample_binary[((byte_count) + (10))],
      state._sample_binary[((byte_count) + (11))],
      state._sample_binary[((byte_count) + (12))],
      state._sample_binary[((byte_count) + (13))],
      state._sample_binary[((byte_count) + (14))],
      state._sample_binary[((byte_count) + (15))]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[((byte_count) + (16))],
      state._sample_binary[((byte_count) + (17))],
      state._sample_binary[((byte_count) + (18))],
      state._sample_binary[((byte_count) + (19))],
      state._sample_binary[((byte_count) + (20))],
      state._sample_binary[((byte_count) + (21))],
      state._sample_binary[((byte_count) + (22))],
      state._sample_binary[((byte_count) + (23))],
      state._sample_binary[((byte_count) + (24))],
      state._sample_binary[((byte_count) + (25))],
      state._sample_binary[((byte_count) + (26))],
      state._sample_binary[((byte_count) + (27))],
      state._sample_binary[((byte_count) + (28))],
      state._sample_binary[((byte_count) + (29))],
      state._sample_binary[((byte_count) + (30))],
      state._sample_binary[((byte_count) + (31))]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[((byte_count) + (32))],
      state._sample_binary[((byte_count) + (33))],
      state._sample_binary[((byte_count) + (34))],
      state._sample_binary[((byte_count) + (35))],
      state._sample_binary[((byte_count) + (36))],
      state._sample_binary[((byte_count) + (37))],
      state._sample_binary[((byte_count) + (38))],
      state._sample_binary[((byte_count) + (39))],
      state._sample_binary[((byte_count) + (40))],
      state._sample_binary[((byte_count) + (41))],
      state._sample_binary[((byte_count) + (42))],
      state._sample_binary[((byte_count) + (43))],
      state._sample_binary[((byte_count) + (44))],
      state._sample_binary[((byte_count) + (45))],
      state._sample_binary[((byte_count) + (46))],
      state._sample_binary[((byte_count) + (47))]);
  ImGui::Text("valid");
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[((byte_count) + (0))],
      state._sample_valid[((byte_count) + (1))],
      state._sample_valid[((byte_count) + (2))],
      state._sample_valid[((byte_count) + (3))],
      state._sample_valid[((byte_count) + (4))],
      state._sample_valid[((byte_count) + (5))],
      state._sample_valid[((byte_count) + (6))],
      state._sample_valid[((byte_count) + (7))],
      state._sample_valid[((byte_count) + (8))],
      state._sample_valid[((byte_count) + (9))],
      state._sample_valid[((byte_count) + (10))],
      state._sample_valid[((byte_count) + (11))],
      state._sample_valid[((byte_count) + (12))],
      state._sample_valid[((byte_count) + (13))],
      state._sample_valid[((byte_count) + (14))],
      state._sample_valid[((byte_count) + (15))]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[((byte_count) + (16))],
      state._sample_valid[((byte_count) + (17))],
      state._sample_valid[((byte_count) + (18))],
      state._sample_valid[((byte_count) + (19))],
      state._sample_valid[((byte_count) + (20))],
      state._sample_valid[((byte_count) + (21))],
      state._sample_valid[((byte_count) + (22))],
      state._sample_valid[((byte_count) + (23))],
      state._sample_valid[((byte_count) + (24))],
      state._sample_valid[((byte_count) + (25))],
      state._sample_valid[((byte_count) + (26))],
      state._sample_valid[((byte_count) + (27))],
      state._sample_valid[((byte_count) + (28))],
      state._sample_valid[((byte_count) + (29))],
      state._sample_valid[((byte_count) + (30))],
      state._sample_valid[((byte_count) + (31))]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[((byte_count) + (32))],
      state._sample_valid[((byte_count) + (33))],
      state._sample_valid[((byte_count) + (34))],
      state._sample_valid[((byte_count) + (35))],
      state._sample_valid[((byte_count) + (36))],
      state._sample_valid[((byte_count) + (37))],
      state._sample_valid[((byte_count) + (38))],
      state._sample_valid[((byte_count) + (39))],
      state._sample_valid[((byte_count) + (40))],
      state._sample_valid[((byte_count) + (41))],
      state._sample_valid[((byte_count) + (42))],
      state._sample_valid[((byte_count) + (43))],
      state._sample_valid[((byte_count) + (44))],
      state._sample_valid[((byte_count) + (45))],
      state._sample_valid[((byte_count) + (46))],
      state._sample_valid[((byte_count) + (47))]);
  ImGui::Text("ascii");
  ImGui::Text(
      " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c",
      state._sample_binary[((byte_count) + (0))],
      state._sample_binary[((byte_count) + (1))],
      state._sample_binary[((byte_count) + (2))],
      state._sample_binary[((byte_count) + (3))],
      state._sample_binary[((byte_count) + (4))],
      state._sample_binary[((byte_count) + (5))],
      state._sample_binary[((byte_count) + (6))],
      state._sample_binary[((byte_count) + (7))],
      state._sample_binary[((byte_count) + (8))],
      state._sample_binary[((byte_count) + (9))],
      state._sample_binary[((byte_count) + (10))],
      state._sample_binary[((byte_count) + (11))],
      state._sample_binary[((byte_count) + (12))],
      state._sample_binary[((byte_count) + (13))],
      state._sample_binary[((byte_count) + (14))],
      state._sample_binary[((byte_count) + (15))]);
  ImGui::Text(
      " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c",
      state._sample_binary[((byte_count) + (16))],
      state._sample_binary[((byte_count) + (17))],
      state._sample_binary[((byte_count) + (18))],
      state._sample_binary[((byte_count) + (19))],
      state._sample_binary[((byte_count) + (20))],
      state._sample_binary[((byte_count) + (21))],
      state._sample_binary[((byte_count) + (22))],
      state._sample_binary[((byte_count) + (23))],
      state._sample_binary[((byte_count) + (24))],
      state._sample_binary[((byte_count) + (25))],
      state._sample_binary[((byte_count) + (26))],
      state._sample_binary[((byte_count) + (27))],
      state._sample_binary[((byte_count) + (28))],
      state._sample_binary[((byte_count) + (29))],
      state._sample_binary[((byte_count) + (30))],
      state._sample_binary[((byte_count) + (31))]);
  ImGui::Text(
      " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c",
      state._sample_binary[((byte_count) + (32))],
      state._sample_binary[((byte_count) + (33))],
      state._sample_binary[((byte_count) + (34))],
      state._sample_binary[((byte_count) + (35))],
      state._sample_binary[((byte_count) + (36))],
      state._sample_binary[((byte_count) + (37))],
      state._sample_binary[((byte_count) + (38))],
      state._sample_binary[((byte_count) + (39))],
      state._sample_binary[((byte_count) + (40))],
      state._sample_binary[((byte_count) + (41))],
      state._sample_binary[((byte_count) + (42))],
      state._sample_binary[((byte_count) + (43))],
      state._sample_binary[((byte_count) + (44))],
      state._sample_binary[((byte_count) + (45))],
      state._sample_binary[((byte_count) + (46))],
      state._sample_binary[((byte_count) + (47))]);
  ImGui::Text("ascii~");
  ImGui::Text(
      " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c",
      ~(state._sample_binary[((byte_count) + (0))]),
      ~(state._sample_binary[((byte_count) + (1))]),
      ~(state._sample_binary[((byte_count) + (2))]),
      ~(state._sample_binary[((byte_count) + (3))]),
      ~(state._sample_binary[((byte_count) + (4))]),
      ~(state._sample_binary[((byte_count) + (5))]),
      ~(state._sample_binary[((byte_count) + (6))]),
      ~(state._sample_binary[((byte_count) + (7))]),
      ~(state._sample_binary[((byte_count) + (8))]),
      ~(state._sample_binary[((byte_count) + (9))]),
      ~(state._sample_binary[((byte_count) + (10))]),
      ~(state._sample_binary[((byte_count) + (11))]),
      ~(state._sample_binary[((byte_count) + (12))]),
      ~(state._sample_binary[((byte_count) + (13))]),
      ~(state._sample_binary[((byte_count) + (14))]),
      ~(state._sample_binary[((byte_count) + (15))]));
  ImGui::Text(
      " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c",
      ~(state._sample_binary[((byte_count) + (16))]),
      ~(state._sample_binary[((byte_count) + (17))]),
      ~(state._sample_binary[((byte_count) + (18))]),
      ~(state._sample_binary[((byte_count) + (19))]),
      ~(state._sample_binary[((byte_count) + (20))]),
      ~(state._sample_binary[((byte_count) + (21))]),
      ~(state._sample_binary[((byte_count) + (22))]),
      ~(state._sample_binary[((byte_count) + (23))]),
      ~(state._sample_binary[((byte_count) + (24))]),
      ~(state._sample_binary[((byte_count) + (25))]),
      ~(state._sample_binary[((byte_count) + (26))]),
      ~(state._sample_binary[((byte_count) + (27))]),
      ~(state._sample_binary[((byte_count) + (28))]),
      ~(state._sample_binary[((byte_count) + (29))]),
      ~(state._sample_binary[((byte_count) + (30))]),
      ~(state._sample_binary[((byte_count) + (31))]));
  ImGui::Text(
      " %2c %2c %2c %2c %2c %2c %2c %2c      %2c %2c %2c %2c %2c %2c %2c %2c",
      ~(state._sample_binary[((byte_count) + (32))]),
      ~(state._sample_binary[((byte_count) + (33))]),
      ~(state._sample_binary[((byte_count) + (34))]),
      ~(state._sample_binary[((byte_count) + (35))]),
      ~(state._sample_binary[((byte_count) + (36))]),
      ~(state._sample_binary[((byte_count) + (37))]),
      ~(state._sample_binary[((byte_count) + (38))]),
      ~(state._sample_binary[((byte_count) + (39))]),
      ~(state._sample_binary[((byte_count) + (40))]),
      ~(state._sample_binary[((byte_count) + (41))]),
      ~(state._sample_binary[((byte_count) + (42))]),
      ~(state._sample_binary[((byte_count) + (43))]),
      ~(state._sample_binary[((byte_count) + (44))]),
      ~(state._sample_binary[((byte_count) + (45))]),
      ~(state._sample_binary[((byte_count) + (46))]),
      ~(state._sample_binary[((byte_count) + (47))]));
  ImGui::End();
  auto b = true;
  ImGui::ShowDemoWindow(&b);
  ImGui::Render();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}