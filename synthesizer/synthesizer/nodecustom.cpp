#include "nodecustom.hpp"

NodeCustom::NodeCustom(Controller* controller, Arguments) : Node(controller) {
    // Set type
    type = "custom";
}

NodeCustom::~NodeCustom() {
    for(auto it = attachments.begin();it != attachments.end(); ++it)
        delete *it;
}

bool NodeCustom::attach(Node* node) {
    attachments.push_back(node);
    return true;
}
