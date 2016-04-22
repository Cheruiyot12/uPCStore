#include "core.h"

Core::Core(QObject *parent) : QObject(parent)
{
    con = new Connector();
    mw = new MainWindow();
    lw = new LoginWidget();


}

void Core::showInterface()
{
    connect(lw, SIGNAL(connectSig(QUrl)), con, SLOT(connect2server(QUrl)));
    connect(lw, SIGNAL(loginSig(QString,QString)), this, SLOT(logIn(QString,QString)));
    connect(con, SIGNAL(onSuccessLogin()), this, SLOT(onSucessLogin()));
    connect(con, SIGNAL(onErr(int)), this ,SLOT(errorHandler(int)));
    connect(con, SIGNAL(onNewGroup(QJsonObject)), this, SLOT(onNewGroup(QJsonObject)));
    connect(mw, SIGNAL(currentGroupChangedSig(int)), this, SLOT(onCurrGrChanged(int)));
    connect(con, SIGNAL(groupsToWidget()), this, SLOT(groupsToBox()));
    connect(con, SIGNAL(onNewItem(QJsonObject)), this ,SLOT(onNewItem(QJsonObject)));
    connect(con, SIGNAL(itemsToList()), this, SLOT(itemsToList()));
    connect(con, SIGNAL(clearGr()), this, SLOT(clearGrList()));
    connect(con, SIGNAL(clearItms()), this, SLOT(clearItmList()));
    connect(mw, SIGNAL(onItemClickedToAddSig(int)), this, SLOT(addItemToComp(int)));
    connect(mw, SIGNAL(onItemClickedToDelSig(int)), this, SLOT(delItemFromComp(int)));
    connect(con, SIGNAL(sendItemValuesToShow(QStringList*,QStringList*,QStringList*)),
            mw, SLOT(procInfoAboutItem(QStringList*,QStringList*,QStringList*)));
    connect(mw, SIGNAL(onShowInfoAboutItemSig(int)), this, SLOT(showInfo(int)));
    lw->show();
}

void Core::logIn(QString log, QString pass)
{
    QJsonObject uData;
    uData["command"] = loginCommand;
    uData["login"] = log;
    uData["password"] = pass;

    QJsonDocument uDoc(uData);

    con->sendTextMess(uDoc.toJson(QJsonDocument::Compact));
}

void Core::onSucessLogin()
{
    mw->show();
    lw->close();


    QJsonObject uData;
    uData["command"] = getItemGroups;

    QJsonDocument uDoc(uData);
    con->sendTextMess(uDoc.toJson(QJsonDocument::Compact));
}

void Core::errorHandler(int code)
{
    switch (code) {
    case wronglogpass:
        mw->showErr("Неверный логин или пароль");
        break;
    case nopermisssions:
        mw->showErr("У вас недостаточно прав на выполнение этого действия");
        break;
    default:
        break;
    }
}

void Core::onNewGroup(QJsonObject json)
{
    ItemGroup* newGroup = new ItemGroup();
    newGroup->fromJson(&json);
    groupList.append(newGroup);
    //groupsToBox();
}

void Core::groupsToBox()
{
    mw->clearGroups();
    for(int i = 0; i < groupList.size(); i++){
        mw->onNewGroup(groupList.at(i)->getName());
    }
}

void Core::onCurrGrChanged(int ind)
{
    QJsonObject obj;
    obj["command"] = getItemsFromGroup;
    obj["groupId"] = groupList.at(ind)->getId();

    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));

}

void Core::onNewItem(QJsonObject json)
{
    Item* newItem = new Item();

    newItem->fromJson(&json);
    items.append(newItem);
}

void Core::itemsToList()
{
    mw->clearItems();
    for(int i = 0; i < items.size(); i++){
        mw->onNewItem(items.at(i)->getName());
    }
}

void Core::clearGrList()
{
    groupList.clear();
}

void Core::clearItmList()
{
    items.clear();
}

void Core::addItemToComp(int id)
{
    Item* itm = items.at(id);
    comp.addHardware(itm);
    mw->onItemToComp(itm->getName(), itm->getId());
}

void Core::delItemFromComp(int id)
{
    //Item* itm = items.at(id);
    //Item* itm;
    comp.delHardware(id);
    mw->onItemDelComp(id);
}

void Core::showInfo(int id)
{
    QJsonObject obj;
    obj["command"] = getItemCharValues;
    obj["type"] = valuesToShow;
    obj["itemId"] = id;

    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}

