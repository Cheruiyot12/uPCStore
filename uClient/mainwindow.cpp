#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
}

MainWindow::~MainWindow()
{

}

void MainWindow::showErr(QString text)
{
    QMessageBox::warning(this, "eRRoR", text);
}
