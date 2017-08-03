#include "sample.hpp"
#include <cstring>
#include <cmath>

#define ONE_OVER_TWO_PI (0.1591549430919)
#define DEFAULT_RESOLUTION (2048)

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
    // Optimized, but slightly less accurate implementation
    return data[((int) (phase * ONE_OVER_TWO_PI * resolution)) % resolution]; // TODO: will we have samples with more than one period? If yes, then change this line.
}

std::string Sample::getId() {
    if(this == sine) return "sine";
    if(this == square) return "square";
    if(this == triangle) return "triangle";
    if(this == sawtooth) return "sawtooth";
    return "undefined";
}

Sample* Sample::get(double x) {
    if(x >= SAMPLE_SINE && x < SAMPLE_SINE + 1.0) return sine;
    if(x >= SAMPLE_TRIANGLE && x < SAMPLE_TRIANGLE + 1.0) return triangle;
    if(x >= SAMPLE_SAWTOOTH && x < SAMPLE_SAWTOOTH + 1.0) return sawtooth;
    if(x >= SAMPLE_SQUARE && x < SAMPLE_SQUARE + 1.0) return square;
    
    return sine; // TODO: add warning?
}

Sample* Sample::sine;
Sample* Sample::square;
Sample* Sample::triangle;
Sample* Sample::sawtooth;

void Sample::initialize() {
    int resolution = DEFAULT_RESOLUTION;
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

void Sample::destruct() {
    delete sine;
    delete square;
    delete triangle;
    delete sawtooth;
}
