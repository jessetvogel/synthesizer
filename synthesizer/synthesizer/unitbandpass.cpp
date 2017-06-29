#include <cmath>

#include "unitbandpass.hpp"
#include "controller.hpp"
#include "instrument.hpp"
#include "IIRfilter.hpp"
#include "parameter.hpp"
#include "settings.hpp"

const int UnitBandpass::maxOrder = 5;

UnitBandpass::UnitBandpass(Controller* controller, int order) : Unit(controller) {
    // Set type
    type = "bandpass";
    
    // May or may not be key dependent
    this->keyDependent = false;
    
    // Set default values
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(center = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "center", "1000.0"));
    parameters.push_back(bandwidth = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "bandwidth", "0.5")); // TODO: come up with some standard values
    
    filter = new IIRFilter(order * 2, order * 2);
}

void UnitBandpass::apply(Instrument* instrument) {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* center = (Unit*) (this->center->pointer);
    Unit* bandwidth = (Unit*) (this->bandwidth->pointer);
    
    // Compute and bound cutoff frequencies
    double wc = 2.0 * M_PI * center->output[0] / sampleRate;
    double wl = wc * (1.0 - bandwidth->output[0]);
    double wh = wc * (1.0 + bandwidth->output[0]);
    if(wl < 0.01) wl = 0.01;
    if(wl > M_PI - 0.001) wl = M_PI - 0.001;
    if(wh < 0.01) wh = 0.01;
    if(wh > M_PI - 0.001) wh = M_PI - 0.001;
    
    // Check if cutoff frequencies have changed, if so: update the filter
    if(wl != omegaL || wh != omegaH) {
        omegaL = wl;
        omegaH = wh;
        updateFilter();
    }
    
    // Simply apply the filter
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = filter->apply(input->output[x]);
}


void UnitBandpass::updateFilter() {
    // See: 'Audio Effects - Theory, Implementation and Application' pp. 59--87
    
    // TODO: copy parts from unitlowpass.cpp and unithighpass.cpp
    
    // Change cut-off frequency
    double betaL = 2.0 / (1.0 + tan(0.5 * omegaL)) - 1.0;
    double betaH = 2.0 / (1.0 + tan(0.5 * omegaH)) - 1.0;
    
    // Create arrays
    double gain = 1.0;
    
    int qLength = order * 2.0;
    int pLength = (order % 2) * 2.0;
    int p2Length = (order / 2) * 2.0;
    double q[qLength]; // roots
    double p[pLength]; // real poles
    double p2Real[p2Length]; // complex poles
    double p2Imag[p2Length];
    
    // Lowpass part (mostly copied from unitlowpass.cpp)
    for(int n = 0;n < qLength/2; ++n)
        q[n] = -1.0;
    
    if(pLength/2 == 1) {
        gain /= 2.0; // (technically, gamma = 0 so 2.0 * cos(gamma) = 2.0)
        p[0] = 0.0;
    }
    
    double gamma;
    for(int n = 0;n < p2Length/2; ++n) {
        gamma = M_PI * 0.25 * ((2.0 * n + 1.0) / order - 1.0);
        double c = cos(gamma);
        gain /= 4.0 * c * c;
        p2Real[n] = 0.0;
        p2Imag[n] = tan(gamma);
    }
    
    // Change cut-off frequency
    for(int n = 0;n < qLength/2; ++n) {
        // Update roots
        gain *= (1.0 + betaH * q[n]); // Note: for the lowpass filter, the higher cut-off frequency must be used
        q[n] = (q[n] + betaH) / (1.0 + betaH * q[n]);
    }
    
    for(int n = 0;n < pLength/2; ++n) {
        // Update poles (real)
        gain /= (1.0 + betaH * p[n]);
        p[n] = (p[n] + betaH) / (1.0 + betaH * p[n]);
    }
    
    for(int n = 0;n < p2Length/2; ++n) {
        // Update poles (complex)
        double a = p2Real[n];
        double b = p2Imag[n];
        double tmp = 1.0 + betaH * a; tmp *= tmp; tmp += betaH * betaH * b * b;
        gain /= tmp;
        p2Real[n] = ((a + betaH) * (1.0 + betaH * a) + betaH * b * b) / tmp;
        p2Imag[n] = ((1.0 + betaH * a) * b - betaH * b * (a + betaH)) / tmp;
    }
    
    // Highpass part
    for(int n = 0;n < qLength; ++n)
        q[qLength/2 + n] = -1.0;
    
    if(pLength/2 == 1) {
        gain /= 2.0; // (technically, gamma = 0 so 2.0 * cos(gamma) = 2.0)
        p[pLength/2 + 0] = 0.0;
    }
    
    for(int n = 0;n < p2Length/2; ++n) {
        gamma = M_PI * 0.25 * ((2.0 * n + 1.0) / order - 1.0);
        double c = cos(gamma);
        gain /= 4.0 * c * c;
        p2Real[p2Length/2 + n] = 0.0;
        p2Imag[p2Length/2 + n] = tan(gamma);
    }
    
    // For a highpass filter: change z into -z, i.e. all roots and poles will be made negative
    for(int n = 0;n < qLength/2; ++n)
        q[qLength/2 + n] = -q[qLength/2 + n];
    
    // Change cut-off frequency
    for(int n = 0;n < qLength/2; ++n) {
        // Update roots
        gain *= (1.0 + betaL * q[qLength/2 + n]); // Note: for the highpass filter, the lower cut-off frequency must be used
        q[qLength/2 + n] = (q[qLength/2 + n] + betaL) / (1.0 + betaL * q[qLength/2 + n]);
    }
    
    for(int n = 0;n < pLength/2; ++n) {
        // Update poles (real)
        gain /= (1.0 + betaL * p[pLength/2 + n]);
        p[pLength/2 + n] = (p[pLength/2 + n] + betaL) / (1.0 + betaL * p[pLength/2 + n]);
    }
    
    for(int n = 0;n < p2Length/2; ++n) {
        // Update poles (complex)
        double a = p2Real[p2Length/2 + n];
        double b = p2Imag[p2Length/2 + n];
        double tmp = 1.0 + betaL * a; tmp *= tmp; tmp += betaL * betaL * b * b;
        gain /= tmp;
        p2Real[p2Length/2 + n] = ((a + betaL) * (1.0 + betaL * a) + betaL * b * b) / tmp;
        p2Imag[p2Length/2 + n] = ((1.0 + betaL * a) * b - betaL * b * (a + betaL)) / tmp;
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
