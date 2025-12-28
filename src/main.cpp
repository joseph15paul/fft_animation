#include "FFT/fft.h"
#include "glm/ext/matrix_transform.hpp"
#include "renderers/Signal.h"
#include <GLFW/glfw3.h>
#include <Shader/Shader.h>
#include <cmath>
#include <glad/glad.h>
#include <iostream>
#include <ostream>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float zoom = 1.0f;

static void error_callback(int error, const char *description) {
  std::cout << description << "\n";
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  const float cameraSpeed = deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos.y += cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos.y -= cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos.x -= cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos.x += cameraSpeed;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  float zoomSpeed = 0.1f;
  zoom *= (1.0f + yoffset * zoomSpeed);

  // Clamp safely
  zoom = glm::clamp(zoom, 0.0001f, 100.0f);
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
  glfwSetScrollCallback(window, scroll_callback);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  auto signl = Signal();
  signl.sample("/home/joseph/Downloads/orca.svg", 100, 4096);
  signl.process();

  glm::mat4 view = glm::mat4(1.0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    view = glm::mat4(1.0);
    view = glm::translate(view, cameraPos);
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    signl.draw(deltaTime, view);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  signl.reset();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
