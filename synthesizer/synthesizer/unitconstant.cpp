#include <algorithm>
#include <sstream>

#include "unitconstant.hpp"
#include "controller.hpp"

UnitConstant::UnitConstant(Controller* controller, double value) : Unit(controller) {
    // Set type
    type = "constant";
    
    // Set id
    std::ostringstream oss;
    oss << value;
    id = oss.str();
    
    // Store value
    this->value = value;
    
    // Hidden
    hidden = true;
    
    // Fill output with value
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = value;
}
