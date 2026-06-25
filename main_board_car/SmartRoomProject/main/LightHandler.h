// LightHandler.h
#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

#include <Arduino.h>

class LightHandler {
public:
   LightHandler(int r, int g, int b); 

    void handleLightOn();
    void handleLightOff();
    void handleGrad();
    void handleDeepLight();

    void update(); // لتحديث الإضاءة في loop إذا لزم

private:
    int rgbPinR;
    int rgbPinG;
    int rgbPinB;

    bool deepLightState;
    int gradValue;

    void setRGB(int r, int g, int b);
    void turnOffAll();
};

#endif
