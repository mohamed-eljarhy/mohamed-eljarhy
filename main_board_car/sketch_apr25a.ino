#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Stepper.h>
#include <Firebase_ESP_Client.h>
#include <ESPmDNS.h> // ✅ إضافة مكتبة mDNS

// --- إعدادات Firebase والشبكة ---
#define FIREBASE_HOST "https://vipcar-v1-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "O1ZcCeCnVjrqX4J44FEygwIpyrpEVI5ZjJxRIwFw"
#define STASSID "Galaxy A26"
#define STAPSK "solidworks"

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
WebServer server(80);

// --- تعريف الأرجل (Pins) ---
#define ser 14
#define clk 12
#define latch 15
#define shld 16
#define clk2 18
#define clkin 17
#define qh 13
#define stepPin 22
#define dirPin 23
#define ena1 27
#define ena4 2
#define ena5 21
#define SDA_PIN 25
#define SCL_PIN 26

// --- متغيرات الحالة (Global Variables) ---
const int stepsPerRevolution = 200;
Adafruit_PWMServoDriver pwm;
bool in[40];
byte p1=0, p2=0, p3=0, p4=0, p5=0;
bool dir = true;
bool shuttersWork = false;
bool activeTv = false;
bool DirTv = false;
int grad = 0;
double red=0, green=0, blue=0;
unsigned long firebaseMillis = 0;

// عدادات الحالة والتبديل
int boolshutter1=1, boolshutter2=1, boolshutter3=1, boolshutter4=1, boolshutter5=1;
int boolWhiteColor=1, boolrgbColor=1, boolTv=1;
bool tUp1=0, tDown1=0, tUp2=0, tDown2=0, tUp3=0, tDown3=0, tUp4=0, tDown4=0, tUp5=0, tDown5=0;

const int servoMin = 150, servoMax = 600;

// --- مهام FreeRTOS ---
TaskHandle_t MotorTaskHandle;
TaskHandle_t IoTTaskHandle;

// --- الدوال الأساسية للـ Shift Registers ---
void writeOutput(){
    digitalWrite(latch, LOW);
    shiftOut(ser, clk, MSBFIRST, p5);
    shiftOut(ser, clk, MSBFIRST, p4);
    shiftOut(ser, clk, MSBFIRST, p3);
    shiftOut(ser, clk, MSBFIRST, p2);
    shiftOut(ser, clk, MSBFIRST, p1);
    digitalWrite(latch, HIGH);
}

void inputBus(){ 
    digitalWrite(shld, LOW); delayMicroseconds(5); digitalWrite(shld, HIGH); delayMicroseconds(5); 
    digitalWrite(clkin, HIGH); digitalWrite(clk2, HIGH); digitalWrite(clkin, LOW);
    for(int i=0; i<5; i++) {
        byte rec = shiftIn(qh, clk2, MSBFIRST);
        for(int j=0; j<8; j++) in[j + (i*8)] = bitRead(rec, j);
    }
}

// --- دالة الحركة بنظام RTOS (غير حاجبة للنواة) ---
void runStepperRTOS(int steps, int speedDelay, int limitIndex, bool forwardDir) {
    digitalWrite(dirPin, forwardDir);
    for (int i = 0; i < steps; i++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(speedDelay);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(speedDelay);
        if (i % 20 == 0) { 
            inputBus();
            if (in[limitIndex] == HIGH) break;
        }
        if (i % 100 == 0) vTaskDelay(1); 
    }
}

// --- دوال الترابيزات (Tables) ---
void handletable1(){ if(shuttersWork) return; digitalWrite(ena1, LOW); bitWrite(p5, 0, 0); writeOutput(); dir = !dir; runStepperRTOS(8700, 600, (dir ? 11 : 12), dir); digitalWrite(ena1, HIGH); bitWrite(p5, 0, 1); writeOutput(); }
void handletable2(){ if(shuttersWork) return; bitWrite(p5, 1, 0); writeOutput(); dir = !dir; runStepperRTOS(6800, 700, (dir ? 13 : 14), dir); bitWrite(p5, 1, 1); writeOutput(); }
void handletable3(){ if(shuttersWork) return; bitWrite(p5, 2, 0); writeOutput(); dir = !dir; runStepperRTOS(7000, 700, (dir ? 16 : 17), dir); bitWrite(p5, 2, 1); writeOutput(); }
void handletable4(){ if(shuttersWork) return; digitalWrite(ena4, LOW); bitWrite(p5, 3, 0); writeOutput(); dir = !dir; runStepperRTOS(8750, 600, (dir ? 18 : 19), dir); digitalWrite(ena4, HIGH); bitWrite(p5, 3, 1); writeOutput(); }
void handlespresso(){ if(shuttersWork) return; digitalWrite(ena5, LOW); bitWrite(p5, 4, 0); writeOutput(); dir = !dir; runStepperRTOS(11000, 600, (dir ? 20 : 21), dir); digitalWrite(ena5, HIGH); bitWrite(p5, 4, 1); writeOutput(); }

// --- دوال الستائر (Shutters) ---
void handleshr1() { shuttersWork=1; if(!in[1]) pwm.setPWM(0, 0, map(100, -180, 180, servoMin, servoMax)); tUp1=1; }
void handleshs1() { pwm.setPWM(0, 0, 150); shuttersWork=0; tUp1=0; tDown1=0; }
void handleshl1() { shuttersWork=1; if(!in[0]) pwm.setPWM(0, 0, map(-110, -180, 180, servoMin, servoMax)); tDown1=1; }

void handleshr2() { shuttersWork=1; if(!in[3]) pwm.setPWM(1, 0, map(100, -180, 180, servoMin, servoMax)); tUp2=1; }
void handleshs2() { pwm.setPWM(1, 0, 150); shuttersWork=0; tUp2=0; tDown2=0; }
void handleshl2() { shuttersWork=1; if(!in[2]) pwm.setPWM(1, 0, map(-110, -180, 180, servoMin, servoMax)); tDown2=1; }

void handleshr3() { shuttersWork=1; if(!in[5]) pwm.setPWM(2, 0, map(105, -180, 180, servoMin, servoMax)); tUp3=1; }
void handleshs3() { pwm.setPWM(2, 0, 150); shuttersWork=0; tUp3=0; tDown3=0; }
void handleshl3() { shuttersWork=1; if(!in[4]) pwm.setPWM(2, 0, map(-110, -180, 180, servoMin, servoMax)); tDown3=1; }

void handleshr4() { shuttersWork=1; if(!in[8]) pwm.setPWM(3, 0, map(100, -180, 180, servoMin, servoMax)); tUp4=1; }
void handleshs4() { pwm.setPWM(3, 0, 150); shuttersWork=0; tUp4=0; tDown4=0; }
void handleshl4() { shuttersWork=1; if(!in[6]) pwm.setPWM(3, 0, map(-110, -180, 180, servoMin, servoMax)); tDown4=1; }

void handleshr5() { shuttersWork=1; if(!in[10]) pwm.setPWM(4, 0, map(100, -180, 180, servoMin, servoMax)); tUp5=1; }
void handleshs5() { pwm.setPWM(4, 0, 150); shuttersWork=0; tUp5=0; tDown5=0; }
void handleshl5() { shuttersWork=1; if(!in[9]) pwm.setPWM(4, 0, map(-110, -180, 180, servoMin, servoMax)); tDown5=1; }

// --- الإضاءة والتلفاز ---
void handlelighton(){ bitWrite(p1,0,1); bitWrite(p1,1,1); bitWrite(p1,2,0); grad=0; writeOutput(); }
void handlelightoff(){ bitWrite(p1,0,0); bitWrite(p1,1,0); bitWrite(p1,2,0); grad=0; writeOutput(); }
void handleTv(){ switch(boolTv){ case 1: DirTv=!DirTv; bitWrite(p1,4,0); bitWrite(p1,5,0); activeTv=true; boolTv++; break; case 2: activeTv=false; bitWrite(p1,4,0); bitWrite(p1,5,0); boolTv=1; break; } writeOutput(); }

void handlergbcolor() {
    if (server.method() == HTTP_POST) {
        StaticJsonDocument<200> doc;
        deserializeJson(doc, server.arg("plain"));
        String hex = doc["color"] | "#000000";
        double vol = doc["_volume"] | 1.0;
        long val = strtol(&hex[1], NULL, 16);
        analogWrite(4, (int)(((val >> 16) & 0xFF) * vol));
        analogWrite(5, (int)(((val >> 8) & 0xFF) * vol));
        analogWrite(19, (int)((val & 0xFF) * vol));
    }
    server.send(200, "text/plain", "OK");
}

// --- مهمة IoT (نواة 1) ---
void IoTTask(void * pvParameters){
    for(;;){
        server.handleClient();
        if(millis() - firebaseMillis > 1500){
            if(Firebase.ready()){
                if (Firebase.RTDB.getString(&fbdo, "/functionName")) {
                    String f = fbdo.to<String>();
                    if(f != "null" && f != ""){
                        if(f == "handletable1") handletable1();
                        else if(f == "handletable2") handletable2();
                        else if(f == "handletable3") handletable3();
                        else if(f == "handletable4") handletable4();
                        else if(f == "handlespresso") handlespresso();
                        else if(f == "handlelighton") handlelighton();
                        else if(f == "handlelightoff") handlelightoff();
                        Firebase.RTDB.setString(&fbdo, "/functionName", "null");
                    }
                }
            }
            firebaseMillis = millis();
        }
        vTaskDelay(5);
    }
}

// --- مهمة المحركات والحساسات (نواة 0) ---
void MotorTask(void * pvParameters){
    for(;;){
        inputBus();
        writeOutput();
        
        // حماية الستائر التلقائية (Auto-stop)
        if (tUp1 && in[1]) handleshs1(); if (tDown1 && in[0]) handleshs1();
        if (tUp2 && in[3]) handleshs2(); if (tDown2 && in[2]) handleshs2();
        if (tUp3 && in[5]) handleshs3(); if (tDown3 && in[4]) handleshs3();
        if (tUp4 && in[8]) handleshs4(); if (tDown4 && in[6]) handleshs4();
        if (tUp5 && in[10]) handleshs5(); if (tDown5 && in[9]) handleshs5();

        vTaskDelay(20);
    }
}

void setup(){
    Serial.begin(115200);
    
    // تعريف كافة الأرجل
    pinMode(ser,OUTPUT); pinMode(latch,OUTPUT); pinMode(clk,OUTPUT);
    pinMode(shld,OUTPUT); pinMode(clk2,OUTPUT); pinMode(clkin,OUTPUT);
    pinMode(qh,INPUT); pinMode(stepPin, OUTPUT); pinMode(dirPin, OUTPUT);
    pinMode(ena1, OUTPUT); pinMode(ena4, OUTPUT); pinMode(ena5, OUTPUT);
    pinMode(4, OUTPUT); pinMode(5, OUTPUT); pinMode(19, OUTPUT);
    
    digitalWrite(ena1, HIGH); digitalWrite(ena4, HIGH); digitalWrite(ena5, HIGH);

    WiFi.begin(STASSID, STAPSK);
    while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
    Serial.println("\nWiFi Connected");
    
    
    // ✅ إعداد mDNS
    if (!MDNS.begin("vipcar")) { // سيعمل الجهاز الآن تحت اسم vipcar.local
        Serial.println("Error setting up MDNS responder!");
    } else {
        Serial.println("mDNS responder started: vipcar.local");
    }
    Wire.begin(SDA_PIN, SCL_PIN);
    pwm.begin(); pwm.setPWMFreq(50);

    // تسجيل كافة مسارات السيرفر
    server.on("/table1", handletable1); server.on("/table2", handletable2);
    server.on("/table3", handletable3); server.on("/table4", handletable4);
    server.on("/spresso", handlespresso);
    server.on("/shr1", handleshr1); server.on("/shs1", handleshs1); server.on("/shl1", handleshl1);
    server.on("/shr2", handleshr2); server.on("/shs2", handleshs2); server.on("/shl2", handleshl2);
    server.on("/shr3", handleshr3); server.on("/shs3", handleshs3); server.on("/shl3", handleshl3);
    server.on("/shr4", handleshr4); server.on("/shs4", handleshs4); server.on("/shl4", handleshl4);
    server.on("/shr5", handleshr5); server.on("/shs5", handleshs5); server.on("/shl5", handleshl5);
    server.on("/lighton", handlelighton); server.on("/lightoff", handlelightoff);
    server.on("/handlergbcolor", handlergbcolor);
    server.begin();

    MDNS.addService("http", "tcp", 80);

    config.host = FIREBASE_HOST;
    config.signer.tokens.legacy_token = FIREBASE_AUTH;
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // إنشاء المهام وتوزيعها على الأنوية
    xTaskCreatePinnedToCore(IoTTask, "IoTTask", 10000, NULL, 1, &IoTTaskHandle, 1);
    xTaskCreatePinnedToCore(MotorTask, "MotorTask", 10000, NULL, 1, &MotorTaskHandle, 0);
    
    Serial.println("VIP Car System Fully Loaded!");
}

void loop(){
    vTaskDelete(NULL); // إلغاء الـ Loop التقليدي والاعتماد على الـ Tasks
}