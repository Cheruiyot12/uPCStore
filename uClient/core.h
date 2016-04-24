#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <mainwindow.h>
#include <loginwidget.h>
#include <connector.h>
#include <itemgroup.h>
#include <item.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <computer.h>


class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

signals:

public slots:
    void showInterface();
    void logIn(QString log, QString pass);
    void onSucessLogin();
    void errorHandler(int code);

    void onNewGroup(QJsonObject json);
    void groupsToBox();
    void onCurrGrChanged(int ind);
    void clearGrList();

    void onNewItem(QJsonObject json);
    void itemsToList();
    void clearItmList();
    void addItemToComp(int id);
    void delItemFromComp(int id);

    void showInfo(int id);

    void placeOrder();
    void onOrderPlaced(int id);
    void onPlaceOrder();
private:
    MainWindow* mw;
    LoginWidget* lw;
    Connector* con;
    QList<ItemGroup*> groupList;
    QList<Item*> items;
    Computer comp;
    int orderNumber = 0;


};

#endif // CORE_H
