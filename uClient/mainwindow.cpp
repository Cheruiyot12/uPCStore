#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* mainWid = new QWidget();
    mainWid->setMinimumSize(640, 480);
    this->setCentralWidget(mainWid);
    QGridLayout* mainLay = new QGridLayout();
    mainWid->setLayout(mainLay);

    QGridLayout* listLay = new QGridLayout();
    mainLay->addLayout(listLay, 0, 0);
    listLay->addWidget(new QLabel("Выберите из списка категорию товара, которую требуется добавить к сборке:"), 0, 0);

    catBox = new QComboBox();
    listLay->addWidget(catBox, 1, 0);

    mainList = new QListWidget();
    listLay->addWidget(mainList, 2, 0);


    QVBoxLayout* finLay = new QVBoxLayout();
    mainLay->addLayout(finLay,0,1);
    QGroupBox* finBox = new QGroupBox("Оценка");
    finLay->addWidget(finBox);
    QGroupBox* confBox = new QGroupBox("Конфигурация");
    finLay->addWidget(confBox);
}

MainWindow::~MainWindow()
{

}

void MainWindow::showErr(QString text)
{
    QMessageBox::warning(this, "eRRoR", text);
}

void MainWindow::onNewGroup(QString name)
{
    catBox->addItem(name);
}

void MainWindow::clearGroups()
{
    catBox->clear();
}
