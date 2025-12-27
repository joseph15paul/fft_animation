#pragma once

#include "phasor/Phasor.h"
#include "FFT/fft.h"
#include "renderers/PhasorRenderer.h"
#include <vector>

class Signal {
	std::vector<Phasor> phasors;
	complexVector samples;
	float samplingRate = 1.0f;
	int numberOfSamples = 0;
	PhasorRenderer renderer;

public:
	Signal();
	~Signal();

	void sample(std::string filePath, float samplingRateHZ, int numberOfSamples);
	void process();
	void draw(float dt, glm::mat4 transform = glm::mat4(1.0));
	void reset();
};
