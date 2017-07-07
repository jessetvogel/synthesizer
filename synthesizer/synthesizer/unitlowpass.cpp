#include <cmath>

#include "unitlowpass.hpp"
#include "controller.hpp"
#include "parameter.hpp"
#include "IIRfilter.hpp"
#include "arguments.hpp"

const int UnitLowpass::maxOrder = 5;

UnitLowpass::UnitLowpass(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type
    type = "lowpass";
    
    // Set arguments
    order = arguments.getInteger("order", 1); // TODO: check for valid values
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(cutOff = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "cutoff", "1000.0"));
    
    // Create filter
    filter = new IIRFilter(order, order);
}

UnitLowpass::~UnitLowpass() {
    delete filter;
}

void UnitLowpass::apply() {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* cutOff = (Unit*) (this->cutOff->pointer);
    
    // Bound cutoff frequency
    double wc = 2.0 * M_PI * cutOff->output[0] / sampleRate;
    if(wc < 0.01) wc = 0.01;
    if(wc > M_PI - 0.001) wc = M_PI - 0.001;
    
    // Check if cutoff frequency has changed, if so: update the filter
    if(wc != omegaC) {
        omegaC = wc;
        updateFilter();
    }
    
    // Simply apply the filter
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = filter->apply(input->output[x]);
}

void UnitLowpass::updateFilter() {
    // See: 'Audio Effects - Theory, Implementation and Application' pp. 59--87
    
    // Create array of poles (p2, since they are complex, store the product p*\bar{p}) and roots (q), and determine gain (prototype filter)
    double gain = 1.0;
    
    int qLength = order;
    int pLength = order % 2;
    int p2Length = order / 2;
    double q[qLength]; // roots
    double p[pLength]; // real poles
    double p2Real[p2Length]; // complex poles
    double p2Imag[p2Length];
    
    for(int n = 0;n < qLength; ++n)
        q[n] = -1.0;
    
    if(pLength == 1) {
        gain /= 2.0; // (technically, gamma = 0 so 2.0 * cos(gamma) = 2.0)
        p[0] = 0.0;
    }
    
    double gamma;
    for(int n = 0;n < p2Length; ++n) {
        gamma = M_PI * 0.25 * ((2.0 * n + 1.0) / order - 1.0);
        double c = cos(gamma);
        gain /= 4.0 * c * c;
        p2Real[n] = 0.0;
        p2Imag[n] = tan(gamma);
    }
    
    // Change cut-off frequency
    double beta = 2.0 / (1.0 + tan(0.5 * omegaC)) - 1.0;
    
    for(int n = 0;n < qLength; ++n) {
        // Update roots
        gain *= (1.0 + beta * q[n]);
        q[n] = (q[n] + beta) / (1.0 + beta * q[n]);
    }
    
    for(int n = 0;n < pLength; ++n) {
        // Update poles (real)
        gain /= (1.0 + beta * p[n]);
        p[n] = (p[n] + beta) / (1.0 + beta * p[n]);
    }
    
    for(int n = 0;n < p2Length; ++n) {
        // Update poles (complex)
        double a = p2Real[n];
        double b = p2Imag[n];
        double tmp = 1.0 + beta * a; tmp *= tmp; tmp += beta * beta * b * b;
        gain /= tmp;
        p2Real[n] = ((a + beta) * (1.0 + beta * a) + beta * b * b) / tmp;
        p2Imag[n] = ((1.0 + beta * a) * b - beta * b * (a + beta)) / tmp;
    }
    
    // Now compute polynomial coefficients from poles and roots
    double a[qLength + 1];
    double b[pLength + 2 * p2Length + 1];
    for(int n = 0;n <= order; ++n)
        a[n] = b[n] = 0.0;

    // Polynomial of X(z)
    b[0] = 1.0;
    for(int n = 0;n < qLength; ++n) {
        b[n + 1] = 1.0;
        for(int k = n;k > 0; --k)
            b[k] = b[k] * -q[n] + b[k - 1];
        b[0] = b[0] * -q[n];
    }
    
    // Polynomial of Y(z)
    a[0] = 1.0;
    for(int n = 0;n < pLength; ++n) {
        a[n + 1] = 1.0;
        for(int k = n;k > 0; --k)
            a[k] = a[k] * -p[n] + a[k - 1];
        a[0] = a[0] * -p[n];
    }
    
    for(int n = 0;n < p2Length; ++n) {
        // Write (z - (a + bi))(z - (a - bi)) = z^2 - 2az + (a^2 + b^2) = z^2 + xz + y
        double x = - 2.0 * p2Real[n];
        double y = p2Real[n] * p2Real[n] + p2Imag[n] * p2Imag[n];
        
        a[pLength + 2*n + 2] = 1.0;
        for(int k = pLength + 2*n + 1;k > 1; --k)
            a[k] = a[k] * y + a[k - 1] * x + a[k - 2];
        a[1] = a[1] * y + a[0] * x;
        a[0] = a[0] * y;
    }
    
    // Dividing by z^n translates into 'reversing the coefficients'
    for(int n = 0;n <= order; ++n) {
        filter->alpha[n] = a[order - n];
        filter->beta[n] = b[order - n];
    }
    
    filter->gain = gain;
}
