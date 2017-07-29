#include "data.hpp"
#include "web.hpp"
#include "info.hpp"
#include "error.hpp"
#include "util.hpp"
#include <fstream>

#include <iostream>

std::regex Data::regexSettingsStore("^\\/data\\/settings\\/store$");
std::regex Data::regexInstrumentData("^\\/data\\/instrument\\/(\\w+)\\/get$");
std::regex Data::regexInstrumentStore("^\\/data\\/instrument\\/(\\w+)\\/store$");

bool Data::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestPath = request->getRequestPath();
    std::cmatch cm;
    
    // Settings
    if(std::regex_match(requestPath.c_str(), cm, regexSettingsStore)) {
        if(Data::store("../settings/settings", request->getBody()))
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to store launch data\"}]}");
    }
    
    // Instruments
    if(std::regex_match(requestPath.c_str(), cm, regexInstrumentData)) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "../instruments/%s/data.json", std::string(cm[1]).c_str());
        std::string path(buffer);
        if(!Util::fileExists(path)) Data::store(path, "{}");
        return Web::sendFile(request, response, path);
    }
    
    if(std::regex_match(requestPath.c_str(), cm, regexInstrumentStore)) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "../instruments/%s/data.json", std::string(cm[1]).c_str());
        std::string path(buffer);
        if(!Util::fileExists(path)) {
            std::cout << "file '" << path << "' does not exist!" << std::endl;
            return Error::respond(request, response, 404);
        }
        
        if(Data::store(path, request->getBody()))
            return Info::writeJSON(request, response, "{}");
        else
            return Info::writeJSON(request, response, "{\"error\":[{\"message\":\"Unable to store settings\"}]}");
    }
    
    return false;
}

bool Data::store(std::string path, std::string contents) {
    std::ofstream file(path);
    if(file.fail()) return false;
    
    file << contents << std::endl;
    
    file.close();
    return true;
}