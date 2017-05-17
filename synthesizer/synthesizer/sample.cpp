#include "sample.hpp"
#include <cstring>
#include <cmath>

Sample::Sample(int resolution, float* data) {
    // Store the resolution and data
    this->resolution = resolution;
    this->data = new float[resolution];
    memcpy(this->data, data, sizeof(float) * resolution);
}

Sample::~Sample() {
    delete[] data;
}

Sample::Sample(Sample* sample1, Sample* sample2, float mix) {
    // Combine the two given samples to create a new one
    resolution = sample1->resolution;
    data = new float[resolution];
    for(int i = 0;i < resolution;i ++)
        data[i] = sample1->data[i] * mix + sample2->getValue((double) i / resolution * 2.0 * M_PI);
}

float Sample::getValue(double phase) {
    // Wrap phase into 0, 2Pi interval
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
    int resolution = 512;
    float dataSine[resolution];
    float dataSquare[resolution];
    float dataTriangle[resolution];
    float dataSawtooth[resolution];
    for(int i = 0;i < resolution;i ++) {
        dataSine[i] = sin((double) i / resolution * 2.0 * M_PI);
        dataSquare[i] = i < (resolution / 2) ? 1.0 : -1.0;
        dataTriangle[i] = i < (resolution / 4) ? 4.0 * i / resolution : (i < (3 * resolution / 4) ? 2.0 - 4.0 * i / resolution : 4.0 * i / resolution - 4.0);
        dataSawtooth[i] = i < (resolution / 2) ? 2.0 * i / resolution : 2.0 * i / resolution - 2.0;
    }

    sine = new Sample(resolution, dataSine);
    square = new Sample(resolution, dataSquare);
    triangle = new Sample(resolution, dataTriangle);
    sawtooth = new Sample(resolution, dataSawtooth);
}
