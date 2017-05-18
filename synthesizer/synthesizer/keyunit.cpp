#include "keyunit.hpp"
#include "keyoscillator.hpp"
#include "ADSR.hpp"

void KeyUnit::apply(Instrument* instrument) { }

bool KeyUnit::setValue(std::string parameter, std::string value) {
    return true;
};

void KeyUnit::reset() {
    updated = false;
};

void KeyUnit::update(Instrument* instrument) {
    if(updated) return;
    updated = true;
    apply(instrument);
};

KeyUnit* KeyUnit::create(Controller* controller, std::string type) {
    if(type.compare("oscillator") == 0) return new KeyOscillator(controller);
    if(type.compare("ADSR") == 0) return new ADSR(controller);
    
    return NULL;
}
