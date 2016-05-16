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

    void onGetPermissions();
    void onObtaintedPermissions(QString st);

    void onItemSelectedToMod(int num);
    void onCharsRequested();
    void loadChrsToCurr(QList<itemChars> *ql, QJsonArray *arr);
    void loadChNames(QList<chars> *ql);

    void onSaveReq(openMode sMode, int nnid, QString nnme, float nprce, QList<itemChars>* nchrs);
    void onDeleteItm(int ind);

    void onSuccDelIt();
private:
    MainWindow* mw;
    LoginWidget* lw;
    Connector* con;
    QList<ItemGroup*> groupList;
    QList<chars> *charsList;
    ItemGroup* curGr;
    QList<Item*> items;
    Computer comp;
    int orderNumber = 0;
    permissions permId = permissions::user;
    int itemSelectedToModNum = -1;


};

#endif // CORE_H
