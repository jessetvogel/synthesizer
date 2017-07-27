#include "util.hpp"
#include <fstream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

bool Util::isInteger(std::string string) {
    for(int i = 0;i < string.length(); ++i) {
        char c = string[i];
        if(c < '0' || c > '9') return false;
    }
    return true;
}

bool Util::isFile(std::string path) {
    struct stat fileStat;
    ::stat(path.c_str(), &fileStat);
    return S_ISREG(fileStat.st_mode);
}

bool Util::fileExists(std::string path) {
    std::ifstream file(path);
    return file.good() && isFile(path);
}

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Basics_of_HTTP/MIME_types/Complete_list_of_MIME_types
std::string Util::MIMEType(std::string extension) {
    extension = toLowerCase(extension);
    
    if(extension.compare("html") == 0) return "text/html";
    if(extension.compare("css") == 0) return "text/css";

    if(extension.compare("json") == 0) return "application/json";
    if(extension.compare("js") == 0) return "application/javascript";
    if(extension.compare("eot") == 0) return "application/vnd.ms-fontobject";
    
    if(extension.compare("png") == 0) return "image/png";
    if(extension.compare("ico") == 0) return "image/x-icon";
    if(extension.compare("jpg") == 0) return "image/jpeg";
    if(extension.compare("jpeg") == 0) return "image/jpeg";
    if(extension.compare("svg") == 0) return "image/svg+xml";
    
    if(extension.compare("ttf") == 0) return "font/ttf";
    if(extension.compare("woff") == 0) return "font/woff";
    if(extension.compare("woff2") == 0) return "font/woff2";
    
    // By default
    return "text/plain";
}

std::string Util::toLowerCase(std::string string) {
    std::transform(string.begin(), string.end(), string.begin(), ::tolower);
    return string;
}
