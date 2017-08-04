#include <algorithm>

#include "nodes.hpp"
#include "node.hpp"
#include "nodeinput.hpp"
#include "controller.hpp"
#include "audiodevices.hpp"
#include "settings.hpp"
#include "nodecustom.hpp"
#include "nodeparameter.hpp"
#include "nodeaudiooutput.hpp"
#include "nodecollector.hpp"
#include "keystate.hpp"

#include "nodefactory.hpp"
#include "options.hpp"

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
    // Note that all other lists (parameters, collectors, audio_output's, etc.) are subsets of nodes
    
    for(auto it = constants.begin(); it != constants.end(); ++it)
        delete *it;
    mutex.unlock();
}

bool Nodes::create(std::string type, std::string id, std::string args) {
    // Check if node with this id already exists
    Node* node = getNode(id);
    if(node != NULL) { Status::addError("Node id is already used"); return false; }
    
    // Create new node
    Options options(controller, args);
    node = NodeFactory::create(controller, type, id, options);
    if(node == NULL) return false;
    
    // Append node to relevant lists
    addNode(node);
    std::string nodeType = node->getType();
    if(nodeType.compare("parameter") == 0) addNodeParameter((NodeParameter*) node);
    if(nodeType.compare(0, 9, "collector") == 0) addCollector((NodeCollector*) node); // TODO
    if(nodeType.compare("audio_output") == 0) addAudioOutput((NodeAudioOutput*) node);
    return true;
}

bool Nodes::destroy(std::string id) {
    // Find node with given id
    Node* node = NULL;
    mutex.lock();
    auto it = nodes.begin();
    for(;it != nodes.end(); ++it) {
        if(id.compare((*it)->getId()) == 0) {
            node = *it;
            break;
        }
    }
    mutex.unlock();

    // In case it does not exists, return
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    
    // Make sure there are no references to this node
    bool reference = false;
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        if((*it)->dependsOn(node)) {
            reference = true;
            break;
        }
    }
    mutex.unlock();
    if(reference) { Status::addError("Cannot delete node as other nodes depend on it"); return false; }
    
    // Remove node from (other) relevant lists
    std::string nodeType = node->getType();
    if(nodeType.compare("parameter") == 0) removeNodeParameter((NodeParameter*) node);
    if(nodeType.compare("collector") == 0) removeCollector((NodeCollector*) node);
    if(nodeType.compare("audio_output") == 0) removeAudioOutput((NodeAudioOutput*) node);
    
    // Erase from list and delete object
    nodes.erase(it);
    delete node;
    
    return true;
}

bool Nodes::rename(std::string oldId, std::string newId) {
    Node* node = getNode(oldId);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    
    Node* nodeNew = getNode(newId);
    if(nodeNew != NULL) { Status::addError("Node id is already used"); return false; }
    
    return node->setId(newId);
}

bool Nodes::set(std::string strNodeInput, std::string value) {
    NodeInput* nodeInput = getNodeInput(strNodeInput);
    if(nodeInput == NULL) { Status::addError("Provided node input does not exist"); return false; }
    
    return nodeInput->set(value);
}

bool Nodes::hide(std::string id) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    
    node->hide();
    return true;
}

bool Nodes::info(std::string id) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }

    Status::addExtra(std::string("info:").append(id));
    return true;
}

bool Nodes::activate(std::string id) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    
    node->setActive(true);
    return true;
}

bool Nodes::deactivate(std::string id) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    
    node->setActive(false);
    return true;
}

bool Nodes::addInput(std::string id, std::string label, std::string nodeInput) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    if(node->getType().compare("custom") != 0) { Status::addError("Can only add inputs to custom nodes"); return false; }
    
    NodeInput* input = getNodeInput(nodeInput);
    if(input == NULL) { Status::addError("Provided node input does not exist"); return false; }
    
    bool success = node->addInput(label, input);
    if(success) {
        Node* previousOwner = getNode(nodeInput.substr(0, nodeInput.find("."))); // TODO, do this better
        previousOwner->removeInput(input);
    }
    
    return success;
}

bool Nodes::addOutput(std::string id, std::string label, std::string nodeOutput) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    if(node->getType().compare("custom") != 0) { Status::addError("Can only add outputs to custom nodes"); return false; }
    
    NodeOutput* output = getNodeOutput(nodeOutput);
    if(output == NULL) { Status::addError("Provided node output does not exist"); return false; }
    
    bool success = node->addOutput(label, output);
    if(success) {
        Node* previousOwner = getNode(nodeOutput.substr(0, nodeOutput.find(":"))); // TODO, do this better
        previousOwner->removeOutput(output);
    }
    
    return success;
}

bool Nodes::attach(std::string id, std::string attachment) {
    Node* node = getNode(id);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    if(node->getType().compare("custom") != 0) { Status::addError("Nodes can only be attached to custom nodes"); return false; }
    NodeCustom* nodeCustom = (NodeCustom*) node;
    
    node = getNode(attachment);
    if(node == NULL) { Status::addError("Provided node does not exist"); return false; }
    if(node == nodeCustom) { Status::addError("Cannot attach node to itself"); return false; }
    
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

NodeOutput* Nodes::createConstant(double value) {
    NodeOutput* nodeOutput = new NodeOutput(controller, NULL);
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    float* buffer = nodeOutput->getBuffer();
    for(int x = 0;x < framesPerBuffer; ++x)
        buffer[x] = value;
    return nodeOutput;
}

bool Nodes::deleteConstant(NodeOutput* nodeOutput) {
    bool found = false;
    mutex.lock();
    auto position = std::find(constants.begin(), constants.end(), nodeOutput);
    if(position != constants.end()) {
        constants.erase(position);
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
    parameters.push_back(parameter);
    mutex.unlock();
    return true;
}

bool Nodes::updateNodeParameter(int midiCC, double value) {
    mutex.lock();
    for(auto it = parameters.begin();it != parameters.end(); ++it) {
        NodeParameter* parameter = *it;
        if(parameter->getMidiCC() == midiCC)
            parameter->setValue(value);
    }
    mutex.unlock();
    return true;
}

bool Nodes::removeNodeParameter(NodeParameter* parameter) {
    bool found = false;
    mutex.lock();
    auto position = std::find(parameters.begin(), parameters.end(), parameter);
    if(position != parameters.end()) {
        parameters.erase(position);
        found = true;
    }
    mutex.unlock();
    return found;
}

void Nodes::addKeyEvent(KeyEvent* keyEvent) {
    // Send this key event to all collectors
    mutex.lock();
    for(auto it = collectors.begin(); it != collectors.end(); ++it) {
        (*it)->addKeyEvent(keyEvent);
    }
    mutex.unlock();
}

void Nodes::resetNodes() {
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
        (*it)->reset();
    mutex.unlock();
}

void Nodes::resetNodesVoiceDependent() {
//    mutex.lock(); // TODO: tmp solution
    for(auto it = nodes.begin(); it != nodes.end(); ++it) {
        Node* node = *it;
        if(node->isVoiceDependent())
            node->reset();
    }
//    mutex.unlock();
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

bool Nodes::clear() {
    mutex.lock();
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
        delete *it;
    // Note that all other lists (parameters, collectors, audio_output's, etc.) are subsets of nodes
    
    for(auto it = constants.begin(); it != constants.end(); ++it)
        delete *it;
    
    // Clear all lists
    nodes.clear();
    collectors.clear();
    audioOutputs.clear();
    parameters.clear();
    
    mutex.unlock();
    
    // Recreate default nodes
    NodeFactory::createDefaultNodes(controller, this);

    return true;
}

bool Nodes::apply() {
    // Reset buffer
    float* buffer = controller->getAudioDevices()->getBufferOutput();
    unsigned long framesPerBuffer = controller->getSettings()->bufferSize;
    memset(buffer, 0, sizeof(float) * framesPerBuffer * controller->getAudioDevices()->getOutputChannelCount());
    
    // Reset nodes
    resetNodes();
    
    mutex.lock();
    
    // Update all audio outputs, and add them to the buffer
    double volume = controller->getSettings()->masterVolume;
    volume *= volume;
    for(auto it = audioOutputs.begin();it != audioOutputs.end(); ++it) {
        NodeAudioOutput* audioOutput = *it;
        audioOutput->update();
        int channelCount = controller->getAudioDevices()->getOutputChannelCount();
        for(int channel = 0;channel < channelCount; ++channel) {
            float* output = audioOutput->getChannel(channel)->getBuffer();
            for(int x = 0;x < framesPerBuffer; ++x)
                buffer[x * channelCount + channel] += output[x] * volume;
        }
    }
    
    mutex.unlock();
    return true;
}
