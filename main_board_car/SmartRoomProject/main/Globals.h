#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

// Pins
extern int dirPin;
extern int stepPin;
extern int ena4;
extern int ena5;

// Servo limits
extern int servoMin;
extern int servoMax;

// Timing flags
extern int tUp1, tDown1, tUp2, tDown2, tUp3, tDown3, tUp4, tDown4, tUp5, tDown5;

#endif
