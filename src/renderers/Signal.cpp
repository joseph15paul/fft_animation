#include "renderers/Signal.h"
#include "glm/ext/matrix_transform.hpp"
#include <complex>
#include <vector>
#define PI 3.14

Signal::Signal() {}
Signal::~Signal() {}

void Signal::sample(std::string filePath, float samplingRateHZ) {
  samplingRate = 2 * PI * samplingRateHZ;
}

void Signal::process() {
  phasors.clear();
  auto signal = std::vector<std::complex<float>>{0.1f, 0.2f, 0.3f, 0.4f};
  addPowerOf2Padding(signal);
  auto freqResolution = samplingRate / signal.size();
  auto freqs = fft(signal);

  for (int i = 0; i < signal.size(); i++) {
    phasors.push_back(Phasor(i * freqResolution, freqs[i]));
  }
}

void Signal::draw(float dt) {
  auto transform = glm::mat4(1.0);
  for (Phasor &phasor : phasors) {
    phasor.update(dt);
    renderer.draw(phasor, transform);
    transform = glm::translate(transform,
                               glm::vec3(phasor.getComplex().real() / 10,
                                         phasor.getComplex().imag() / 10, 0.0));
  }
}
