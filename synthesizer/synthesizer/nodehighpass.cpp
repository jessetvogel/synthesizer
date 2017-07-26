#include <cmath>

#include "nodehighpass.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "settings.hpp"
#include "IIRfilter.hpp"
#include "options.hpp"

const int NodeHighpass::maxOrder = 5;

NodeHighpass::NodeHighpass(Controller* controller, Options options) : Node(controller) {
    // Set type
    type = "highpass";
    
    // Set options
    order = options.getInteger("order", 1);
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("cutoff", cutOff = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "1000.0"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));

    // Create filter
    filter = new IIRFilter(order, order);
}

NodeHighpass::~NodeHighpass() {
    delete filter;
}

void NodeHighpass::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    float* cutOff = ((NodeOutput*) this->cutOff->pointer)->getBuffer();
    
    float* output = this->output->getBuffer();
    
    // Bound cutoff frequency
    double wc = 2.0 * M_PI * cutOff[0] / sampleRate;
    if(wc < 0.01) wc = 0.01;
    if(wc > M_PI - 0.001) wc = M_PI - 0.001;
    
    // Check if cutoff frequency has changed, if so: update the filter
    if(wc != omegaC) {
        omegaC = wc;
        updateFilter();
    }
    
    // Simply apply the filter
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = filter->apply(input[x]);
}

void NodeHighpass::updateFilter() {
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
    
    // For a highpass filter: change z into -z, i.e. all roots and poles will be made negative
    for(int n = 0;n < qLength; ++n)
        q[n] = -q[n];
    
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
