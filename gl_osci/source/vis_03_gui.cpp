
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
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[96], state._sample_binary[97],
      state._sample_binary[98], state._sample_binary[99],
      state._sample_binary[100], state._sample_binary[101],
      state._sample_binary[102], state._sample_binary[103],
      state._sample_binary[104], state._sample_binary[105],
      state._sample_binary[106], state._sample_binary[107],
      state._sample_binary[108], state._sample_binary[109],
      state._sample_binary[110], state._sample_binary[111]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[112], state._sample_binary[113],
      state._sample_binary[114], state._sample_binary[115],
      state._sample_binary[116], state._sample_binary[117],
      state._sample_binary[118], state._sample_binary[119],
      state._sample_binary[120], state._sample_binary[121],
      state._sample_binary[122], state._sample_binary[123],
      state._sample_binary[124], state._sample_binary[125],
      state._sample_binary[126], state._sample_binary[127]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[128], state._sample_binary[129],
      state._sample_binary[130], state._sample_binary[131],
      state._sample_binary[132], state._sample_binary[133],
      state._sample_binary[134], state._sample_binary[135],
      state._sample_binary[136], state._sample_binary[137],
      state._sample_binary[138], state._sample_binary[139],
      state._sample_binary[140], state._sample_binary[141],
      state._sample_binary[142], state._sample_binary[143]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[144], state._sample_binary[145],
      state._sample_binary[146], state._sample_binary[147],
      state._sample_binary[148], state._sample_binary[149],
      state._sample_binary[150], state._sample_binary[151],
      state._sample_binary[152], state._sample_binary[153],
      state._sample_binary[154], state._sample_binary[155],
      state._sample_binary[156], state._sample_binary[157],
      state._sample_binary[158], state._sample_binary[159]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[160], state._sample_binary[161],
      state._sample_binary[162], state._sample_binary[163],
      state._sample_binary[164], state._sample_binary[165],
      state._sample_binary[166], state._sample_binary[167],
      state._sample_binary[168], state._sample_binary[169],
      state._sample_binary[170], state._sample_binary[171],
      state._sample_binary[172], state._sample_binary[173],
      state._sample_binary[174], state._sample_binary[175]);
  ImGui::Text(
      " %2x %2x %2x %2x %2x %2x %2x %2x     %2x %2x %2x %2x %2x %2x %2x %2x",
      state._sample_binary[176], state._sample_binary[177],
      state._sample_binary[178], state._sample_binary[179],
      state._sample_binary[180], state._sample_binary[181],
      state._sample_binary[182], state._sample_binary[183],
      state._sample_binary[184], state._sample_binary[185],
      state._sample_binary[186], state._sample_binary[187],
      state._sample_binary[188], state._sample_binary[189],
      state._sample_binary[190], state._sample_binary[191]);
  ImGui::End();
  auto b = true;
  ImGui::ShowDemoWindow(&b);
  ImGui::Render();
  ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}