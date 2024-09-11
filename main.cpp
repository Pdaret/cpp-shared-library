#include <iostream>
#include "library.h" // Include the header file for the shared library

int main() {
    // Test the GetAppKey function
    std::cout << "App Key: " << GetAppKey() << std::endl;

    // Test the GetServerKey function
    std::cout << "Server Key: " << GetServerKey() << std::endl;

    // Test the GetFingerPrint function
    std::cout << "App Store Fingerprint: " << GetFingerPrint() << std::endl;

    // Test RevertConfigBack
    const char* config = "vless://123e4567-e89b-12d3-a456-426614174000@192.168.1.100:443?somequery";
    const char* revertedConfig = RevertConfigBack(config);
    std::cout << "Reverted Config: " << revertedConfig << std::endl;

    // Test RevertComplexProxy
    const char* proxy = "{'port':443, 'uuid':'123e4567-e89b-12d3-a456-426614174000', 'ip':'192.168.1.100'}";
    const char* revertedProxy = RevertComplexProxy(proxy);
    std::cout << "Reverted Proxy: " << revertedProxy << std::endl;

    return 0;
}
