#include "FFT/fft.h"
#include "glm/ext/matrix_transform.hpp"
#include "renderers/Signal.h"
#include <GLFW/glfw3.h>
#include <Shader/Shader.h>
#include <cmath>
#include <filesystem>
#include <glad/glad.h>
#include <iostream>
#include <ostream>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float zoom = 1.0f;
bool latch = false;
int sampleCount = 512;
std::string file;
float rate = 100.0;
Signal *signal;

static void error_callback(int error, const char *description) {
  std::cout << description << "\n";
}

void resetSignal(Signal &signal) {
  signal.sample(file, rate, sampleCount);
  signal.process();
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
    cameraPos.x += cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos.x -= cameraSpeed;
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (action != GLFW_PRESS)
    return;

  if (key == GLFW_KEY_9) {
    sampleCount *= 2;
    sampleCount = glm::clamp(sampleCount, 2, 8192);
    resetSignal(*signal);
  } else if (key == GLFW_KEY_0) {
    sampleCount /= 2;
    sampleCount = glm::clamp(sampleCount, 2, 8192);
    resetSignal(*signal);
  }

  if (key == GLFW_KEY_L)
    latch = !latch;

  if (key == GLFW_KEY_N) {
    rate += rate / 10;
    resetSignal(*signal);
  }
  if (key == GLFW_KEY_B) {
    rate -= rate / 10;
    resetSignal(*signal);
  }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  float zoomSpeed = 0.1f;
  zoom *= (1.0f + yoffset * zoomSpeed);

  // Clamp safely
  zoom = glm::clamp(zoom, 0.0001f, 100.0f);
}

void printUsage(const char *programName) {
  std::cout << "Usage:\n"
            << "  " << programName << " <file_path>\n\n"
            << "Arguments:\n"
            << "  file_path   Path to input file\n";
}

bool fileExists(const std::filesystem::path &path) {
  return std::filesystem::exists(path) &&
         std::filesystem::is_regular_file(path);
}

int main(int argc, char *argv[]) {
  if ((std::string)argv[1] == "--help") {
    printUsage(argv[0]);
    return 0;
  }
  if (argc != 2) {
    printUsage(argv[0]);
    return 1;
  }

  std::string filePath = argv[1];
  if (!fileExists(filePath)) {
    std::cerr << "Error: file does not exist: " << filePath << "\n";
    return 1;
  }
  file = filePath;

  glfwSetErrorCallback(error_callback);
  if (!glfwInit()) {
    std::cout << "glfw init failed \n";
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  auto *window = glfwCreateWindow(1080, 1080, "FFT Canvas", NULL, NULL);
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
  glfwSetKeyCallback(window, keyCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Signal local;
  signal = &local;
  signal->initialize();
  signal->sample(filePath, rate, 512);
  signal->process();

  glm::mat4 view;

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    float currentFrame = glfwGetTime();
    deltaTime = (currentFrame - lastFrame);
    lastFrame = currentFrame;
    signal->update(deltaTime);

    view = glm::mat4(1.0);
    if (latch) {
      view =
          glm::translate(view, glm::vec3(-signal->getTip().real() * zoom,
                                         -signal->getTip().imag() * zoom, 0.0));
    } else {
      view = glm::translate(view, cameraPos);
    }
    view = glm::scale(view, glm::vec3(zoom, zoom, 1.0f));
    signal->draw(view);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  signal->reset();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
