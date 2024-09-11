#ifndef RENGO_LIBRARY_H
#define RENGO_LIBRARY_H

extern "C" {

// Function to get the app key
const char* GetAppKey();

// Function to get the server key
const char* GetServerKey();

// Function to get the fingerprint
const char* GetFingerPrint();

// Function to revert the config back
// Accepts a string (const char*) and returns a string (const char*)
const char* RevertConfigBack(const char* config);

// Function to revert a complex proxy
// Accepts a string (const char*) and returns a string (const char*)
const char* RevertComplexProxy(const char* proxy);

}
#endif //RENGO_LIBRARY_H
