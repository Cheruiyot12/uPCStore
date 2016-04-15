#include "client.h"

Client::Client(QWebSocket* sock, QObject *parent) : QObject(parent), mainSocket(sock), dataBase(new ClientDatabase())
{
   connect(mainSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessage(QString)));
   connect(mainSocket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
}

void Client::onTextMessage(QString msg)
{
    if (isDebug)
        qDebug() << msg;

    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject obj = doc.object();

    switch (obj["command"].toInt()) {
    case loginCommand:
    {
        if (dataBase->loginToDb(obj["login"].toString(), obj["password"].toString())){
            if(isDebug)
                qDebug() << obj["login"].toString() << "logged to DB";
            if(dataBase->auth(obj["login"].toString(), obj["password"].toString())){
                QJsonObject uData;
                uData["command"] = successLogin;
                QJsonDocument uDoc(uData);
                this->sendTextMes(uDoc.toJson(QJsonDocument::Compact));
            }else{
                qDebug() << "cant auth";
            }

        } else {
            QJsonObject uData;
            uData["command"] = error;
            uData["eRRoRcode"] = wronglogpass;
            QJsonDocument uDoc(uData);
            this->sendTextMes(uDoc.toJson(QJsonDocument::Compact));
        }
        break;
    }
    case getItemGroups:
    {
        QString grpStr = dataBase->getGroups();
        QJsonObject grps;
        grps["command"] = getItemGroups;
        grps["groupArr"] = grpStr;
        QJsonDocument gr2send(grps);

        this->sendTextMes(gr2send.toJson(QJsonDocument::Compact));
    }
    default:
        break;
    }
}

void Client::onDisconnect()
{
    emit this->cliDisc(this);
}

void Client::sendTextMes(QString msg)
{
    mainSocket->sendTextMessage(msg);
}
