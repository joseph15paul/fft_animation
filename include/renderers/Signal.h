#pragma once

#include "FFT/fft.h"
#include "models/TracePoint.h"
#include "phasor/Phasor.h"
#include "renderers/PhasorRenderer.h"
#include <vector>

class Signal {
  std::vector<Phasor> phasors;
  complexVector samples;
  float samplingRate = 1.0f;
  int numberOfSamples = 0;
  PhasorRenderer renderer;
  std::vector<TracePoint> trace;
  Shader shader;
  unsigned int vao;
  unsigned int vbo;
  std::complex<float> tip = {0.0, 0.0};

public:
  Signal(Shader shader = Shader("src/shaders/Trace/shader.vert",
                                "src/shaders/Trace/shader.frag"));
  ~Signal();

  void initialize();
  void sample(std::string filePath, float samplingRateHZ, int numberOfSamples);
  void process();
  void update(float dt);
  void draw(glm::mat4 transform = glm::mat4(1.0));
  std::complex<float> getTip() { return tip; };
  void reset();

private:
  void renderTrace(glm::mat4 mvp);
  void updateTrace(float dt);
};
