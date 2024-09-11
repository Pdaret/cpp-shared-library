#include "library.h"
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <sstream>

const std::string APP_KEY = "573d23e9ff036761bfb8d179717453173d8191834b73cb9388d12a00228cb62e";
const std::string SERVER_KEY = "afa2d1267fa1609aa58e268312ca2940d7793c5688eb6a2fe9e00b5f9f68e014";
const std::string APP_STORE_FINGERPRINT = "C0:6F:83:53:83:12:07:D2:D6:76:C7:4C:E6:89:57:BB:B4:18:C6:23:EA:91:9C:9F:AE:D5:B4:F9:C1:89:22:9F";

extern "C" {

// Function to get the app key
const char* GetAppKey() {
    return APP_KEY.c_str();
}

// Function to get the server key
const char* GetServerKey() {
    return SERVER_KEY.c_str();
}

// Function to get the fingerprint
const char* GetFingerPrint() {
    return APP_STORE_FINGERPRINT.c_str();
}

// Function to revert config back
const char* RevertConfigBack(const char* config) {
    static std::string result;

    std::string configStr(config);

    if (configStr.find("vless://") != 0) {
        throw std::invalid_argument("invalid config format");
    }

    auto pos = configStr.find('@');
    if (pos == std::string::npos) {
        throw std::invalid_argument("invalid config format");
    }

    std::string uuid = configStr.substr(8, pos - 8);
    std::vector<std::string> uuidParts;
    std::istringstream iss(uuid);
    std::string part;
    while (std::getline(iss, part, '-')) {
        uuidParts.push_back(part);
    }
    if (uuidParts.size() != 5) {
        throw std::invalid_argument("invalid UUID format");
    }

    unsigned long long uuidFourthPart = std::stoull(uuidParts[3], nullptr, 16);
    uuidFourthPart--;
    std::stringstream ss;
    ss << std::hex << uuidFourthPart;
    uuidParts[3] = ss.str();

    std::string originalUUID = uuidParts[0] + "-" + uuidParts[1] + "-" + uuidParts[2] + "-" + uuidParts[3] + "-" + uuidParts[4];

    std::string ipPortAndQuery = configStr.substr(pos + 1);
    pos = ipPortAndQuery.find(':');
    if (pos == std::string::npos) {
        throw std::invalid_argument("invalid IP:port format");
    }

    std::string ip = ipPortAndQuery.substr(0, pos);
    std::string portAndQuery = ipPortAndQuery.substr(pos + 1);

    std::vector<std::string> ipParts;
    iss.clear();
    iss.str(ip);
    while (std::getline(iss, part, '.')) {
        ipParts.push_back(part);
    }
    if (ipParts.size() != 4) {
        throw std::invalid_argument("invalid IP address");
    }

    int lastByte = std::stoi(ipParts[3]);
    lastByte--;
    ipParts[3] = std::to_string(lastByte);

    std::string originalIP = ipParts[0] + "." + ipParts[1] + "." + ipParts[2] + "." + ipParts[3];

    pos = portAndQuery.find('?');
    std::string port = portAndQuery.substr(0, pos);
    std::string query = (pos == std::string::npos) ? "" : portAndQuery.substr(pos + 1);

    int portInt = std::stoi(port);
    portInt -= 5;
    std::string originalPort = std::to_string(portInt);

    result = "vless://" + originalUUID + "@" + originalIP + ":" + originalPort + "?" + query;
    return result.c_str();
}

const char* RevertComplexProxy(const char* proxy) {
    static std::string result(proxy);
    // Revert UUID
    std::regex uuidRegex(R"(([0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-)([0-9a-fA-F]{4})(-[0-9a-fA-F]{12}))");
    for (std::sregex_iterator it(result.begin(), result.end(), uuidRegex), end_it; it != end_it; ++it) {
        std::smatch match = *it;
        unsigned long long uuidFourthPart = std::stoull(match[2], nullptr, 16);
        uuidFourthPart--;
        std::stringstream ss;
        ss << std::hex << uuidFourthPart;
        std::string newUUID = match[1].str() + ss.str() + match[3].str();
        result.replace(match.position(0), match.length(0), newUUID);
    }

    // Revert IP address
    std::regex ipRegex(R"((\d+\.\d+\.\d+\.)(\d+))");
    for (std::sregex_iterator it(result.begin(), result.end(), ipRegex), end_it; it != end_it; ++it) {
        std::smatch match = *it;
        int lastByte = std::stoi(match[2]);
        lastByte--;
        std::string newIP = match[1].str() + std::to_string(lastByte);
        result.replace(match.position(0), match.length(0), newIP);
    }

    // Revert port
    std::regex portRegex(R"('port':(\d+))");
    for (std::sregex_iterator it(result.begin(), result.end(), portRegex), end_it; it != end_it; ++it) {
        std::smatch match = *it;
        int port = std::stoi(match[1]);
        port -= 5;
        std::string newPort = "'port':" + std::to_string(port);
        result.replace(match.position(0), match.length(0), newPort);
    }

    return result.c_str();
}

} // extern "C"
