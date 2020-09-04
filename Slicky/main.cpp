#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    qRegisterMetaType<Status>("Status");
    qRegisterMetaTypeStreamOperators<Status>("Status");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
