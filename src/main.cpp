#include "FFT/fft.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "models/Vertex.h"
#include "phasor/Phasor.h"
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

  auto signl = Signal();
  signl.sample("/home/joseph/Downloads/clover.svg", 1, 50);
  signl.process();
  auto last = glfwGetTime();
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

  signl.reset();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
