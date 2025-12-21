#include <iostream>
#include "FFT/fft.h"

int main() {
    auto signal = std::vector<std::complex<float>> {1.0f,2.0f,3.0f,4.0f, 5.0f};
    addPowerOf2Padding(signal);
    std::cout << "signal: ";
    printComplexVector(signal);

    auto freqs = fft(signal);
    std::cout << "frequency domain: ";
    printComplexVector(freqs);
    return 0;
}

