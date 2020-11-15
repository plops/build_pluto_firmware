
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
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[0], state._sample_binary[1], state._sample_binary[2],
      state._sample_binary[3], state._sample_binary[4], state._sample_binary[5],
      state._sample_binary[6], state._sample_binary[7], state._sample_binary[8],
      state._sample_binary[9], state._sample_binary[10],
      state._sample_binary[11], state._sample_binary[12],
      state._sample_binary[13], state._sample_binary[14],
      state._sample_binary[15]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[16], state._sample_binary[17],
      state._sample_binary[18], state._sample_binary[19],
      state._sample_binary[20], state._sample_binary[21],
      state._sample_binary[22], state._sample_binary[23],
      state._sample_binary[24], state._sample_binary[25],
      state._sample_binary[26], state._sample_binary[27],
      state._sample_binary[28], state._sample_binary[29],
      state._sample_binary[30], state._sample_binary[31]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[32], state._sample_binary[33],
      state._sample_binary[34], state._sample_binary[35],
      state._sample_binary[36], state._sample_binary[37],
      state._sample_binary[38], state._sample_binary[39],
      state._sample_binary[40], state._sample_binary[41],
      state._sample_binary[42], state._sample_binary[43],
      state._sample_binary[44], state._sample_binary[45],
      state._sample_binary[46], state._sample_binary[47]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[48], state._sample_binary[49],
      state._sample_binary[50], state._sample_binary[51],
      state._sample_binary[52], state._sample_binary[53],
      state._sample_binary[54], state._sample_binary[55],
      state._sample_binary[56], state._sample_binary[57],
      state._sample_binary[58], state._sample_binary[59],
      state._sample_binary[60], state._sample_binary[61],
      state._sample_binary[62], state._sample_binary[63]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[64], state._sample_binary[65],
      state._sample_binary[66], state._sample_binary[67],
      state._sample_binary[68], state._sample_binary[69],
      state._sample_binary[70], state._sample_binary[71],
      state._sample_binary[72], state._sample_binary[73],
      state._sample_binary[74], state._sample_binary[75],
      state._sample_binary[76], state._sample_binary[77],
      state._sample_binary[78], state._sample_binary[79]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[80], state._sample_binary[81],
      state._sample_binary[82], state._sample_binary[83],
      state._sample_binary[84], state._sample_binary[85],
      state._sample_binary[86], state._sample_binary[87],
      state._sample_binary[88], state._sample_binary[89],
      state._sample_binary[90], state._sample_binary[91],
      state._sample_binary[92], state._sample_binary[93],
      state._sample_binary[94], state._sample_binary[95]);
  ImGui::Text("valid");
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[0], state._sample_valid[1], state._sample_valid[2],
      state._sample_valid[3], state._sample_valid[4], state._sample_valid[5],
      state._sample_valid[6], state._sample_valid[7], state._sample_valid[8],
      state._sample_valid[9], state._sample_valid[10], state._sample_valid[11],
      state._sample_valid[12], state._sample_valid[13], state._sample_valid[14],
      state._sample_valid[15]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[16], state._sample_valid[17], state._sample_valid[18],
      state._sample_valid[19], state._sample_valid[20], state._sample_valid[21],
      state._sample_valid[22], state._sample_valid[23], state._sample_valid[24],
      state._sample_valid[25], state._sample_valid[26], state._sample_valid[27],
      state._sample_valid[28], state._sample_valid[29], state._sample_valid[30],
      state._sample_valid[31]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[32], state._sample_valid[33], state._sample_valid[34],
      state._sample_valid[35], state._sample_valid[36], state._sample_valid[37],
      state._sample_valid[38], state._sample_valid[39], state._sample_valid[40],
      state._sample_valid[41], state._sample_valid[42], state._sample_valid[43],
      state._sample_valid[44], state._sample_valid[45], state._sample_valid[46],
      state._sample_valid[47]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[48], state._sample_valid[49], state._sample_valid[50],
      state._sample_valid[51], state._sample_valid[52], state._sample_valid[53],
      state._sample_valid[54], state._sample_valid[55], state._sample_valid[56],
      state._sample_valid[57], state._sample_valid[58], state._sample_valid[59],
      state._sample_valid[60], state._sample_valid[61], state._sample_valid[62],
      state._sample_valid[63]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[64], state._sample_valid[65], state._sample_valid[66],
      state._sample_valid[67], state._sample_valid[68], state._sample_valid[69],
      state._sample_valid[70], state._sample_valid[71], state._sample_valid[72],
      state._sample_valid[73], state._sample_valid[74], state._sample_valid[75],
      state._sample_valid[76], state._sample_valid[77], state._sample_valid[78],
      state._sample_valid[79]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_valid[80], state._sample_valid[81], state._sample_valid[82],
      state._sample_valid[83], state._sample_valid[84], state._sample_valid[85],
      state._sample_valid[86], state._sample_valid[87], state._sample_valid[88],
      state._sample_valid[89], state._sample_valid[90], state._sample_valid[91],
      state._sample_valid[92], state._sample_valid[93], state._sample_valid[94],
      state._sample_valid[95]);
  ImGui::End();
  auto b = true;
  ImGui::ShowDemoWindow(&b);
  ImGui::Render();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}