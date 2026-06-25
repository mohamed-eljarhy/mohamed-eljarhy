#include "TableHandler.h"

void TableHandler::handleTable(int tableNum) {
    switch(tableNum) {
        case 1: handletable1(); break;
        case 2: handletable2(); break;
        case 3: handletable3(); break;
        case 4: handletable4(); break;
        case 5: handlespresso(); break;
    }
    // دالة update، يمكن استدعاء تحديث أي شيء دوري هنا

}
 void TableHandler::update() {
    // مثال: إذا كنت تريد تحديث أي جداول بشكل دوري
    // حالياً مجرد دالة فارغة لتجنب الأخطاء
}
