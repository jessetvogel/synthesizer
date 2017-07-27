#include "nodefactory.hpp"

#include "controller.hpp"
#include "nodes.hpp"
#include "options.hpp"

#include "nodeoscillator.hpp"
#include "nodeconstant.hpp"
#include "nodeadder.hpp"
#include "nodeADSR.hpp"
#include "nodelowpass.hpp"
#include "nodehighpass.hpp"
#include "nodebandpass.hpp"
#include "nodelabel.hpp"
#include "nodeconditional.hpp"
#include "nodefuzz.hpp"
#include "nodedelay.hpp"
#include "nodePWM.hpp"
#include "nodeparameter.hpp"
#include "nodefollower.hpp"
#include "nodekeyinfo.hpp"
#include "nodeleadkeyinfo.hpp"
#include "nodemodulationwheel.hpp"
#include "nodeaudioinput.hpp"
#include "nodeaudiooutput.hpp"
#include "nodecollector.hpp"
#include "nodecustom.hpp"

#include "status.hpp"

void NodeFactory::createDefaultNodes(Controller* controller, Nodes* nodes) {    
    nodes->addNode((new NodeKeyInfo(controller, NodeKeyInfo::Frequency))->setId("key_frequency"));
    nodes->addNode((new NodeKeyInfo(controller, NodeKeyInfo::Velocity))->setId("key_velocity"));
    nodes->addNode((new NodeKeyInfo(controller, NodeKeyInfo::Duration))->setId("key_duration"));
    nodes->addNode((new NodeKeyInfo(controller, NodeKeyInfo::Release))->setId("key_release"));
    
    nodes->addNode((new NodeLeadKeyInfo(controller, NodeLeadKeyInfo::Frequency))->setId("lead_key_frequency"));
    nodes->addNode((new NodeLeadKeyInfo(controller, NodeLeadKeyInfo::Velocity))->setId("lead_key_velocity"));
    nodes->addNode((new NodeLeadKeyInfo(controller, NodeLeadKeyInfo::Duration))->setId("lead_key_duration"));
    nodes->addNode((new NodeLeadKeyInfo(controller, NodeLeadKeyInfo::Release))->setId("lead_key_release"));
    nodes->addNode((new NodeLeadKeyInfo(controller, NodeLeadKeyInfo::Pressing))->setId("lead_key_pressing"));
    
    nodes->addNode((new NodeModulationWheel(controller))->setId("modulation_wheel"));
}

Node* NodeFactory::create(Controller* controller, std::string type, std::string id, Options options) {
    Node* node = NULL;
    
    if(type.compare("adder") == 0)                  node = new NodeAdder(controller, options);
    else if(type.compare("delay") == 0)             node = new NodeDelay(controller, options);
    else if(type.compare("lowpass") == 0)           node = new NodeLowpass(controller, options);
    else if(type.compare("highpass") == 0)          node = new NodeHighpass(controller, options);
    else if(type.compare("bandpass") == 0)          node = new NodeBandpass(controller, options);
    else if(type.compare("oscillator") == 0)        node = new NodeOscillator(controller, options);
    else if(type.compare("ADSR") == 0)              node = new NodeADSR(controller, options);
    else if(type.compare("conditional") == 0)       node = new NodeConditional(controller, options);
    else if(type.compare("fuzz") == 0)              node = new NodeFuzz(controller, options);
    else if(type.compare("PWM") == 0)               node = new NodePWM(controller, options);
    else if(type.compare("follower") == 0)          node = new NodeFollower(controller, options);
    else if(type.compare("label") == 0)             node = new NodeLabel(controller, options);
    else if(type.compare("parameter") == 0)         node = new NodeParameter(controller, options);
    else if(type.compare("collector") == 0)         node = new NodeCollector(controller, options);
    else if(type.compare("audio_input") == 0)       node = new NodeAudioInput(controller, options);
    else if(type.compare("audio_output") == 0)      node = new NodeAudioOutput(controller, options);
    else if(type.compare("custom") == 0)            node = new NodeCustom(controller, options);

    if(node == NULL) {
        // If no match was found, return NULL
        Status::addError("Node type does not exist");
        return NULL;
    }
    
    node->setId(id);
    return node;
}
