#ifndef FIREBASEHANDLER_H
#define FIREBASEHANDLER_H

#include <Firebase_ESP_Client.h>
#include "Config.h"

class FirebaseHandler {
public:
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    
    void begin();
    void checkFunction(void (*functions[])(), const char* functionNames[], int numFunctions);
};

#endif
