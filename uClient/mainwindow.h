#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QComboBox>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void showErr(QString text);
    void onNewGroup(QString name);
    void clearGroups();
private:
    QComboBox* catBox;
    QListWidget* mainList;
};

#endif // MAINWINDOW_H
