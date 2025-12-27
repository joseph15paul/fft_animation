#include "Circle/circle.h"
#include "FFT/fft.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "models/Vertex.h"
#include "phasor/Phasor.h"
#include "renderers/PhasorRenderer.h"
#include "renderers/Signal.h"
#include <GLFW/glfw3.h>
#include <Shader/Shader.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <ostream>

static void error_callback(int error, const char *description) {
  std::cout << description << "\n";
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main() {
  auto signal = std::vector<std::complex<float>>{1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  addPowerOf2Padding(signal);
  std::cout << "signal: ";
  printComplexVector(signal);

  auto freqs = fft(signal);
  std::cout << "frequency domain: ";
  printComplexVector(freqs);

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    std::cout << "glfw init failed \n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  auto *window = glfwCreateWindow(1080, 1080, "MY WINDOW", NULL, NULL);
  if (!window) {
    std::cout << "window creation failed \n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  glfwSwapInterval(1);

  auto centre = Vertex{glm::vec3{-0.25f, 0.0f, 0.0f},
                       glm::vec3{1.0f, 1.0f, 1.0f}, glm::vec2{0.0f, 0.0f}};

  auto phasor = Phasor(1.0f, 5.0f);
  float aspect = (float)1920 / (float)1080;

  glm::mat4 projection = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
  auto last = glfwGetTime();
  glm::mat4 trans = glm::mat4(1.0f);
  trans = glm::translate(trans, glm::vec3(0.1f, 0.0f, 0.0f));
  auto sp = Phasor(2.0f, 1.0f);
  auto signl = Signal();
  signl.sample("", 1);
  signl.process();
  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Circle(0.55f, centre).draw();
    auto now = glfwGetTime();
    auto dt = now - last;
    last = now;
    /* PhasorRenderer().draw(phasor, trans);
     auto transf =
         glm::translate(trans, glm::vec3(phasor.getComplex().real() / 10,
                                         phasor.getComplex().imag() / 10, 0.0));
     sp.update(dt);
     PhasorRenderer().draw(sp, transf);
 */

    signl.draw(dt);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
