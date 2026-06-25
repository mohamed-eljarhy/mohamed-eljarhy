#include "ServoHandler.h"

void ServoHandler::openShutter(int shutterNum) {
    switch(shutterNum) {
        case 1: handleshr1(); break;
        case 2: handleshr2(); break;
        case 3: handleshr3(); break;
        case 4: handleshr4(); break;
        case 5: handleshr5(); break;
    }
}

void ServoHandler::closeShutter(int shutterNum) {
    switch(shutterNum) {
        case 1: handleshl1(); break;
        case 2: handleshl2(); break;
        case 3: handleshl3(); break;
        case 4: handleshl4(); break;
        case 5: handleshl5(); break;
    }
}

void ServoHandler::stopShutter(int shutterNum) {
    switch(shutterNum) {
        case 1: handleshs1(); break;
        case 2: handleshs2(); break;
        case 3: handleshs3(); break;
        case 4: handleshs4(); break;
        case 5: handleshs5(); break;
    }
}

// دالة update الجديدة
void ServoHandler::update() {
    // يمكن إضافة أي تحديث دوري هنا
    // على سبيل المثال إذا أردنا التأكد من توقف الشتر بعد حركة معينة
    // حالياً سنتركها فارغة إذا لم يكن هناك حاجة لتحديث مستمر
}
