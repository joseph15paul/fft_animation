#include <complex>
#include <vector>

typedef std::vector<std::complex<float>> complexVector;

complexVector fft(complexVector& signal);

void addPowerOf2Padding(complexVector& signal);

void printComplexVector(complexVector& signal);

