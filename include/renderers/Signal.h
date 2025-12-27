#pragma once

#include "phasor/Phasor.h"
#include "FFT/fft.h"
#include "renderers/PhasorRenderer.h"
#include <vector>

class Signal {
	std::vector<Phasor> phasors;
	complexVector samples;
	float samplingRate = 1.0f;
	PhasorRenderer renderer;

public:
	Signal();
	~Signal();

	void sample(std::string filePath, float samplingRateHZ);
	void process();
	void draw(float dt);
};
