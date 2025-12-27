#pragma once
#include <complex>

class Phasor {
	float amplitude = 1.0f;
	float phase = 0.0f;
	float omega = 0.0f;

public:
	Phasor(float angularFrequency = 0.0f, float amplitude = 1.0, float phase = 0.0f);
	Phasor(float angularFrequency = 0.0f, std::complex<float> initailState = {1.0f, 0.0f});
	~Phasor();

	std::complex<float> getComplex() const
	{
		return std::polar(amplitude, phase);
	}
	
	float getAmplitude() const
	{
		return amplitude;
	}

	float getPhase() const
	{
		return phase;
	}


	float getAngularFrequency() const
	{
		return omega;
	}

	void update(float deltaTime);

	Phasor operator*(const Phasor& other) const;
	Phasor& operator*=(const Phasor& other);

	Phasor operator*(float scalar) const;
	Phasor& operator*=(float scalar);

private:
	
};

