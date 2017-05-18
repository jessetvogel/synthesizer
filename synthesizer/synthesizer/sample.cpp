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
    long qpd = phase / M_PI;
    if (qpd >= 0) qpd += qpd & 1;
    else qpd -= qpd & 1;
    phase -= M_PI * (double) qpd - M_PI;
    
    // Linear interpolation
    double x = phase / (2.0 * M_PI) * resolution;
    int low = floor(x);
    int high = ceil(x);
    double z = (double) high - x;

    return data[low] * z + data[high] * (1.0 - z);
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

    for(int i = 0;i < resolution;i ++) {
        dataSine[i] = sin((double) i / resolution * 2.0 * M_PI);
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
