#include "connector.h"

Connector::Connector(QObject *parent) : QObject(parent)
{
    connect(&cliSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&cliSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&cliSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMsg(QString)));
}

void Connector::onConnected()
{
    qDebug() << "connected";
    connected = true;
}

void Connector::onDisconnected()
{
    qDebug() << "disconnected";
    connected = false;
}

void Connector::connect2server(QUrl url)
{

    qDebug() << "try to connect " << url.toString();

    cliSocket.open(url);
}

void Connector::onTextMsg(QString msg)
{
    //qDebug() << msg;
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject obj = doc.object();

    switch (obj["command"].toInt()) {
    case successLogin:
    {
        emit this->onSuccessLogin();
        break;
    }
    case error:
    {
        emit this->onErr(obj["eRRoRcode"].toInt());
        break;
    }
    case getItemGroups:
    {
        emit this->clearGr();
        QJsonDocument grDoc;
        grDoc = QJsonDocument::fromJson(obj["groupArr"].toString().toUtf8());
        QJsonArray grArr = grDoc.array();
        for (int i = 0; i < grArr.size(); i++){
            emit this->onNewGroup(grArr.at(i).toObject());
        }
        emit this->groupsToWidget();
        break;
    }
    case getItemsFromGroup:
    {
        emit this->clearItms();
        QJsonDocument itmDoc;
        itmDoc = QJsonDocument::fromJson(obj["itmsArr"].toString().toUtf8());
        QJsonArray itmArr = itmDoc.array();
        for (int i = 0; i < itmArr.size(); i++){
            emit this->onNewItem(itmArr.at(i).toObject());
        }
        emit this->itemsToList();
        break;
    }
    case getItemCharValues:
    {
        if(obj["type"].toInt() == valuesToShow){
            QStringList s1, s2, s3;
            QJsonObject valObj;
            //emit this->clearItms();
            QJsonDocument itmDoc;
            itmDoc = QJsonDocument::fromJson(obj["itmsArr"].toString().toUtf8());
            QJsonArray itmArr = itmDoc.array();
            for (int i = 0; i < itmArr.count(); i++){
                valObj = itmArr.at(i).toObject();
                s1.append(valObj["charName"].toString());
                s2.append(valObj["charValue"].toString());
                s3.append(valObj["charUnits"].toString());
            }
            emit this->sendItemValuesToShow(&s1,&s2,&s3);
        }else{
            //TODO
            //qDebug() << msg;
            QList<itemChars> ql;

            QJsonObject valObj;
            //emit this->clearItms();
            QJsonDocument itmDoc;
            itmDoc = QJsonDocument::fromJson(obj["itmsArr"].toString().toUtf8());
            QJsonArray itmArr = itmDoc.array();
            for (int i = 0; i < itmArr.count(); i++){
                itemChars chr;
                valObj = itmArr.at(i).toObject();
                chr.charname = valObj["charName"].toString();
                chr.charValue = valObj["charValue"].toString();
                chr.charUnits = valObj["charUnits"].toString();
                chr.charId = valObj["charId"].toInt();
                ql.append(chr);
            }
            emit this->sendItemValuesToEdit(&ql, &itmArr);
        }
        break;
    }
    case placeOrd:
    {
        emit this->orderPlacedSig(obj["orderId"].toInt());
        break;
    }
    case getPermissions:
    {
        qDebug() << "obtained permissions " << obj["permissions"].toString();
        emit this->obtaintedPermsSig(obj["permissions"].toString());
        break;
    }
    case getCharNames:
    {
        QList<chars> *ql = new QList<chars>();

        QJsonObject valObj;
        QJsonDocument itmDoc;
        itmDoc = QJsonDocument::fromJson(obj["chars"].toString().toUtf8());
        QJsonArray itmArr = itmDoc.array();
        for (int i = 0; i < itmArr.count(); i++){
            chars chr;
            valObj = itmArr.at(i).toObject();
            chr.charname = valObj["charName"].toString();
            chr.charId = valObj["charId"].toInt();
            ql->append(chr);
        }
        emit this->onCharNames(ql);
        break;
    }
    default:
        break;
    }
}

void Connector::onBinMsg(QByteArray msg)
{

}

void Connector::sendTextMess(QString msg)
{
    cliSocket.sendTextMessage(msg);
}
