
#include "utils.h"

#include "globals.h"

extern State state;
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if ((((((key) == (GLFW_KEY_ESCAPE)) || ((key) == (GLFW_KEY_Q)))) &&
       ((action) == (GLFW_PRESS)))) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if ((((key) == (GLFW_KEY_UP)) && ((action) == (GLFW_PRESS)))) {
    (state._sample_offset)++;
  }
  if ((((key) == (GLFW_KEY_DOWN)) && ((action) == (GLFW_PRESS)))) {
    (state._sample_offset)--;
  }
  if ((((key) == (GLFW_KEY_1)) && ((action) == (GLFW_PRESS)))) {
    state._sample_threshold = ((state._sample_threshold) * (((1) - ((0.10f)))));
  }
  if ((((key) == (GLFW_KEY_2)) && ((action) == (GLFW_PRESS)))) {
    state._sample_threshold = ((state._sample_threshold) * (((1) + ((0.10f)))));
  }
}
void errorCallback(int err, const char *description) {

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("error") << (" ")
      << (std::setw(8)) << (" err='") << (err) << ("::") << (typeid(err).name())
      << ("'") << (std::setw(8)) << (" description='") << (description)
      << ("::") << (typeid(description).name()) << ("'") << (std::endl)
      << (std::flush);
}
static void framebufferResizeCallback(GLFWwindow *window, int width,
                                      int height) {

  (std::cout)
      << (std::setw(10))
      << (std::chrono::high_resolution_clock::now().time_since_epoch().count())
      << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__) << (":")
      << (__LINE__) << (" ") << (__func__) << (" ") << ("resize") << (" ")
      << (std::setw(8)) << (" width='") << (width) << ("::")
      << (typeid(width).name()) << ("'") << (std::setw(8)) << (" height='")
      << (height) << ("::") << (typeid(height).name()) << ("'") << (std::endl)
      << (std::flush);
  auto app = (State *)(glfwGetWindowUserPointer(window));
  app->_framebufferResized = true;
}
void initWindow() {
  if (glfwInit()) {
    glfwSetErrorCallback(errorCallback);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    std::stringstream label;
    (label) << ("glfw oscilloscope [") << (state._code_generation_time)
            << ("] git:") << (std::fixed) << (std::setprecision(3))
            << (state._main_version);
    state._window = glfwCreateWindow(930, 930, label.str().c_str(), NULL, NULL);

    (std::cout) << (std::setw(10))
                << (std::chrono::high_resolution_clock::now()
                        .time_since_epoch()
                        .count())
                << (" ") << (std::this_thread::get_id()) << (" ") << (__FILE__)
                << (":") << (__LINE__) << (" ") << (__func__) << (" ")
                << ("initWindow") << (" ") << (std::setw(8))
                << (" state._window='") << (state._window) << ("::")
                << (typeid(state._window).name()) << ("'") << (std::setw(8))
                << (" glfwGetVersionString()='") << (glfwGetVersionString())
                << ("::") << (typeid(glfwGetVersionString()).name()) << ("'")
                << (std::endl) << (std::flush);
    glfwSetKeyCallback(state._window, keyCallback);
    glfwSetWindowUserPointer(state._window, &(state));
    glfwSetFramebufferSizeCallback(state._window, framebufferResizeCallback);
    glfwMakeContextCurrent(state._window);
    glfwSwapInterval(1);
  }
}
void cleanupWindow() {
  glfwDestroyWindow(state._window);
  glfwTerminate();
}