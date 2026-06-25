#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

// External variables


extern bool shuttersWork;
extern Adafruit_PWMServoDriver pwm;

// Functions
void inputBus();
void writeOutput();

// Tables
void handletable1();
void handletable2();
void handletable3();
void handletable4();
void handlespresso();

// Servo shutters
void handleshr1();
void handleshs1();
void handleshl1();
void handleshr2();
void handleshs2();
void handleshl2();
void handleshr3();
void handleshs3();
void handleshl3();
void handleshr4();
void handleshs4();
void handleshl4();
void handleshr5();
void handleshs5();
void handleshl5();

#endif
