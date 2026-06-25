// LightHandler.cpp
#include "LightHandler.h"

LightHandler::LightHandler(int r, int g, int b) {
    rgbPinR = r;
    rgbPinG = g;
    rgbPinB = b;
    deepLightState = false;
    gradValue = 0;

    pinMode(rgbPinR, OUTPUT);
    pinMode(rgbPinG, OUTPUT);
    pinMode(rgbPinB, OUTPUT);
    turnOffAll();
}

void LightHandler::handleLightOn() {
    deepLightState = true;
    setRGB(255, 255, 255);  // إضاءة كاملة بيضاء
}

void LightHandler::handleLightOff() {
    deepLightState = false;
    turnOffAll();
}

void LightHandler::handleGrad() {
    // مثال لتأثير تدرجي RGB
    gradValue += 5;
    if (gradValue > 255) gradValue = 0;
    setRGB(gradValue, 255 - gradValue, gradValue / 2);
}

void LightHandler::handleDeepLight() {
    if (deepLightState) {
        setRGB(255, 200, 150); // إضاءة عميقة أو خاصة
    } else {
        turnOffAll();
    }
}

// دوال مساعدة
void LightHandler::setRGB(int r, int g, int b) {
    analogWrite(rgbPinR, r);
    analogWrite(rgbPinG, g);
    analogWrite(rgbPinB, b);
}

void LightHandler::turnOffAll() {
    analogWrite(rgbPinR, 0);
    analogWrite(rgbPinG, 0);
    analogWrite(rgbPinB, 0);
}

// إذا أحببت يمكن إضافة update() للتحكم في Grad تلقائيًا
void LightHandler::update() {
    handleGrad();
}
