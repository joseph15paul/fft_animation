#include "renderers/Signal.h"
#include "Shader/Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "models/TracePoint.h"
#include <algorithm>
#include <cmath>
#include <complex>
#include <cstdlib>
#include <vector>
#define NANOSVG_IMPLEMENTATION
#include "nanoSVG/nanosvg.h"

constexpr float PI = 3.1415927f;
constexpr size_t MAX_TRACE = 20000;

Signal::Signal(Shader shader) : shader(shader) {

}

Signal::~Signal() { reset(); }

void Signal::initialize() {
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, MAX_TRACE * sizeof(TracePoint), trace.data(),
               GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TracePoint),
                        (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(TracePoint),
                        (void *)offsetof(TracePoint, alpha));
  glEnableVertexAttribArray(2);
  glBindVertexArray(0);
}

void Signal::reset() {
  renderer.reset();

  if (vbo != 0) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    vbo = 0;
  }
  if (vao == 0) {
    return;
  }
  glDeleteVertexArrays(1, &vao);
  vao = 0;
}

void Signal::sample(std::string filePath, float samplingRateHZ,
                    int numberOfSamples) {
  samplingRate = 2 * PI * samplingRateHZ;
  samples.clear();
  trace.clear();
  this->numberOfSamples = numberOfSamples;

  NSVGimage *image = nsvgParseFromFile(filePath.c_str(), "px", 96);

  complexVector poly;
  for (NSVGshape *shape = image->shapes; shape; shape = shape->next) {
    for (NSVGpath *path = shape->paths; path; path = path->next) {
      float *pts = path->pts;
      int n = path->npts;

      for (int i = 0; i < n - 1; i++) {
        float x = pts[2 * i];
        float y = pts[2 * i + 1];
        poly.push_back(std::complex<float>{x, -y});
      }
    }
  }

  nsvgDelete(image);

  float total = 0.0f;
  for (size_t i = 1; i < poly.size(); i++) {
    total += std::abs(poly[i] - poly[i - 1]);
  }
  float step = total / numberOfSamples;
  float acc = 0.0f;

  for (size_t i = 1; i < poly.size(); i++) {
    float dx = poly[i].real() - poly[i - 1].real();
    float dy = poly[i].imag() - poly[i - 1].imag();
    float len = std::hypot(dx, dy);

    while (acc + step <= len) {
      acc += step;
      float t = acc / len;
      samples.push_back(
          {poly[i - 1].real() + t * dx, poly[i - 1].imag() + t * dy});
    }
    acc -= len;
  }

  float maxRadius = 0.0f;
  for (auto &p : samples)
    maxRadius = std::max(maxRadius, std::abs(p));

  for (auto &p : samples) {
    p = {p.real() / maxRadius, p.imag() / maxRadius};
  }
}

void Signal::process() {
  phasors.clear();
  addPowerOf2Padding(samples);
  std::cout << "\nsamples: ";
  printComplexVector(samples);

  auto freqResolution = samplingRate / numberOfSamples;
  auto freqs = fft(samples);
  std::cout << "\nfreq: ";
  printComplexVector(freqs);

  auto half = numberOfSamples / 2;
  const float invN = 1.0f / numberOfSamples;
  for (int i = 1; i <= half; i++) {
    phasors.push_back(Phasor(i * freqResolution, freqs[i] * invN));
  }
  for (int i = numberOfSamples - 1; i > half; i--) {
    int bin = static_cast<int>(i) - static_cast<int>(numberOfSamples);
    float freq = bin * freqResolution;
    phasors.push_back(Phasor(freq, freqs[i] * invN));
  }
}

void Signal::update(float dt) {
  tip = {0.0f, 0.0f};
  for (Phasor &phasor : phasors) {
    phasor.update(dt);
    tip += phasor.getComplex();
  }
  trace.push_back({glm::vec2(tip.real(), tip.imag()), 1.0});
  if (trace.size() > MAX_TRACE)
    trace.erase(trace.begin());
  updateTrace(dt);
}

void Signal::draw(glm::mat4 transform) {
  auto phasorCentreTranslation = glm::mat4(transform);
  for (Phasor &phasor : phasors) {
    renderer.draw(phasor, phasorCentreTranslation);
    phasorCentreTranslation = glm::translate(
        phasorCentreTranslation,
        glm::vec3(phasor.getComplex().real(), phasor.getComplex().imag(), 0.0));
  }

  renderTrace(transform);
}

void Signal::renderTrace(glm::mat4 mvp) {
  shader.use();
  shader.setVec3("uColor", glm::vec3{0.4, 0.6, 0.4});
  shader.setMat4("mvp", mvp);
  glBindVertexArray(vao);
  glDrawArrays(GL_LINE_STRIP, 0, trace.size());
}

void Signal::updateTrace(float dt) {
  float rateFactor = samplingRate / (samplingRate + 1500.0f);
  for (auto &p : trace) {
    p.alpha -= dt * 0.1f * rateFactor;
    p.alpha = glm::max(p.alpha, 0.0f);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0, trace.size() * sizeof(TracePoint),
                  trace.data());
}
