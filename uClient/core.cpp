#include "core.h"

Core::Core(QObject *parent) : QObject(parent)
{
    con = new Connector();
    mw = new MainWindow();
    lw = new LoginWidget();
    ordw = new orderWidget();
    cw = new CharEditWidget();
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
    connect(mw, SIGNAL(onPlaceOrderSig()), this, SLOT(onPlaceOrder()));
    connect(con, SIGNAL(orderPlacedSig(int)), this, SLOT(onOrderPlaced(int)));
    connect(mw, SIGNAL(onRequestPermissionsSig()), this, SLOT(onGetPermissions()));
    connect(con, SIGNAL(obtaintedPermsSig(QString)), this, SLOT(onObtaintedPermissions(QString)));
    connect(mw, SIGNAL(selectItemToMod(int)), this, SLOT(onItemSelectedToMod(int)));
    connect(mw, SIGNAL(onLoadChrsReqSig()), this, SLOT(onCharsRequested()));
    connect(con, SIGNAL(sendItemValuesToEdit(QList<itemChars>*, QJsonArray *)), this, SLOT(loadChrsToCurr(QList<itemChars>*, QJsonArray *)));
    connect(con, SIGNAL(onCharNames(QList<chars>*)), this, SLOT(loadChNames(QList<chars>*)));
    connect(mw, SIGNAL(onSaveReqSig(openMode,int,QString,float,QList<itemChars>*)),
            this, SLOT(onSaveReq(openMode,int,QString,float,QList<itemChars>*)));
    connect(mw, SIGNAL(deleteSelectedItem(int)), this, SLOT(onDeleteItm(int)));
    connect(con, SIGNAL(succDelItem()), this, SLOT(onSuccDelIt()));
    connect(mw, SIGNAL(crePriceSig(QString)), this, SLOT(generatePrice(QString)));
    connect(mw, SIGNAL(openOrderMenuSig()), this, SLOT(openOrdMenu()));
    connect(con, SIGNAL(onOrderList(QJsonArray*)), this, SLOT(loadOrders(QJsonArray*)));
    connect(con, SIGNAL(enableLogin()), lw, SLOT(enableLogin()));
    connect(mw->ComBox, SIGNAL(clicked(bool)), this, SLOT(scp(bool)));
    connect(con, SIGNAL(GodOfCharsAppears(QJsonArray*)), cw, SLOT(loadChars(QJsonArray*)));
    connect(con, SIGNAL(LinksKing(QJsonArray*)), cw, SLOT(loadLinks(QJsonArray*)));
    connect(mw->chAct, SIGNAL(triggered(bool)), this, SLOT(opCw()));
    connect(cw, SIGNAL(adls(QString)), con, SLOT(sendTextMess(QString)));
    connect(cw, SIGNAL(dels(QString)), con, SLOT(sendTextMess(QString)));
    lw->show();
}


void Core::scp(bool bl)
{
    checkComp = bl;
}

void Core::opCw()
{
    cw->show();
    QJsonObject obb;
    obb["command"] = getCharLinks;
    QJsonDocument uDoc(obb);
    con->sendTextMess(uDoc.toJson(QJsonDocument::Compact));

    QJsonObject obb2;

    obb2["command"] = getCharsWithTypes;
    QJsonDocument uDo2(obb2);
    con->sendTextMess(uDo2.toJson(QJsonDocument::Compact));
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
    curGr = groupList.at(ind);

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

    if(comp.addHardware(itm, checkComp)){
        mw->onItemToComp(itm->getName(), itm->getId());
        mw->setPrice(comp.recountPrice());
    } else {
        QMessageBox::warning(0, "Ошибка", "Невозможно добавить данный товар к сборке по причине его несовметимости с остальными "
                                          "товарами. Для отключения данной проверки уберите галочку с пункта \"Совмеcтимость\" над списком товаров.");
    }
}

void Core::delItemFromComp(int id)
{
    //Item* itm = items.at(id);
    //Item* itm;
    comp.delHardware(id);
    mw->onItemDelComp(id);
    mw->setPrice(comp.recountPrice());
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

void Core::placeOrder()
{
    QJsonObject obj;
    obj["command"] = placeOrd;

    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}

void Core::onOrderPlaced(int id)
{
    orderNumber = id;
    QJsonObject obj;
    obj["command"] = addItemsToOrd;
    obj["orderId"] = orderNumber;
    obj["itemArr"] = comp.toJson();
    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}

void Core::onPlaceOrder()
{
    if(!comp.isEmpty()){
        if(orderNumber == 0){
            placeOrder();
        } else {
            mw->showErr("В данной версии программы нельзя изменить отправленный заказ");
            //TODO
        }
    }else{
        mw->showErr("Нельзя отправить пустой заказ!");
    }
}

void Core::onGetPermissions()
{
    QJsonObject obj;
    obj["command"] = getPermissions;
    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}

void Core::onObtaintedPermissions(QString st)
{
    if(permId != permissions::admin && permId != permissions::seller){
        if(st == "admin"){
            permId = permissions::admin;
            mw->activateSellerMode();
            mw->activateAdminMode();
            uw = new UserWidget();
            connect(con, SIGNAL(onUserList(QJsonArray*)), this, SLOT(onUserList(QJsonArray*)));
            connect(con, SIGNAL(onSuccUserMod()), this, SLOT(onReqUserList()));
            connect(mw, SIGNAL(openUserEditSig()), this, SLOT(openUserW()));
            connect(uw, SIGNAL(addUser(QString)), con, SLOT(sendTextMess(QString)));
            connect(uw, SIGNAL(modUser(QString)), con, SLOT(sendTextMess(QString)));
            connect(uw, SIGNAL(delUser(QString)), con, SLOT(sendTextMess(QString)));
            connect(uw, SIGNAL(requestUsers()), this, SLOT(onReqUserList()));
        } else if (st == "seller"){
            permId = permissions::seller;
            mw->activateSellerMode();
        } else {
            QMessageBox::warning(0,"Ошибка", "У вас нет прав для администрирования системы!");
        }
    }
}

void Core::onItemSelectedToMod(int num)
{
    itemSelectedToModNum = num;
    if(num!=-1){

        Item* itm = items.at(num);
        mw->showEditWidget(itm->getId(), itm->getName(), itm->getPrice(), 1, openMode::editing);

    } else {
        mw->showEditWidget(-1, "", 0.0, 1, openMode::adding);
    }
}

void Core::onCharsRequested()
{
    QJsonObject ob2;
    ob2["command"] = getCharNames;
    ob2["typeId"] = curGr->getId();

    QJsonDocument doc2(ob2);
    con->sendTextMess(doc2.toJson(QJsonDocument::Compact));

    if(itemSelectedToModNum!=-1){
        QJsonObject obj;
        obj["command"] = getItemCharValues;
        obj["type"] = valuesToEdit;
        obj["itemId"] = items.at(itemSelectedToModNum)->getId();

        QJsonDocument doc(obj);
        con->sendTextMess(doc.toJson(QJsonDocument::Compact));
    }


}

void Core::loadChrsToCurr(QList<itemChars> *ql,QJsonArray *arr)
{
    qDebug() << "lchtc";
    items.at(itemSelectedToModNum)->addChars(arr);
    mw->loadChrsToCurrWid(ql);
}

void Core::loadChNames(QList<chars> *ql)
{
    charsList = ql;
    mw->loadChNamsToCurrWid(charsList);

}

void Core::onSaveReq(openMode sMode, int nnid, QString nnme, float nprce, QList<itemChars> *nchrs)
{
    QJsonObject obj;
    QJsonArray chrs;
    obj["itemPrice"] = nprce;
    obj["itemName"] = nnme;

    for(int i = 0; i < nchrs->size(); i++){
        QJsonObject ch;
        int jjj = 0;
        for(int j = 0; j < charsList->size(); j++){
            if(nchrs->at(i).charname == charsList->at(j).charname){
                jjj = charsList->at(j).charId;
                break;
            }
        }
        ch["charId"] = jjj;
        ch["charValue"] = nchrs->at(i).charValue;
        ch["charUnits"] = nchrs->at(i).charUnits;
        chrs.append(ch);
    }
    if(sMode == openMode::editing){
        obj["command"] = editItem;
        obj["itemId"] = items.at(itemSelectedToModNum)->getId();
    } else {
        obj["command"] = addItem;
        obj["typeId"] = curGr->getId();
    }

    QJsonDocument ard(chrs);
    QString ss = ard.toJson(QJsonDocument::Compact);
    obj["chars"] = ss;
    QJsonDocument ob(obj);
    con->sendTextMess(ob.toJson(QJsonDocument::Compact));
}

void Core::onDeleteItm(int ind)
{
    QJsonObject obj;
    obj["command"] = delItem;
    obj["itemId"] = items.at(ind)->getId();
    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}

void Core::onSuccDelIt()
{
    onCurrGrChanged(groupList.indexOf(curGr));
}

void Core::onReqUserList()
{
    QJsonObject obj;
    obj["command"] = reqUserList;
    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}

void Core::onUserList(QJsonArray *arr)
{
    uw->loadUsers(arr);
}

void Core::openUserW()
{
    uw->show();
    onReqUserList();
}

void Core::generatePrice(QString path)
{
    QXlsx::Document xlsx;

    xlsx.write("A1", "Название");
    xlsx.write("B1", "Цена");

    int i = 0;
    for(i = 0; i<comp.hardware.size(); i++){
        xlsx.write(QString("A%1").arg(i+3), comp.hardware.at(i)->getName());
        xlsx.write(QString("B%1").arg(i+3), comp.hardware.at(i)->getPrice());
    }

    xlsx.write(QString("A%1").arg(i+4), "Итого: ");
    xlsx.write(QString("B%1").arg(i+4), comp.recountPrice());

    xlsx.saveAs(path+"\\CompSave.xlsx");
}

void Core::loadOrders(QJsonArray *arr)
{
    ordw->loadOrderNumbers(arr);
}

void Core::openOrdMenu()
{
    QJsonObject obj;
    obj["command"] = reqOrderList;
    QJsonDocument doc(obj);
    con->sendTextMess(doc.toJson(QJsonDocument::Compact));
}
