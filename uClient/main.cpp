//#include "mainwindow.h"
#include "core.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])

{

    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("platforms");
    paths.append("imageformats");
    QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);

    qApp->setStyle(QStyleFactory::create("fusion"));

    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53,53,53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(53,53,53));
    palette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53,53,53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);

    palette.setColor(QPalette::Highlight, QColor(142,45,197).darker(150));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    qApp->setPalette(palette);



    qApp->setStyleSheet("QLineEdit { selection-background-color: #B857EF }");


    Core *cor = new Core();
    cor->showInterface();

    return a.exec();
}
