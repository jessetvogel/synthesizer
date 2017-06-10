#include <algorithm>

#include "unitconstant.hpp"
#include "controller.hpp"

std::vector<UnitConstant*> UnitConstant::units;

UnitConstant::UnitConstant(Controller* controller, double value) {
    // Store pointer to controller
    this->controller = controller;
    
    // Store value
    this->value = value;
    
    // Constants are not key dependent
    keyDependent = false;
    
    // Create arrays
    output = new float[controller->getFramesPerBuffer()];
    for(int x = 0;x < controller->getFramesPerBuffer(); ++x)
        output[x] = value;
}

UnitConstant* UnitConstant::create(Controller* controller, double value) {
    UnitConstant* u = new UnitConstant(controller, value);
    units.push_back(u);
    return u;
}

bool UnitConstant::remove(UnitConstant* u) {
    auto position = std::find(units.begin(), units.end(), u);
    if(position == units.end()) return false;
    
    units.erase(position);
    delete u;
    return true;
}

bool UnitConstant::isUnitConstant(Unit* u) {
    return std::find(units.begin(), units.end(), u) != units.end();
}
