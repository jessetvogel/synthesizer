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

bool Sample::set(Controller* controller, Sample** parameterAddr, std::string value) {
    if(value.compare("sine") == 0)      { *parameterAddr = sine; return true; }
    if(value.compare("square") == 0)    { *parameterAddr = square; return true; }
    if(value.compare("triangle") == 0)  { *parameterAddr = triangle; return true; }
    if(value.compare("sawtooth") == 0)  { *parameterAddr = sawtooth; return true; }
    
    return false;
}
