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
#include <userwidget.h>
#include <xlsxdocument.h>
//#include "cryptor.h"
#include <qacrypt.h>
#include <orderwidget.h>
#include <chareditwidget.h>
#include <QAction>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);
    bool checkComp = true;

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

    void scp(bool bl);


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
    void onReqUserList();
    void onUserList(QJsonArray* arr);
    void openUserW();

    void generatePrice(QString path);

    void loadOrders(QJsonArray* arr);
    void openOrdMenu();

    void opCw();

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
    UserWidget* uw;
    orderWidget *ordw;
    CharEditWidget *cw;



};

#endif // CORE_H
