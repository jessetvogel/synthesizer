#ifndef nodeleadkeyinfo_hpp
#define nodeleadkeyinfo_hpp

#include "node.hpp"

class NodeLeadKeyInfo : public Node {
    
public:

    enum InfoType { Frequency, Velocity, Duration, Release, Pressing };
    
private:
    
    InfoType infoType;
    
    NodeOutput* output;
    
public:
    
    NodeLeadKeyInfo(Controller*, InfoType);
    
    void apply();
    
};


#endif
