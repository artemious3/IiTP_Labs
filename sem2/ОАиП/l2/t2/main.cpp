#include <QApplication>
#include <QPushButton>
#include "orderlistwindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    OrderListWindow w;
    w.show();
    return QApplication::exec();
}
