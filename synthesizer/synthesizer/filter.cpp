#include "filter.hpp"
#include <cstring>
#include <cmath>

Filter::Filter(double low, double high, int bands, double* values) {
    // Store all values
    this->low = low;
    this->high = high;
    this->bands = bands;
    this->values = new double[bands];
    memcpy(this->values, values, sizeof(double) * bands);
    
    logConstant = 1.0 / log2(high / low);
}

Filter::~Filter() {
    delete[] values;
}

double Filter::at(double frequency) {
    double x = log2(frequency / low) * logConstant * (bands - 1);
    if(x <= 0) return values[0];
    if(x >= bands - 1) return values[bands - 1];
    
    int a = floor(x);
    double z = x - a;
    return (1.0 - z) * values[a] + z * values[a + 1];
}


