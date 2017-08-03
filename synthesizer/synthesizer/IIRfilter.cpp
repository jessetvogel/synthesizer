#include <cstring>

#include "IIRfilter.hpp"

IIRFilter::IIRFilter(int FFFO, int FBFO) {
    
                if(FFFO < 0 || FBFO < 0) { /* TODO: ? */ }
    
    // Store FFFO / FBFO
    this->FFFO = FFFO;
    this->FBFO = FBFO;
    
    // Create arrays
    input = new double[FFFO + 1];
    output = new double[FBFO + 1];
    alpha = new double[FFFO + 1];
    beta = new double[FBFO + 1];
    
    memset(input, 0, sizeof(double) * (FFFO + 1));
    memset(output, 0, sizeof(double) * (FBFO + 1));
    memset(alpha, 0, sizeof(double) * (FFFO + 1));
    memset(beta, 0, sizeof(double) * (FBFO + 1));
    
    gain = 1.0;
}

IIRFilter::~IIRFilter() {
    delete[] input;
    delete[] output;
    delete[] alpha;
    delete[] beta;
}

#include <limits>
#include <iostream>

double IIRFilter::apply(double sample) {
    // Shift input / output arrays
    memmove(input + 1, input, sizeof(double) * FFFO);
    memmove(output + 1, output, sizeof(double) * FBFO);
    
    // Set new input
    input[0] = sample;
    
    // Compute new output
    output[0] = 0.0;
    for(int i = 0;i <= FFFO; ++i)
        output[0] += gain * beta[i] * input[i];
    
    for(int i = 1;i <= FBFO; ++i)
        output[0] -= alpha[i] * output[i];
    
    output[0] /= alpha[0];
    
    // TODO: this is safety, remove this, but also come up with a way to prevent it from happening
    if(output[0] != output[0] || output[0] == std::numeric_limits<double>::infinity()) {
        for(int i = 0;i <= FFFO; ++i)
            input[i] = 0.0;
        for(int i = 0;i <= FBFO; ++i)
            output[i] = 0.0;
        
        // print coefficients
//        for(int i = 0;i <= FFFO; ++i)
//            std::cout << "b[" << i << "] = " << beta[i] << std::endl;
//        
//        for(int i = 0;i <= FBFO; ++i)
//            std::cout << "a[" << i << "] = " << alpha[i] << std::endl;
//        
//        std::cout << "gain = " << gain << std::endl << std::endl;
    }
    
    // Return output
    return output[0];
}
