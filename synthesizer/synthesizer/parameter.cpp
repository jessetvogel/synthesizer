#include <string>

#include "parameter.hpp"
#include "controller.hpp"
#include "units.hpp"
#include "unit.hpp"
#include "blocks.hpp"
#include "block.hpp"
#include "sample.hpp"
#include "unitconstant.hpp"
#include "util.hpp"
#include "curve.hpp"
#include "function.hpp"

#include "status.hpp"

Parameter::Parameter(Controller* controller, Type type, std::string label, std::string defaultValue) : type(type), label(label) {
    // Store stuff and set default value
    this->controller = controller;
    
    pointer = NULL;
    set(defaultValue);
    strValue = defaultValue;
}

bool Parameter::set(std::string value) {
    switch(type) {

        case SAMPLE:
            if(value.compare("sine") == 0)      { pointer = Sample::sine; strValue = value; return true; }
            if(value.compare("square") == 0)    { pointer = Sample::square; strValue = value; return true; }
            if(value.compare("triangle") == 0)  { pointer = Sample::triangle; strValue = value; return true; }
            if(value.compare("sawtooth") == 0)  { pointer = Sample::sawtooth; strValue = value; return true; }
            Status::addError("Sample does not exist");
            return false;
            
        case CURVE:
            if(value.compare("linear") == 0)        { pointer = Curve::Linear; strValue = value; return true; }
            if(value.compare("sine") == 0)          { pointer = Curve::Sine; strValue = value; return true; }
            if(value.compare("quadin") == 0)        { pointer = Curve::QuadIn; strValue = value; return true; }
            if(value.compare("quadout") == 0)       { pointer = Curve::QuadOut; strValue = value; return true; }
            if(value.compare("quartin") == 0)       { pointer = Curve::QuartIn; strValue = value; return true; }
            if(value.compare("quartout") == 0)      { pointer = Curve::QuartOut; strValue = value; return true; }
            if(value.compare("exponential") == 0)   { pointer = Curve::Exponential; strValue = value; return true; }
            Status::addError("Curve does not exist");
            return false;
            
            
        case FUNCTION:
            if(value.compare("identity") == 0)  { pointer = Function::Identity; strValue = value; return true; }
            if(value.compare("pow10") == 0)     { pointer = Function::Pow10; strValue = value; return true; }
            Status::addError("Function does not exist");
            return false;
            
        case UNIT:
        case UNIT_KEY_INDEPENDENT:
            
            Unit* unit = NULL;
            // Check if it is a number
            if(Util::isNumber(value)) {
                // Create a new unit constant
                unit = controller->getUnits()->createConstant(stod(value));
            }
            
            // Check if it is a unit
            else if((unit = controller->getUnits()->getUnit(value)) != NULL) {
                // Check for key dependence
                if(type == UNIT_KEY_INDEPENDENT && unit->isKeyDependent()) {
                    Status::addError("Key independent unit cannot depend on key dependent unit");
                    return false;
                }
            }
            
            // Check if it is a block output
            else {
                unsigned long position;
                if((position = value.find(":")) != std::string::npos) {
                    std::string blockId = value.substr(0, position);
                    std::string label = value.substr(position + 1);
                    
                    Block* block = controller->getBlocks()->getBlock(blockId);
                    if(block == NULL) return false; // TODO
                    
                    unit = block->getOutput(label);
                    if(unit == NULL) return false; // TODO
                }
            }
            
            if(unit == NULL) return false; // TODO
            
            // If we are overwriting a unit constant, delete the old constant
            if(pointer != NULL && ((class Unit*) pointer)->getType().compare("constant") == 0)
                controller->getUnits()->deleteConstant((UnitConstant*) pointer);
            
            pointer = unit;
            strValue = value;
            return true;
    }
}

std::string Parameter::typeToString(Type type) {
    switch(type) {
        case UNIT: return "unit_key_dependent";
        case UNIT_KEY_INDEPENDENT: return "unit";
        case SAMPLE: return "sample";
        case CURVE: return "curve";
        case FUNCTION: return "function";
    }
}


