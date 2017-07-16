#include <algorithm>

#include "nodes.hpp"
#include "node.hpp"
#include "nodeinput.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "settings.hpp"
#include "nodecustom.hpp"
#include "nodeconstant.hpp"
#include "nodeparameter.hpp"
#include "nodeaudiooutput.hpp"
#include "nodecollector.hpp"
#include "midistate.hpp"

#include "nodefactory.hpp"
#include "arguments.hpp"

#include "status.hpp"

Nodes::Nodes(Controller* controller) {
    // Store controller
    this->controller = controller;
    
    // Default nodes
    NodeFactory::createDefaultNodes(controller, this);
}

Nodes::~Nodes() {
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
        delete *it;

    for(auto it = constants.begin(); it != constants.end(); ++it)
        delete *it;
    // Note that the parameters and collectors are a subset of nodes, hence they need not be deallocated (in fact, double deallocation will result in errors)
    mutex.unlock();
}

bool Nodes::create(std::string type, std::string id, std::string arguments) {
    // Check if node with this id already exists
    Node* node = getNode(id);
    if(node != NULL) return false; // TODO: (search for 'return false')
    
    // Create new node
    Arguments args(controller, arguments);
    node = NodeFactory::create(controller, type, id, args);
    if(node == NULL) return false;
    
    // Append node to relevant lists
    addNode(node);
    std::string nodeType = node->getType();
    if(nodeType.compare("parameter") == 0) addNodeParameter((NodeParameter*) node);
    if(nodeType.compare("collector") == 0) addCollector((NodeCollector*) node);
    if(nodeType.compare("audio_output") == 0) addAudioOutput((NodeAudioOutput*) node);
    return true;
}

bool Nodes::destroy(std::string id) {
    // Find node with given id (and erase it already from nodes list)
    mutex.lock();
    Node* node = NULL;
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            nodes.erase(it);
            node = *it;
            break;
        }
    }
    mutex.unlock();

    // In case it does not exists, return
    if(node == NULL) return false;
    
    // Remove node from (other) relevant lists
    std::string nodeType = node->getType();
    if(nodeType.compare("parameter") == 0) removeNodeParameter((NodeParameter*) node);
    if(nodeType.compare("collector") == 0) removeCollector((NodeCollector*) node);
    if(nodeType.compare("audio_output") == 0) removeAudioOutput((NodeAudioOutput*) node);
    
    // Delete object
    delete node;
    
    return true;
}

bool Nodes::rename(std::string oldId, std::string newId) {
    Node* node = getNode(oldId);
    if(node == NULL) return false; // TODO
    
    Node* nodeNew = getNode(newId);
    if(nodeNew != NULL) return false;
    
    return node->setId(newId);
}

bool Nodes::set(std::string strNodeInput, std::string value) {
    NodeInput* nodeInput = getNodeInput(strNodeInput);
    if(nodeInput == NULL) return false; // TODO
    
    return nodeInput->set(value);
}

bool Nodes::hide(std::string id) {
    Node* node = getNode(id);
    if(node == NULL) return false; // TODO
    
    node->hide();
    return true;
}

bool Nodes::info(std::string id) {
    Node* node = getNode(id);
    if(node == NULL) return false; // TODO

    Status::addExtra(std::string("info:").append(id));
    return true;
}

bool Nodes::addInput(std::string id, std::string label, std::string nodeInput) {
    Node* node = getNode(id);
    if(node == NULL) return false;
    if(node->getType().compare("custom") != 0) return false;
    
    NodeInput* input = getNodeInput(nodeInput);
    if(input == NULL) return false; // TODO
    
    bool success = node->addInput(label, input);
    if(success) {
        Node* previousOwner = getNode(nodeInput.substr(0, nodeInput.find("."))); // TODO, do this better
        previousOwner->removeInput(input);
    }
    
    return success;
}

bool Nodes::addOutput(std::string id, std::string label, std::string nodeOutput) {
    Node* node = getNode(id);
    if(node == NULL) return false;
    if(node->getType().compare("custom") != 0) return false;
    
    NodeOutput* output = getNodeOutput(nodeOutput);
    if(output == NULL) return false; // TODO
    
    bool success = node->addOutput(label, output);
    if(success) {
        Node* previousOwner = getNode(nodeOutput.substr(0, nodeOutput.find(":"))); // TODO, do this better
        previousOwner->removeOutput(output);
    }
    
    return success;
}

bool Nodes::attach(std::string id, std::string attachment) {
    Node* node = getNode(id);
    if(node == NULL) return false;
    if(node->getType().compare("custom") != 0) return false;
    NodeCustom* nodeCustom = (NodeCustom*) node;
    
    node = getNode(attachment);
    if(node == NULL) return false;
    
    bool success = nodeCustom->attach(node);
    if(success) {
        mutex.lock();
        for(auto it = nodes.begin(); it != nodes.end(); ++it) {
            if(*it == node) {
                nodes.erase(it);
                break;
            }
        }
        mutex.unlock();
    }
    
    return success;
}

bool Nodes::addNode(Node* node) {
    mutex.lock();
    nodes.push_back(node);
    mutex.unlock();
    return true;
}

Node* Nodes::getNode(std::string id) {
    mutex.lock();
    Node* node = NULL;
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        if((*it)->getId().compare(id) == 0) {
            node = *it;
            break;
        }
    }
    mutex.unlock();
    return node;
}

NodeConstant* Nodes::createConstant(double value) {
    NodeConstant* constant = new NodeConstant(controller, value);
    mutex.lock();
    constants.push_back(constant);
    mutex.unlock();
    return constant;
}

bool Nodes::deleteConstant(NodeConstant* constant) {
    mutex.lock();
    bool found = false;
    auto position = std::find(constants.begin(), constants.end(), constant);
    if(position != constants.end()) {
        constants.erase(position);
        delete constant;
        found = true;
    }
    mutex.unlock();
    return found;
}

bool Nodes::addCollector(NodeCollector* collector) {
    mutex.lock();
    collectors.push_back(collector);
    mutex.unlock();
    return true;
}

bool Nodes::removeCollector(NodeCollector* collector) {
    bool found = false;
    mutex.lock();
    auto position = std::find(collectors.begin(), collectors.end(), collector);
    if(position != collectors.end()) {
        collectors.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

bool Nodes::addAudioOutput(NodeAudioOutput* audioOutput) {
    mutex.lock();
    audioOutputs.push_back(audioOutput);
    mutex.unlock();
    return true;
}

bool Nodes::removeAudioOutput(NodeAudioOutput* output) {
    bool found = false;
    mutex.lock();
    auto position = std::find(audioOutputs.begin(), audioOutputs.end(), output);
    if(position != audioOutputs.end()) {
        audioOutputs.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

bool Nodes::addNodeParameter(NodeParameter* parameter) {
    mutex.lock();
    bool success = false;
    int midiCC = parameter->getMidiCC();
    if(parameters.find(midiCC) == parameters.end()) {
        parameters[midiCC] = parameter;
        success = true;
    }
    mutex.unlock();
    return success;
}

NodeParameter* Nodes::getNodeParameter(int midiCC) {
    mutex.lock();
    NodeParameter* parameter = NULL;
    auto position = parameters.find(midiCC);
    if(position != parameters.end())
        parameter = position->second;
    mutex.unlock();
    return parameter;
}

bool Nodes::removeNodeParameter(NodeParameter* parameter) {
    bool found = false;
    mutex.lock();
    auto position = parameters.find(parameter->getMidiCC());
    if(position != parameters.end()) {
        parameters.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

void Nodes::addKeyEvent(KeyEvent* keyEvent) {
    // Add a copy of this key event to all collectors
    mutex.lock();
    for(auto it = collectors.begin(); it != collectors.end(); ++it) {
        KeyEvent* k = new KeyEvent();
        k->key = keyEvent->key;
        k->stage = keyEvent->stage;
        k->velocity = keyEvent->velocity;
        k->frequency = keyEvent->frequency;
        k->duration = keyEvent->duration;
        k->release = keyEvent->release;
        (*it)->addKeyEvent(k);
    }
    mutex.unlock();
}

void Nodes::resetNodes() {
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
        (*it)->reset();
    mutex.unlock();
}

void Nodes::resetNodesKeyDependent() {
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        Node* node = *it;
        if(node->isKeyDependent())
            node->reset();
    }
    mutex.unlock();
}

NodeInput* Nodes::getNodeInput(std::string str) {
    std::size_t position = str.find(".");
    if(position == std::string::npos) return NULL;
    
    std::string id = str.substr(0, position);
    std::string label = str.substr(position + 1);
    
    Node* node = NULL;
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            node = *it;
            break;
        }
    }
    mutex.unlock();
    if(node == NULL) return NULL;
    return node->getInput(label);
}

NodeOutput* Nodes::getNodeOutput(std::string str) {
    std::string id, label;
    
    std::size_t position = str.find(":");
    if(position != std::string::npos) {
        id = str.substr(0, position);
        label = str.substr(position + 1);
    }
    else {
        id = str;
        label = "default";
    }
    
    Node* node = NULL;
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            node = *it;
            break;
        }
    }
    mutex.unlock();
    if(node == NULL) return NULL;
    return node->getOutput(label);
}

bool Nodes::apply() {
    // Reset buffer
    float* buffer = controller->getAudioDevices()->getBufferOutput();
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    memset(buffer, 0, sizeof(float) * framesPerBuffer * controller->getAudioDevices()->getOutputChannelCount());
    
    // Reset nodes
    resetNodes();
    
    mutex.lock();

    // Update all collectors
    for(auto it = collectors.begin();it != collectors.end(); ++it) {
        NodeCollector* collector = *it;
        float* collectorBuffer = collector->getOutput()->getBuffer();
        memset(collectorBuffer, 0, sizeof(float) * framesPerBuffer);
        
        // Loop through its list of key events
        MidiState* midiState = controller->getMidiState();
        std::vector<KeyEvent*>* keyEvents = collector->getKeyEvents();
        for(auto it = keyEvents->end() - 1;it != keyEvents->begin() - 1; --it) {
            // Delete the once that have expired
            KeyEvent* keyEvent = *it;
            double releaseTime = collector->getReleaseTime();
            if(keyEvent->release > releaseTime) {
                keyEvents->erase(it);
                delete keyEvent;
                continue;
            }
            
            // Update the output of this event
            midiState->updateKeyEvent(keyEvent);
            mutex.unlock();
            resetNodesKeyDependent();
            mutex.lock();
            
            NodeOutput* input = (NodeOutput*) collector->getInput()->pointer;
            currentKey = keyEvent;
            input->getNode()->update();
            
            // Add output of node
            float* inputOutput = input->getBuffer();
            for(int x = 0;x < framesPerBuffer; ++x)
                collectorBuffer[x] += inputOutput[x];
        }
    }
    
    // Update all nodes
    for(auto it = nodes.begin();it != nodes.end(); ++it) {
        Node* node = *it;
        if(!node->isKeyDependent())
            node->update();
    }
    
    // Add all outputs to the buffer
    for(auto it = audioOutputs.begin();it != audioOutputs.end(); ++it) {
        NodeAudioOutput* audioOutput = *it;
        int channelCount = controller->getAudioDevices()->getOutputChannelCount();
        for(int channel = 0;channel < channelCount; ++channel) {
            float* output = audioOutput->getChannel(channel)->getBuffer();
            for(int x = 0;x < framesPerBuffer; ++x)
                buffer[x * channelCount + channel] += output[x];
        }
    }
    
    mutex.unlock();
    return true;
}
