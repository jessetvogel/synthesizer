#include "web.hpp"
#include "util.hpp"
#include "error.hpp"

std::regex Web::regexPages("^\\/(settings|help|debug)?$");
std::regex Web::regexGeneralResource("^\\/((?:js|css|fonts)\\/.*|favicon\\.ico)$");
std::regex Web::regexInstrumentResource("^\\/instrument\\/(\\w+\\/.+)$");
std::regex Web::regexExtension("^.*?(?:\\.(\\w+))?$");

bool Web::handle(Request* request, Response* response) {
    // Check if we are to handle this request
    std::string requestPath = request->getRequestPath();
    std::cmatch cm;
    
    // General resources
    if(std::regex_match(requestPath.c_str(), cm, regexGeneralResource)) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "../web/%s", std::string(cm[1]).c_str());
        std::string path(buffer);
        return sendFile(request, response, path);
    }
    
    // Instrument resource
    if(std::regex_match(requestPath.c_str(), cm, regexInstrumentResource)) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "../instruments/%s", std::string(cm[1]).c_str());
        std::string path(buffer);
        return sendFile(request, response, path);
    }
    
    // Pages
    if(std::regex_match(requestPath.c_str(), cm, regexPages)) {
        std::string page = cm[1];
        if(page.compare("") == 0)            return Web::sendFile(request, response, "../web/pages/main.html");
        if(page.compare("settings") == 0)    return Web::sendFile(request, response, "../web/pages/settings.html");
        if(page.compare("help") == 0)        return Web::sendFile(request, response, "../web/pages/help.html");
        if(page.compare("debug") == 0)        return Web::sendFile(request, response, "../web/pages/debug.html");
        
        return Error::respond(request, response, 404);
    }
    
    return false;
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
