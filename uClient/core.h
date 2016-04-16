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
private:
    MainWindow* mw;
    LoginWidget* lw;
    Connector* con;
    QList<ItemGroup*> groupList;
    QList<Item*> items;

};

#endif // CORE_H
