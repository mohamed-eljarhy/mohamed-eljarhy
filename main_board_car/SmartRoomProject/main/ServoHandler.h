#ifndef SERVOHANDLER_H
#define SERVOHANDLER_H

#include "Utils.h"

class ServoHandler {
public:
    void openShutter(int shutterNum);
    void closeShutter(int shutterNum);
    void stopShutter(int shutterNum);

    void update();  // دالة جديدة لتحديث حالة السيرفو
};

#endif
