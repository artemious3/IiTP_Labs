#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    L2::MainWindow w;
    w.show();
    return a.exec();
}