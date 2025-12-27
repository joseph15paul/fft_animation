#include "phasor/Phasor.h"
#include <complex>

Phasor::Phasor(float omega, float amplitude, float phase)
    : omega(omega), amplitude(amplitude), phase(phase) {}

Phasor::Phasor(float omega, std::complex<float> initialState)
    : omega(omega), amplitude(std::abs(initialState)), phase(std::arg(initialState)) {}

Phasor::~Phasor() {}

void Phasor::update(float deltaTime) { phase += omega * deltaTime; }

// Multiplication (phasor × phasor)
Phasor Phasor::operator*(const Phasor &other) const {
  return Phasor(omega + other.getAngularFrequency(),
                amplitude * other.getAmplitude(), phase + other.getPhase());
}

Phasor &Phasor::operator*=(const Phasor &other) {
  omega += other.getAngularFrequency();
  amplitude *= other.getAmplitude();
  phase += other.getPhase();
  return *this;
}

// Scalar multiply
Phasor Phasor::operator*(float scalar) const {
  return Phasor(omega, amplitude * scalar, phase);
}

Phasor &Phasor::operator*=(float scalar) {
  amplitude *= scalar;
  return *this;
}

