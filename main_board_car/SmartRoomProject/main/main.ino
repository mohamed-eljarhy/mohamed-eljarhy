#include <Arduino.h>
#include "Config.h"
#include "Pins.h"
#include "Utils.h"
#include "TableHandler.h"
#include "ServoHandler.h"
#include "WiFiHandler.h"
#include "FirebaseHandler.h"
#include "LightHandler.h"   // إضافة كلاس الإضاءة
#include <WiFi.h>
#include <WebServer.h>

// ==================== Global objects ====================
TableHandler tableHandler;
ServoHandler servoHandler;
WiFiHandler wifiHandler;
FirebaseHandler firebaseHandler;
LightHandler lightHandler(5, 18, 19); // كائن التحكم في الإضاءة

// ==================== Function pointers ====================
const char* functionNames[] = {
   
    "handletable1", "handletable2", "handletable3",
    "handlelighton","handlelightoff","handleTv",
    "handlespresso","handledeepLight"
};

void (*functions[])() = {
    handletable1, handletable2, handletable3,
    [](){ lightHandler.handleLightOn(); }, 
    [](){ lightHandler.handleLightOff(); },
   // handleTv,
    handlespresso,
    [](){ lightHandler.handleDeepLight(); }
};

const int numFunctions = sizeof(functionNames) / sizeof(functionNames[0]);

// ==================== Setup ====================
void setup() {
    Serial.begin(115200);

    // إعداد الواي فاي والسيرفر
    wifiHandler.begin();

    // إعداد Firebase
    firebaseHandler.begin();

    // إعداد الإضاءة
    //lightHandler.begin();

    // إعداد الطاولات والستائر
   // tableHandler.begin();
   // servoHandler.begin();
}

// ==================== Main Loop ====================
void loop() {
    // معالجة طلبات السيرفر
    wifiHandler.server.handleClient();

    // معالجة الوظائف القادمة من Firebase
    firebaseHandler.checkFunction(functions, functionNames, numFunctions);

    // تحديث مخرجات الطاولات والستائر
    tableHandler.update();
    servoHandler.update();

    // تحديث حالة الإضاءة إذا كان هناك grad أو deepLight
    lightHandler.update();

    // قراءة مدخلات الحساسات
    writeOutput();
    inputBus();

    delay(10); // تقليل الحمل على المعالج
}
