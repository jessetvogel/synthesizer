#include "web.hpp"
#include "util.hpp"
#include "error.hpp"

std::regex Web::regexWeb("^\\/((?:js|css|fonts)\\/.*|favicon\\.ico)$");
std::regex Web::regexExtension("^.*?(?:\\.(\\w+))?$");

bool Web::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestURI = request->getRequestURI();
    std::cmatch cm;
    if(!std::regex_match(requestURI.c_str(), cm, regexWeb)) return false;
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "../web/%s", std::string(cm[1]).c_str());
    std::string path(buffer);
    
    return sendFile(request, response, path);
}

bool Web::sendFile(Request* request, Response* response, std::string path) {
    std::cmatch cm;
    if(!std::regex_match(path.c_str(), cm, regexExtension)) return false;
    
    // Check if the file exists
    if(!Util::fileExists(path))
        return Error::respond(request, response, 404);
    
    response->setHeader("Content-Type", Util::MIMEType(cm[1]));
    
    response->writeStatus();
    response->writeHeaders();
    response->writeFile(path);
    
    return true;
}
