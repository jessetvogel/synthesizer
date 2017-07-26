#include "api.hpp"
#include <iostream>
#include "util.hpp"

std::regex API::regexRequestURI("^\\/api\\/(.+)$");

Interface* API::interface;

bool API::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestURI = request->getRequestURI();
    std::cmatch cm;
    if(!std::regex_match(requestURI.c_str(), cm, regexRequestURI)) return false;
    
    std::string line = interface->command(cm[1]);
    
    // Respond
    response->setHeader("Content-Type", Util::MIMEType("json"));
    
    response->writeStatus();
    response->writeHeaders();
    response->writeLine(line);
    
    return true;
}
