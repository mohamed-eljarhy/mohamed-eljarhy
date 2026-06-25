#include "FirebaseHandler.h"
#include <Arduino.h>

void FirebaseHandler::begin() {
    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void FirebaseHandler::checkFunction(void (*functions[])(), const char* functionNames[], int numFunctions) {
    if (Firebase.RTDB.getString(&fbdo, "/functionName")) {
        String functionName = fbdo.to<String>();
        if(functionName != "null") {
            for(int i = 0; i < numFunctions; i++) {
                if(functionName == functionNames[i]) {
                    functions[i]();
                    if(functionName != "esptrue") {
                        Firebase.RTDB.setString(&fbdo, "/functionName", "null");
                    }
                    return;
                }
            }
        }
    }
}
