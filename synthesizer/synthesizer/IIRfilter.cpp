#include <cstring>

#include "IIRfilter.hpp"

IIRFilter::IIRFilter(int FFFO, int FBFO) {
    
                if(FFFO < 0 || FBFO < 0) { /* TODO: ? */ }
    
    // Store FFFO / FBFO
    this->FFFO = FFFO;
    this->FBFO = FBFO;
    
    // Create arrays
    input = new float[FFFO + 1];
    output = new float[FBFO + 1];
    alpha = new float[FFFO + 1];
    beta = new float[FBFO + 1];
    
    memset(input, 0, sizeof(float) * (FFFO + 1));
    memset(output, 0, sizeof(float) * (FBFO + 1));
    memset(alpha, 0, sizeof(float) * (FFFO + 1));
    memset(beta, 0, sizeof(float) * (FBFO + 1));
    
    gain = 1.0;
}

IIRFilter::~IIRFilter() {
    delete[] input;
    delete[] output;
    delete[] alpha;
    delete[] beta;
}

float IIRFilter::apply(float sample) {
    // Shift input / output arrays
    memmove(input + 1, input, sizeof(float) * FFFO);
    memmove(output + 1, output, sizeof(float) * FBFO);
    
    // Set new input
    input[0] = sample;
    
    // Compute new output
    output[0] = 0.0;
    for(int i = 0;i <= FFFO; ++i)
        output[0] += beta[i] * input[i];
    
    for(int i = 1;i <= FBFO; ++i)
        output[0] -= alpha[i] * output[i];
    
    output[0] *= gain / alpha[0];
    
    // Return output
    return output[0];
}
