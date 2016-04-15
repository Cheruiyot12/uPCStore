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
    default:
        break;
    }
}

void Core::onNewGroup(QJsonObject json)
{
    ItemGroup* newGroup = new ItemGroup();
    newGroup->fromJson(&json);
    groupList.append(newGroup);
    groupsToBox();
}

void Core::groupsToBox()
{
    mw->clearGroups();
    for(int i = 0; i < groupList.size(); i++){
        mw->onNewGroup(groupList.at(i)->getName());
    }
}
