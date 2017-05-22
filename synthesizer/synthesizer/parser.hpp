#ifndef parser_hpp
#define parser_hpp

#include <string>

class Controller;

class Parser {

    Controller* controller;
    std::string filepath;
    std::string directory;
    
public:
    
    Parser(Controller*);
    
    inline void setDirectory(std::string directory) { this->directory = directory; };
    
    bool parseFile(std::string);
    bool parseLine(std::string);
};

#endif
