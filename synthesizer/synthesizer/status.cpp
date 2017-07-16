#include <iostream>
#include <sstream>

#include "status.hpp"
#include "controller.hpp"
#include "mididevices.hpp"
#include "audiodevices.hpp"
#include "nodes.hpp"
#include "node.hpp"
#include "nodeinput.hpp"

std::vector<Message> Status::info;
std::vector<Message> Status::warning;
std::vector<Message> Status::error;

std::vector<std::string> Status::extra;

void Status::addInfo(std::string message) {
    Message i;
    i.message = message;
    info.push_back(i);
}

void Status::addWarning(std::string message) {
    Message w;
    w.message = message;
    warning.push_back(w);
}

void Status::addError(std::string message) {
    Message e;
    e.message = message;
    error.push_back(e);
}

bool Status::addExtra(std::string tokens) {
    std::istringstream iss(tokens);
    std::string token;
    while(std::getline(iss, token, ' '))
        extra.push_back(token);
    return true;
}

bool Status::print(Controller* controller) {
    
    std::cout << "{";
    
    bool comma = false;
    
    // Print messages
    if(!info.empty()) {
        if(comma) std::cout << ","; else comma = true;
        printInfo();
    }
    
    if(!warning.empty()) {
        if(comma) std::cout << ","; else comma = true;
        printWarning();
    }
    
    if(!error.empty()) {
        if(comma) std::cout << ","; else comma = true;
        printError();
    }
    
    for(auto it = extra.begin();it != extra.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        printExtra(controller, *it);
    }
    
    std::cout << "}" << std::endl;
    std::cout.flush();
    
    info.clear();
    warning.clear();
    error.clear();
    extra.clear();
    
    return true;
}

void Status::printInfo() {
    
    std::cout << "\"info\":[";
    
    bool comma = false;
    for(auto it = info.begin();it != info.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"message\":\"" << (*it).message << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
}

void Status::printWarning() {
    
    std::cout << "\"warning\":[";
    
    bool comma = false;
    for(auto it = warning.begin();it != warning.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"message\":\"" << (*it).message << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
}

void Status::printError() {
    
    std::cout << "\"error\":[";
    
    bool comma = false;
    for(auto it = error.begin();it != error.end(); ++it) {
        if(comma) std::cout << ","; else comma = true;
        
        std::cout << "{";
        
        std::cout << "\"message\":\"" << (*it).message << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";
}

void Status::printExtra(Controller* controller, std::string extra) {
    if(extra.compare("midi_devices") == 0) return controller->getMIDIDevices()->printMIDIDevices();
    if(extra.compare("input_devices") == 0) return controller->getAudioDevices()->printInputDevices();
    if(extra.compare("output_devices") == 0) return controller->getAudioDevices()->printOutputDevices();
    if(extra.compare("nodes") == 0) return controller->getNodes()->printNodes();
    if(extra.compare("state") == 0) return controller->printState();
    
    std::size_t pos = extra.find(":");
    if(pos != std::string::npos) {
        std::string first = extra.substr(0, pos);
        std::string second = extra.substr(pos + 1);
        if(first.compare("info") == 0) {
            Node* node = controller->getNodes()->getNode(second);
            if(node == NULL) return;
            return node->printNode();
        }
    }
}

// ----------------------------------------------------------------

void MIDIDevices::printMIDIDevices() {
    
    std::cout << "\"midiDevices\":[";
    
    int amountOfDevices = MIDIDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(MIDIDevices::isInput(i)) {
            MIDIDevice* midiDevice = controller->getMIDIDevices()->get(i);
            bool active = (midiDevice != NULL);
            
            if(comma) std::cout << ","; else comma = true;
            std::cout << "{";
            
            std::cout << "\"id\":" << i << ",";
            std::cout << "\"name\":\"" << MIDIDevices::deviceName(i) << "\",";
            std::cout << "\"active\":" << (active ? "true" : "false");
            
            std::cout << "}";
        }
    }
    
    std::cout << "]";
    
}
void AudioDevices::printInputDevices() {
    
    std::cout << "\"inputDevices\":[";
    
    int amountOfDevices = AudioDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(AudioDevices::isInput(i)) {
            if(comma) std::cout << ","; else comma = true;
            std::cout << "{";
            
            std::cout << "\"id\":\"" << i << "\",";
            std::cout << "\"name\":\"" << AudioDevices::deviceName(i) << "\",";
            std::cout << "\"active\":" << (inputDeviceId == i ? "true" : "false") << "";
            
            std::cout << "}";
        }
    }
    
    std::cout << "]";
    
}

void AudioDevices::printOutputDevices() {
    
    std::cout << "\"outputDevices\":[";
    
    int amountOfDevices = AudioDevices::amountOfDevices();
    bool comma = false;
    for(int i = 0;i < amountOfDevices;i ++) {
        if(AudioDevices::isOutput(i)) {
            if(comma) std::cout << ","; else comma = true;
            std::cout << "{";
            
            std::cout << "\"id\":" << i << ",";
            std::cout << "\"name\":\"" << AudioDevices::deviceName(i) << "\",";
            std::cout << "\"active\":" << (outputDeviceId == i ? "true" : "false") << "";
            
            std::cout << "}";
        }
    }
    
    std::cout << "]";
    
}

void Nodes::printNodes() {
    
    std::cout << "\"nodes\":[";
    
    bool comma = false;
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        Node* node = *it;
        if(node->isHidden()) continue;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        std::cout << "\"id\":\"" << node->getId() << "\",";
        std::cout << "\"type\":\"" << node->getType() << "\",";
        std::cout << "\"keyNode\":" << (node->isKeyDependent() ? "true" : "false");
        
        std::cout << "}";
    }
    
    std::cout << "]";
    
}

void Node::printNode() {
    
    std::cout << "\"node\":{";
    
    std::cout << "\"id\":\"" << id << "\",";
    std::cout << "\"type\":\"" << type << "\",";
    std::cout << "\"keyNode\":" << (keyNode ? "true" : "false") << ",";
    std::cout << "\"inputs\":[";
    
    bool comma = false;
    for(auto it = inputs.begin(); it != inputs.end(); ++it) {
        NodeInput* input = it->second;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        std::cout << "\"label\":\"" << it->first << "\",";
        std::cout << "\"type\":\"" << NodeInput::typeToString(input->getType()) << "\",";
        std::cout << "\"value\":\"" << input->getExpression() << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "],";
    
    std::cout << "\"outputs\":[";
    
    comma = false;
    for(auto it = outputs.begin(); it != outputs.end(); ++it) {
        //        NodeOutput* output = it->second;
        
        if(comma) std::cout << ","; else comma = true;
        std::cout << "{";
        
        std::cout << "\"label\":\"" << it->first << "\"";
        
        std::cout << "}";
    }
    
    std::cout << "]";

    std::cout << "}";
}

void Controller::printState() {
    
    std::cout << "\"state\":{";
    
    std::cout << "\"playing\":" << (active ? "true" : "false");
    
    std::cout << "}";
    
}
