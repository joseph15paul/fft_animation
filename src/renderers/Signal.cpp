#include "renderers/Signal.h"
#include "glm/ext/matrix_transform.hpp"
#include <complex>
#include <cstdlib>
#include <vector>
#define NANOSVG_IMPLEMENTATION
#include "nanoSVG/nanosvg.h"
#define PI 3.14

Signal::Signal() {}
Signal::~Signal() { reset(); }

void Signal::reset() { renderer.reset(); }

void Signal::sample(std::string filePath, float samplingRateHZ,
                    int numberOfSamples) {
  samplingRate = 2 * PI * samplingRateHZ;
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
        poly.push_back(std::complex<float>{x, y});
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
  /*
        signal = std::vector<std::complex<float>>{
    { 0.000000f, 0.0f },
    { 0.707107f, 0.0f },
    { 1.000000f, 0.0f },
    { 0.707107f, 0.0f },
    { 0.000000f, 0.0f },
    { -0.707107f, 0.0f },
    { -1.000000f, 0.0f },
    { -0.707107f, 0.0f }
};*/
  addPowerOf2Padding(samples);
  std::cout << "samples: ";
  printComplexVector(samples);
  auto freqResolution = samplingRate / numberOfSamples;
  auto freqs = fft(samples);

  std::cout << "\nfreq: ";
  printComplexVector(freqs);
  auto half = numberOfSamples / 2;
  const float invN = 1.0f / numberOfSamples;
  for (int i = 0; i <= half; i++) {
    phasors.push_back(Phasor(i * freqResolution, freqs[i] * invN));
  }
  for (int i = numberOfSamples - 1; i > half; i--) {
    int bin = static_cast<int>(i) - static_cast<int>(numberOfSamples);
    float freq = bin * freqResolution;
    phasors.push_back(Phasor(freq, freqs[i]));
  }
}

void Signal::draw(float dt, glm::mat4 transform) {
  for (Phasor &phasor : phasors) {
    phasor.update(dt);
    renderer.draw(phasor, transform);
    transform = glm::translate(transform,
                               glm::vec3(phasor.getComplex().real() / 10,
                                         phasor.getComplex().imag() / 10, 0.0));
  }
}
