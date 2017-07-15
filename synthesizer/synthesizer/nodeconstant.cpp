#include <algorithm>
#include <sstream>

#include "nodeconstant.hpp"
#include "controller.hpp"
#include "nodeoutput.hpp"

NodeConstant::NodeConstant(Controller* controller, double value) : Node(controller) {
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
    
    // Set outputs
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
    
    // Fill output with value
    float* output = this->output->getBuffer();
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = value;
}
