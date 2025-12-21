#include <iostream>
#include <cmath>
#include "FFT/fft.h"

using namespace std::complex_literals;

void printComplexVector(complexVector& vector) {
    for (const auto& element : vector) {
        std::cout << element.real() << "+i" << element.imag() << "  ";
    }
    std::cout << std::endl; 
}

std::vector<std::complex<float>> get_roots_of_unity(int n) {
    auto roots = std::vector<std::complex<float>>();
    for (int k = 0; k < n; k++) {
        float theta = 2 * M_PI * k / n;
        std::complex<float> root = std::polar(1.0f, -theta);
        roots.push_back(root);
    }
    return roots;
}

int nextPow2(int n) {
    if (n == 0) {
        return 1;
    }

    if ((n & (n - 1)) == 0) {
        return n;
    }
    
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;
    
    return n;
}

void addPowerOf2Padding(complexVector& signal) {
    int n = nextPow2(signal.size());
    signal.resize(n);
}

std::vector<std::complex<float>> fft(std::vector<std::complex<float>>& signal) {
    int len = signal.size();
    int degree = len - 1;
    if (degree == 0) {
        return signal;
    }

    int nBy2 = len/2;
    auto evens = std::vector<std::complex<float>>(nBy2);
    auto odds = std::vector<std::complex<float>>(nBy2);
    for (int i = 0; i < nBy2; i++) {
        evens[i] = (signal[2*i]);
        odds[i] = (signal[2*i+1]);
    }

    auto odd_freq = fft(odds);
    auto even_freq = fft(evens);
    auto w = get_roots_of_unity(len); 
    // print(w); //not exactly 0

    std::vector<std::complex<float>> freqs(len);
    for (int k = 0; k < nBy2; k++) {
        freqs[k] = (even_freq[k] + w[k] * odd_freq[k]);
        freqs[k + nBy2] = (even_freq[k] - w[k] * odd_freq[k]);
    }
    return freqs;
}

/*
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
*/

