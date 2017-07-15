#ifndef nodekeyinfo_hpp
#define nodekeyinfo_hpp

#include "node.hpp"

class NodeKeyInfo : public Node {

public:
    
    enum InfoType { Frequency, Velocity, Duration, Release };
    
private:
    
    InfoType infoType;

    NodeOutput* output;
    
public:
    
    NodeKeyInfo(Controller*, InfoType);
    
    void apply();
    
};

#endif
