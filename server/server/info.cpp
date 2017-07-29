#include "info.hpp"
#include "util.hpp"
#include "error.hpp"
#include <sstream>
#include <dirent.h>

std::regex Info::regexInfo("^\\/info\\/(\\w+)$");

bool Info::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestPath = request->getRequestPath();
    std::cmatch cm;
    if(!std::regex_match(requestPath.c_str(), cm, regexInfo)) return false;
    
    if(cm[1].compare("instruments") == 0) {
        std::stringstream oss;
        oss << "{\"instruments\":[";
        DIR *directory;
        struct dirent *entry;
        bool comma = false;
        if((directory = opendir("../instruments"))) {
            while((entry = readdir(directory))) {
                if(strcmp(entry->d_name, ".") == 0) continue;
                if(strcmp(entry->d_name, "..") == 0) continue;
                if(Util::isFile("../instruments/" + std::string(entry->d_name))) continue;
                
                if(comma) oss << ','; else comma = true;
                oss << "\"" << entry->d_name << "\"";
            }
            closedir(directory);
        }
        oss << "]}";
        return writeJSON(request, response, oss.str());
    }
    
    return Error::respond(request, response, 404);
}

bool Info::writeJSON(Request* request, Response* response, std::string json) {
    response->setHeader("Content-Type", Util::MIMEType("json"));
    response->setHeader("Content-Length", std::to_string(json.length()));
    
    response->writeStatus();
    response->writeHeaders();
    response->writeLine(json);

    return true;
}
