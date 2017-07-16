#include "nodefunction.hpp"
#include "controller.hpp"
#include "nodeinput.hpp"
#include "nodeoutput.hpp"
#include "arguments.hpp"

NodeFunction::NodeFunction(Controller* controller, Arguments arguments) : Node(controller) {
    // Set type
    type = "function";
    
    // Set arguments
    keyNode = arguments.getBool("key", false);
    
    // Set inputs and outputs
    addInput("input", input = new NodeInput(controller, keyNode ? NodeInput::NODE : NodeInput::NODE_KEY_INDEPENDENT, "0.0"));
    addInput("function", function = new NodeInput(controller, NodeInput::FUNCTION, "identity"));
    
    addOutput(NODE_OUTPUT_DEFAULT, output = new NodeOutput(controller, this));
}

void NodeFunction::apply() {
    float* input = ((NodeOutput*) this->input->pointer)->getBuffer();
    Function* function = (Function*) (this->function->pointer);
    
    float* output = this->output->getBuffer();
    
    for(int x = 0;x < framesPerBuffer; ++x)
        output[x] = Function::evaluate(function, input[x]);
}
