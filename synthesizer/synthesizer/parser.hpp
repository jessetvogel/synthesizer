#ifndef parser_hpp
#define parser_hpp

#include <string>
#include <regex>

class Controller;

class Parser {

    static std::regex regexNeglect;
    
    Controller* controller;
    
    bool parse(std::string);
    
public:
    
    Parser(Controller*, std::string);
    
};

#endif
