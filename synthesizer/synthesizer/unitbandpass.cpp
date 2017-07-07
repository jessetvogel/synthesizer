#include <cmath>

#include "unitbandpass.hpp"
#include "controller.hpp"
#include "IIRfilter.hpp"
#include "parameter.hpp"
#include "settings.hpp"
#include "arguments.hpp"

const int UnitBandpass::maxOrder = 5;

UnitBandpass::UnitBandpass(Controller* controller, Arguments arguments) : Unit(controller) {
    // Set type and store order
    type = "bandpass";
    
    // Set arguments
    order = arguments.getInteger("order", 1);
    
    // Set parameters
    parameters.push_back(input = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "input", "0.0"));
    parameters.push_back(center = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "center", "1000.0"));
    parameters.push_back(bandwidth = new Parameter(controller, keyDependent ? Parameter::UNIT : Parameter::UNIT_KEY_INDEPENDENT, "bandwidth", "0.5")); // TODO: come up with some standard values
    
    filter = new IIRFilter(order * 2, order * 2);
}

void UnitBandpass::apply() {
    Unit* input = (Unit*) (this->input->pointer);
    Unit* center = (Unit*) (this->center->pointer);
    Unit* bandwidth = (Unit*) (this->bandwidth->pointer);
    
    // Compute and bound cutoff frequencies
    double wc = 2.0 * M_PI * center->output[0] / sampleRate;
    double tmp = sqrt(bandwidth->output[0]);
    double wl = wc / tmp;
    double wh = wc * tmp;
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
    
    // Set roots
    for(int n = 0;n < qLength; ++n) {
        // Lowpass has roots -1, highpass has roots +1
        q[n] = n < qLength/2 ? -1.0 : 1.0;
    }
    
    // Set poles
    if(pLength == 2) {
        gain /= 4.0;
        p[0] = p[1] = 0.0;
    }
    
    double gamma, c;
    for(int n = 0;n < p2Length; ++n) {
        if(n < p2Length/2) {
            // Lowpass part
            gamma = M_PI * 0.25 * ((2.0 * n + 1.0) / order - 1.0);
            c = cos(gamma);
            gain /= 4.0 * c * c;
            p2Real[n] = 0.0;
            p2Imag[n] = tan(gamma);
        }
        else {
            // Highpass part
            gamma = M_PI * 0.25 * ((2.0 * (n - p2Length/2) + 1.0) / order - 1.0);
            c = cos(gamma);
            gain /= 4.0 * c * c;
            p2Real[n] = 0.0;
            p2Imag[n] = tan(gamma);
        }
    }
    
    // Change cut-off frequency:
    
    // Update roots
    double beta;
    for(int n = 0;n < qLength; ++n) {
        // For lowpass filter, we use the higher cut-off frequency, for the highpass we use the lower cut-off frequency
        beta = n < qLength/2 ? betaH : betaL;
        gain *= (1.0 + beta * q[n]);
        q[n] = (q[n] + beta) / (1.0 + beta * q[n]);
    }
    
    // Update poles (real)
    for(int n = 0;n < pLength; ++n) {
        beta = n < pLength/2 ? betaH : betaL;
        gain /= (1.0 + beta * p[n]);
        p[n] = (p[n] + beta) / (1.0 + beta * p[n]);
    }

    // Update poles (complex)
    for(int n = 0;n < p2Length; ++n) {
        beta = n < p2Length/2 ? betaH : betaL;
        double a = p2Real[n];
        double b = p2Imag[n];
        double tmp = 1.0 + beta * a; tmp *= tmp; tmp += beta * beta * b * b;
        gain /= tmp;
        p2Real[n] = ((a + beta) * (1.0 + beta * a) + beta * b * b) / tmp;
        p2Imag[n] = ((1.0 + beta * a) * b - beta * b * (a + beta)) / tmp;
    }
    
    // Now compute polynomial coefficients from poles and roots
    int orderX = qLength;
    int orderY = pLength + 2 * p2Length;
    double a[orderX + 1];
    double b[orderY + 1];
    memset(a, 0, sizeof(double) * (orderX + 1));
    memset(b, 0, sizeof(double) * (orderY + 1));
    
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
    for(int n = 0;n < orderX + 1; ++n)
        filter->alpha[n] = a[orderX - n];
    for(int n = 0;n < orderY + 1; ++n)
        filter->beta[n] = b[orderY - n];
    
    filter->gain = gain;
}
