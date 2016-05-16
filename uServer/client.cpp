#include "client.h"

Client::Client(QWebSocket* sock, QObject *parent) : QObject(parent), mainSocket(sock), dataBase(new ClientDatabase())
{
    connect(mainSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMessage(QString)));
    connect(mainSocket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    //rootBase = new ClientDatabase();
    //rootBase->auth("postgres","alkor"); //TODO
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
                login = obj["login"].toString();
            }else{
                qDebug() << obj[login].toString() <<"cant auth";
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
        result res = dataBase->getGroups();
        if(!res.isError){
            QJsonObject grps;
            grps["command"] = getItemGroups;
            grps["groupArr"] = res.resStr;
            QJsonDocument gr2send(grps);


            this->sendTextMes(gr2send.toJson(QJsonDocument::Compact));
        } else {
            this->sendTextMes(handleError(res.errorCode));
        }
        break;
    }
    case getItemsFromGroup:
    {
        result res = dataBase->getItemsFromGroup(obj["groupId"].toInt());
        if(!res.isError){
            QJsonObject itms;
            itms["command"] = getItemsFromGroup;
            itms["itmsArr"]= res.resStr;
            QJsonDocument itm2send(itms);

            this->sendTextMes(itm2send.toJson(QJsonDocument::Compact));
        } else {
            this->sendTextMes(handleError(res.errorCode));
        }
        break;
    }
    case getItemCharValues:
    {
        result res = dataBase->getCharsOfItem(obj["itemId"].toInt());
        if(!res.isError){
            QJsonObject itms;
            itms["command"] = getItemCharValues;
            itms["type"] = obj["type"];
            itms["itmsArr"]= res.resStr;
            QJsonDocument itm2send(itms);

            this->sendTextMes(itm2send.toJson(QJsonDocument::Compact));
        } else {
            this->sendTextMes(handleError(res.errorCode));
        }
        break;
    }
    case placeOrd:
    {
        result res = dataBase->placeOrder();
        if(!res.isError){
            QJsonObject itms;
            itms["command"] = placeOrd;
            itms["orderId"] = res.resStr.toInt();
            QJsonDocument itm2send(itms);

            this->sendTextMes(itm2send.toJson(QJsonDocument::Compact));
        } else {
            this->sendTextMes(handleError(res.errorCode));
        }
        break;
    }
    case addItemsToOrd:
    {
        QJsonDocument itmDoc;
        result rest;
        itmDoc = QJsonDocument::fromJson(obj["itemArr"].toString().toUtf8());
        QJsonArray itmArr = itmDoc.array();
        for (int i = 0; i < itmArr.size(); i++){
           rest = dataBase->addItemToOrder(obj["orderId"].toInt(),
                                                  itmArr.at(i).toObject().value("itemId").toInt(),
                                                  1);
           if(rest.isError){
               this->sendTextMes(handleError(rest.errorCode));
           }
        }
        break;
    }
    case getPermissions:
    {
        result rest;
        rest = dataBase->getPermissions(login);
        if(!rest.isError){
            QJsonObject objct;
            objct["command"] = getPermissions;
            objct["permissions"] = rest.resStr;
            QJsonDocument doct(objct);
            this->sendTextMes(doct.toJson(QJsonDocument::Compact));
        }else{
            this->sendTextMes(handleError(rest.errorCode));
        }
        break;
    }
    case getCharNames:
    {
        result rest;
        rest = dataBase->getCharNames(obj["typeId"].toInt());
        if(!rest.isError){
            QJsonObject objct;
            objct["command"] = getCharNames;
            objct["chars"] = rest.resStr;
            QJsonDocument doct(objct);
            this->sendTextMes(doct.toJson(QJsonDocument::Compact));
        }else{
            this->sendTextMes(handleError(rest.errorCode));
        }
        break;
    }
    case editItem:
    {
        QList<itemChars> *ql = new QList<itemChars>;
        QJsonDocument itmDoc = QJsonDocument::fromJson(obj["chars"].toString().toUtf8());
        QJsonArray arr = itmDoc.array();
        for(int i = 0; i < arr.count(); i++){
            itemChars it;
            it.charId = arr.at(i).toObject().value("charId").toInt();
            it.charValue = arr.at(i).toObject().value("charValue").toString();
            it.charUnits = arr.at(i).toObject().value("charUnits").toString();
            ql->append(it);
        }
        dataBase->editItem(obj["itemId"].toInt(), obj["itemName"].toString(), obj["itemPrice"].toDouble(), ql);
        break;
    }
    case addItem:
    {
        QList<itemChars> *ql = new QList<itemChars>;
        QJsonDocument itmDoc = QJsonDocument::fromJson(obj["chars"].toString().toUtf8());
        QJsonArray arr = itmDoc.array();
        for(int i = 0; i < arr.count(); i++){
            itemChars it;
            it.charId = arr.at(i).toObject().value("charId").toInt();
            it.charValue = arr.at(i).toObject().value("charValue").toString();
            it.charUnits = arr.at(i).toObject().value("charUnits").toString();
            ql->append(it);
        }
        dataBase->addItem(obj["typeId"].toInt(), obj["itemName"].toString(), obj["itemPrice"].toDouble(), ql);
        break;
    }
    default:
        break;
    }
}


QByteArray Client::handleError(int errCode)
{
    QJsonObject uData;
    uData["command"] = error;
    switch (errCode) {
    case 42501:
    {
        uData["eRRoRcode"] = nopermisssions;
        break;
    }
    default:
    {
        uData["eRRoRcode"] = undefinederror;
        break;
    }
    }
    QJsonDocument uDoc(uData);
    return uDoc.toJson(QJsonDocument::Compact);
}

void Client::onDisconnect()
{
    emit this->cliDisc(this);
}

void Client::sendTextMes(QString msg)
{
    mainSocket->sendTextMessage(msg);
}
