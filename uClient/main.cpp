//#include "mainwindow.h"
#include "core.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core *cor = new Core();
    cor->showInterface();

    return a.exec();
}
