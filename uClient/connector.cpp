#include "connector.h"

Connector::Connector(QObject *parent) : QObject(parent)
{
    connect(&cliSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&cliSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&cliSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMsg(QString)));

    //ERR_load_crypto_strings();
    //OpenSSL_add_all_algorithms();
    //OPENSSL_config(NULL);
}

void Connector::onConnected()
{
    qDebug() << "connected";
    connected = true;

    QJsonObject uData;
    uData["type"] = messType::connectMes;
    QJsonDocument uDoc(uData);

    //sendTextMess(uDoc.toJson(QJsonDocument::Compact));
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
    QJsonDocument doc;
    QJsonDocument firDoc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject firObj = firDoc.object();

    /*if(firObj["type"].toInt() == messType::rsaOpenkey){
        pubKey pkey;
        strcpy(pkey.n_b, (char*)firObj["n_b"].toVariant().data());
        strcpy(pkey.e_b, (char*)firObj["e_b"].toVariant().data());
        pkey.n_size = firObj["n_size"].toInt();
        pkey.b_size = firObj["b_size"].toInt();
        enc_rsa = Cryptor::constructEncrypt(pkey);
        aesKey = Cryptor::genAesKey(256);

        qDebug() << " aes key:"<< aesKey.toBase64();

        QByteArray encrKe = Cryptor::encr(aesKey, enc_rsa);

        QJsonObject uData;
        uData["type"] = messType::aesKey;
        uData["key"] = QString::fromLatin1(aesKey.toBase64());
        QJsonDocument uDoc(uData);

        sendTextMess(uDoc.toJson(QJsonDocument::Compact));
        //useAes = true;


    } else if(firObj["type"].toInt() == messType::aesKey){

    } else {
        /*doc = QJsonDocument::fromJson(Cryptor::decryptAes(QByteArray::fromBase64(firObj["data"].toString().toLatin1()),
                                      aesKey, QByteArray::fromBase64(firObj["iv"].toString().toLatin1())).toUtf8());*/
        doc = QJsonDocument::fromJson(msg.toUtf8());
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
        case successDelItem:
        {
            emit this->succDelItem();
            break;
        }
        case reqUserList:
        {
            QJsonDocument usDoc;
            QJsonArray userArr;
            usDoc = QJsonDocument::fromJson(obj["users"].toString().toUtf8());
            userArr = usDoc.array();
            emit this->onUserList(&userArr);
            break;
        }
        case succUserMod:
        {
            emit this->onSuccUserMod();
            break;
        }
        case reqOrderList:
        {
            QJsonDocument ordDoc;
            QJsonArray ordArr;
            ordDoc = QJsonDocument::fromJson(obj["orders"].toString().toUtf8());
            ordArr = ordDoc.array();
            emit this->onOrderList(&ordArr);
        }
        default:
            break;
        }
    }
//}

void Connector::onBinMsg(QByteArray msg)
{

}

void Connector::sendTextMess(QString msg)
{
    if(useAes){
        QJsonObject obj;
        //QByteArray iv = Cryptor::genAesKey(128);
        //["type"] = messType::data;
        //obj["data"] = QString::fromLatin1(Cryptor::encryptAes(msg, aesKey, iv).toBase64());
        //obj["iv"] = QString::fromLatin1(iv.toBase64());
        QJsonDocument doc(obj);
        cliSocket.sendTextMessage(doc.toJson(QJsonDocument::Compact));
    } else
        cliSocket.sendTextMessage(msg);
}
