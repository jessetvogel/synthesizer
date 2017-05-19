#include "keyunit.hpp"
#include "util.hpp"
#include "controller.hpp"
#include "unit.hpp"
#include "unitconstant.hpp"

#include "keyunitoscillator.hpp"
#include "keyunitADSR.hpp"
#include "keyunitadder.hpp"

void KeyUnit::apply(Instrument* instrument) { }

bool KeyUnit::setValue(std::string parameter, std::string value) {
    return true;
};

void KeyUnit::reset() {
    updated = false;
};

bool KeyUnit::deleteOnRemove() {
    return false;
}

void KeyUnit::update(Instrument* instrument) {
    if(updated) return;
    updated = true;
    apply(instrument);
};

KeyUnit* KeyUnit::create(Controller* controller, std::string type) {
    // Oscillator
    if(type.compare("oscillator") == 0) return new KeyUnitOscillator(controller);
    
    // ADSR envelope
    if(type.compare("ADSR") == 0) return new KeyUnitADSR(controller);
    
    // Adder of size n in {1, 2, ... , UnitAdder::maxN}
    for(int n = 1;n <= KeyUnitAdder::maxN;n ++) {
        char typeName[12];
        sprintf(typeName, "adder_%d", n);
        if(type.compare(typeName) == 0)
            return new KeyUnitAdder(controller, n);
    }
    
    return NULL;
}

bool KeyUnit::set(Controller* controller, KeyUnit** parameterAddr, std::string value) {
    if(Util::isNumber(value)) {
        if(*parameterAddr != NULL && (*parameterAddr)->deleteOnRemove()) delete (*parameterAddr);
        *parameterAddr = new UnitConstant(controller, stod(value));
        return true;
    }
    else {
        KeyUnit* keyUnit = controller->getKeyUnit(value);
        if(keyUnit == NULL) {
            Unit* unit = controller->getUnit(value);
            if(unit == NULL) return false;
            if(*parameterAddr != NULL && (*parameterAddr)->deleteOnRemove()) delete (*parameterAddr);
            *parameterAddr = unit;
            return true;
        }
        if(*parameterAddr != NULL && (*parameterAddr)->deleteOnRemove()) delete (*parameterAddr);
        *parameterAddr = keyUnit;
        return true;
    }
}
