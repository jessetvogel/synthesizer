#include "sample.hpp"
#include <cstring>
#include <cmath>

Sample::Sample(int resolution, int periods, double* data) {
    // Store all information
    this->resolution = resolution;
    this->periods = periods;
    this->data = new double[resolution];
    memcpy(this->data, data, sizeof(double) * resolution);
}

Sample::~Sample() {
    delete[] data;
}

float Sample::getValue(double phase) {
    // Wrap phase into 0, 2Pi interval TODO: extend to (0, periods * 2 * pi) interval. This works now because all samples have periods = 2
    long n = phase / (2.0 * M_PI);
    phase -= 2.0 * M_PI * floor(n);
    
    // Linear interpolation
    double x = phase / (2.0 * M_PI) * resolution;
    int low = floor(x);
    double z = (double) x - low;
    
    return data[low] * (1.0 - z) + data[(low + 1) % resolution] * z;
}

Sample* Sample::sine;
Sample* Sample::square;
Sample* Sample::triangle;
Sample* Sample::sawtooth;

void Sample::initialize() {
    int resolution = 64;
    double dataSine[resolution];
    double dataSquare[resolution];
    double dataTriangle[resolution];
    double dataSawtooth[resolution];

    for(int i = 0;i < resolution; ++i) {
        dataSine[i] = sin(2.0 * M_PI * i / resolution);
        dataSquare[i] = i < (resolution / 2) ? 1.0 : -1.0;
        dataTriangle[i] = i < (resolution / 4) ? 4.0 * i / resolution : (i < (3 * resolution / 4) ? 2.0 - 4.0 * i / resolution : 4.0 * i / resolution - 4.0);
        dataSawtooth[i] = i < (resolution / 2) ? 2.0 * i / resolution : 2.0 * i / resolution - 2.0;
    }

    sine = new Sample(resolution, 1, dataSine);
    square = new Sample(resolution, 1, dataSquare);
    triangle = new Sample(resolution, 1, dataTriangle);
    sawtooth = new Sample(resolution, 1, dataSawtooth);
}

Sample* Sample::fromString(std::string str) {
    if(str.compare("sine") == 0) return sine;
    if(str.compare("square") == 0) return square;
    if(str.compare("triangle") == 0) return triangle;
    if(str.compare("sawtooth") == 0) return sawtooth;
    
    return NULL;
}
