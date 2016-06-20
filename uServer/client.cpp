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
    QJsonDocument doc;
    QJsonDocument firDoc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject firObj = firDoc.object();

    if(firObj["type"].toInt() == messType::connectMes){
        int codes;
        myPrivKey = DH_new();
        DH_generate_parameters_ex(myPrivKey, 512, DH_GENERATOR_5, NULL);

        DH_check(myPrivKey, &codes);
        if(codes!=0){
            qDebug() << "DH check failed";
            abort();
        }
        DH_generate_key(myPrivKey);

        QJsonObject objctt;
        objctt["type"] = messType::rsaOpenkey;
        objctt["p"] = QString(QACrypt::bn2binArr(myPrivKey->p).toBase64());
        objctt["g"] = QString(QACrypt::bn2binArr(myPrivKey->g).toBase64());
        objctt["pubKey"] = QString(QACrypt::bn2binArr(myPrivKey->pub_key).toBase64());
        QJsonDocument doct(objctt);
        this->sendTextMes(doct.toJson(QJsonDocument::Compact));


    } else if(firObj["type"].toInt() == messType::rsaOpenkey){
        BIGNUM *pubKey = BN_new();

        QACrypt::binArr2bn(QByteArray::fromBase64(firObj["pubKey"].toString().toLatin1()), &pubKey);
        qDebug() << "DH pubkey recieved!";

        unsigned char* sharedSecret = new unsigned char[1024];
        int keyLen = DH_compute_key(sharedSecret, pubKey, myPrivKey);
        if(keyLen == -1){
            ERR_print_errors_fp(stderr);
        }

        QByteArray keyArr = QByteArray::fromRawData((char*)sharedSecret, keyLen);

        //qDebug() << "sharedSecret: " << keyArr.toBase64();
        aesKey = keyArr;

        useAes = true;

    } else {
        if(useAes){
        doc = QJsonDocument::fromJson(QACrypt::decrypt2(QByteArray::fromBase64(firObj["data"].toString().toLatin1()),
                                      aesKey, QByteArray::fromBase64(firObj["iv"].toString().toLatin1())));
        } else {
            doc = QJsonDocument::fromJson(msg.toUtf8());
        }


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
            result res = dataBase->addItem(obj["typeId"].toInt(), obj["itemName"].toString(), obj["itemPrice"].toDouble(), ql);

            if(!res.isError){
                QJsonObject objcc;
                objcc["command"] = successDelItem;
                QJsonDocument doct(objcc);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case delItem:
        {
            result res = dataBase->deleteItem(obj["itemId"].toInt());
            if(!res.isError){
                QJsonObject objcc;
                objcc["command"] = successDelItem;
                QJsonDocument doct(objcc);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case reqUserList:
        {
            result res = dataBase->getUserList();
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = reqUserList;
                objct["users"] = res.resStr;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case addUserC:
        {
            result res = dataBase->createUser(obj["userLogin"].toString(),
                    obj["userPassword"].toString(),
                    obj["userEmail"].toString(),
                    obj["userPermissions"].toString());
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = succUserMod;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case delUserc:
        {
            result res = dataBase->deleteUser(obj["userId"].toInt());
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = succUserMod;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case modUserC:
        {
            result res = dataBase->modUser(obj["userId"].toInt(),
                    obj["userEmail"].toString(),
                    obj["modPass"].toBool(),
                    obj["userPermissions"].toString(),
                    obj["userPassword"].toString());
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = succUserMod;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case reqOrderList:
        {
            result res = dataBase->getInfoAboutOrders();
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = reqOrderList;
                objct["orders"] = res.resStr;
                qDebug() << res.resStr;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case getCharLinks:
        {
            result res = dataBase->getLinkedChars();
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = getCharLinks;
                objct["list"] = res.resStr;
                //qDebug() << res.resStr;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case getCharsWithTypes:
        {
            result res = dataBase->getCharsWithTypes();
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = getCharsWithTypes;
                objct["list"] = res.resStr;
                qDebug() << res.resStr;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case dellLink:
        {
            result res = dataBase->delLin(obj["charId1"].toInt(), obj["charId2"].toInt());
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = succLink;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        case addLink:
        {
            result res = dataBase->adLin(obj["charId1"].toInt(), obj["charId2"].toInt());
            if(!res.isError){
                QJsonObject objct;
                objct["command"] = succLink;
                QJsonDocument doct(objct);
                this->sendTextMes(doct.toJson(QJsonDocument::Compact));
            } else {
                this->sendTextMes(handleError(res.errorCode));
            }
            break;
        }
        default:
            break;
        }
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
    dataBase->mainDB.close();
    emit this->cliDisc(this);
}

void Client::sendTextMes(QString msg)
{
    if(useAes){
        QJsonObject obj;
        QByteArray iv = QACrypt::genAesKey(128);
        obj["type"] = messType::data;
        obj["data"] = QString::fromLatin1(QACrypt::encrypt2(msg.toUtf8(), aesKey, iv).toBase64());
        obj["iv"] = QString::fromLatin1(iv.toBase64());
        QJsonDocument doc(obj);
        mainSocket->sendTextMessage(doc.toJson(QJsonDocument::Compact));
    } else
        mainSocket->sendTextMessage(msg);
}
