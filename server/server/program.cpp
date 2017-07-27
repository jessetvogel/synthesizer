#include "program.hpp"
#include "info.hpp"

std::regex Program::regexRestart("^\\/restart$");
std::regex Program::regexExit("^\\/exit$");

Interface* Program::interface;

bool Program::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestURI = request->getRequestURI();
    std::cmatch cm;

    if(std::regex_match(requestURI.c_str(), cm, regexRestart)) {
        if(interface->restart())
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to restart\"}]}");
    }
    
    if(std::regex_match(requestURI.c_str(), cm, regexExit)) {
        if(interface->stop())
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to restart\"}]}");
    }
    
    return false;
}