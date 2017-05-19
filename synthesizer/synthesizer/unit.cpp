#include <cstdlib>

#include "unit.hpp"
#include "util.hpp"
#include "controller.hpp"

#include "unitoscillator.hpp"
#include "unitconstant.hpp"
#include "unitadder.hpp"

Unit* Unit::create(Controller* controller, std::string type) {
    // Oscillator
    if(type.compare("oscillator") == 0) return new UnitOscillator(controller);
    
    // Adder of size n in {1, 2, ... , UnitAdder::maxN}
    for(int n = 1;n <= UnitAdder::maxN;n ++) {
        char typeName[12];
        sprintf(typeName, "adder_%d", n);
        if(type.compare(typeName) == 0)
            return new UnitAdder(controller, n);
    }
    
    return NULL;
}

bool Unit::set(Controller* controller, Unit** parameterAddr, std::string value) {
    if(Util::isNumber(value)) {
        if(*parameterAddr != NULL && (*parameterAddr)->deleteOnRemove()) delete (*parameterAddr);
        *parameterAddr = new UnitConstant(controller, stod(value));
        return true;
    }
    else {
        Unit* unit = controller->getUnit(value);
        if(unit == NULL) return false;

        if(*parameterAddr != NULL && (*parameterAddr)->deleteOnRemove()) delete (*parameterAddr);
        *parameterAddr = unit;
        return true;
    }
}
