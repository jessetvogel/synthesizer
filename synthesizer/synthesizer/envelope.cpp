#include "envelope.hpp"

float Envelope::value(bool pressing, double duration, double release) {
    finished = !pressing;
    return 1.0f;
}


Envelope* Envelope::standard;

void Envelope::initialize() {
    standard = new Envelope();
}