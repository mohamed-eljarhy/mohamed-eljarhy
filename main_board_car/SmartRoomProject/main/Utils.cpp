#include "Utils.h"
#include <Arduino.h>
#include "Globals.h"
// ==================== متغيرات خارجية ====================
 bool in[40];
byte p1=B00000000;
byte p2=B00000000;
byte p3=B00000000;
byte p4=B00000000;
byte p5=B00000000;
 bool shuttersWork=true;
extern Adafruit_PWMServoDriver pwm;

// ==================== دوال مساعدة ====================
void inputBus() {
    // هنا ضع كود قراءة المدخلات كما في مشروعك الأصلي
}

void writeOutput() {
    // هنا ضع كود إخراج القيم كما في مشروعك الأصلي
}

// ==================== دوال الطاولات ====================
void handletable1() {
    if (shuttersWork == 0) {
        bitWrite(p5, 0, 0);
        writeOutput();
        bool dir = true;
        digitalWrite(dirPin, dir);
        for (int i = 0; i < 8750; i++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(700);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(700);
            inputBus();
            if (in[12] == HIGH) break;
        }
        bitWrite(p5, 0, 1);
    }
}

void handletable2() {
    if (shuttersWork == 0) {
        bitWrite(p5, 1, 0);
        writeOutput();
        bool dir = true;
        digitalWrite(dirPin, dir);
        for (int i = 0; i < 9000; i++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(700);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(700);
            inputBus();
            if (in[13] == HIGH) break;
        }
        bitWrite(p5, 1, 1);
    }
}

void handletable3() {
    if (shuttersWork == 0) {
        bitWrite(p5, 2, 0);
        writeOutput();
        bool dir = true;
        digitalWrite(dirPin, dir);
        for (int i = 0; i < 9000; i++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(700);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(700);
            inputBus();
            if (in[16] == HIGH) break;
        }
        bitWrite(p5, 2, 1);
    }
}

void handletable4() {
    if (shuttersWork == 0) {
        digitalWrite(ena4, LOW);
        bitWrite(p5, 3, 0);
        writeOutput();
        bool dir = true;
        digitalWrite(dirPin, dir);
        for (int i = 0; i < 10800; i++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(600);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(600);
            inputBus();
            if (in[18] == HIGH) break;
        }
        digitalWrite(ena4, HIGH);
        bitWrite(p5, 3, 1);
    }
}

void handlespresso() {
    if (shuttersWork == 0) {
        digitalWrite(ena5, LOW);
        bitWrite(p5, 4, 0);
        writeOutput();
        bool dir = true;
        digitalWrite(dirPin, dir);
        for (int i = 0; i < 11000; i++) {
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(600);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(600);
            inputBus();
            if (in[20] == HIGH) break;
        }
        digitalWrite(ena5, HIGH);
        bitWrite(p5, 4, 1);
    }
}

// ==================== دوال السرفو للطاولات ====================
void handleshr1() { shuttersWork = 1; if (in[1] == 0) pwm.setPWM(0, 0, map(100, -180, 180, servoMin, servoMax)); tUp1 = 1; }
void handleshs1() { pwm.setPWM(0, 150, 150); shuttersWork = 0; }
void handleshl1() { shuttersWork = 1; if (in[0] == 0) pwm.setPWM(0, 0, map(-110, -180, 180, servoMin, servoMax)); tDown1 = 1; }

void handleshr2() { shuttersWork = 1; if (in[3] == 0) pwm.setPWM(1, 0, map(100, -180, 180, servoMin, servoMax)); tUp2 = 1; }
void handleshs2() { pwm.setPWM(1, 150, 150); shuttersWork = 0; }
void handleshl2() { shuttersWork = 1; if (in[2] == 0) pwm.setPWM(1, 0, map(-110, -180, 180, servoMin, servoMax)); tDown2 = 1; }

void handleshr3() { shuttersWork = 1; if (in[5] == 0) pwm.setPWM(2, 0, map(105, -180, 180, servoMin, servoMax)); tUp3 = 1; }
void handleshs3() { pwm.setPWM(2, 150, 150); shuttersWork = 0; }
void handleshl3() { shuttersWork = 1; if (in[4] == 0) pwm.setPWM(2, 0, map(-110, -180, 180, servoMin, servoMax)); tDown3 = 1; }

void handleshr4() { shuttersWork = 1; if (in[8] == 0) pwm.setPWM(3, 0, map(100, -180, 180, servoMin, servoMax)); tUp4 = 1; }
void handleshs4() { pwm.setPWM(3, 150, 150); shuttersWork = 0; }
void handleshl4() { shuttersWork = 1; if (in[6] == 0) pwm.setPWM(3, 0, map(-110, -180, 180, servoMin, servoMax)); tDown4 = 1; }

void handleshr5() { shuttersWork = 1; if (in[10] == 0) pwm.setPWM(4, 0, map(100, -180, 180, servoMin, servoMax)); tUp5 = 1; }
void handleshs5() { pwm.setPWM(4, 150, 150); shuttersWork = 0; }
void handleshl5() { shuttersWork = 1; if (in[9] == 0) pwm.setPWM(4, 0, map(-110, -180, 180, servoMin, servoMax)); tDown5 = 1; }
