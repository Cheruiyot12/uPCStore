#include "clientdatabase.h"

ClientDatabase::ClientDatabase(QObject *parent) : QObject(parent)
{
    mainDB = QSqlDatabase::addDatabase("QPSQL");
    mainDB.setHostName("localhost");
    mainDB.setPort(5432);
    mainDB.setDatabaseName("uPCStore");
}

bool ClientDatabase::loginToDb(QString log, QString pass)
{
    mainDB.setUserName(log);
    mainDB.setPassword(pass);

    return mainDB.open();
}

bool ClientDatabase::auth(QString log, QString pass)
{
    QSqlQuery query;
    query.prepare(QString("SELECT authorize('%1', encode(digest('%2', 'sha256'), 'hex'))").arg(log).arg(pass));
    if(query.exec()){
        query.last();
        qDebug() << query.value(0) << log;
        return query.value(0).toBool();
    }else{
        qDebug() << "failed on auth";
        qDebug() << query.lastError().databaseText();
    }
}

result ClientDatabase::getGroups()
{
    result res;
    QSqlQuery query;
    QJsonArray grArr;
    query.prepare("SELECT * FROM item_types");
    if(query.exec()){
        while (query.next()){
            QJsonObject grp;
            grp["groupId"] = query.value(0).toInt();
            grp["groupName"] = query.value(1).toString();
            grArr.append(grp);
        }
        QJsonDocument doc(grArr);
        res.resStr = doc.toJson(QJsonDocument::Compact);
    } else {
        qDebug() << "failed on getGroups";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::getItemsFromGroup(int grId)
{
    result res;
    QSqlQuery query;
    QJsonArray itemArr;
    query.prepare(QString("SELECT id_item,  name_item, price_item_usd FROM items WHERE type_id_item = %1").arg(grId));
    if(query.exec()){
        while (query.next()){
            QJsonObject grp;
            grp["itemId"] = query.value(0).toInt();
            grp["itemName"] = query.value(1).toString();
            grp["itemPrice"] = query.value(2).toDouble();
            //qDebug() << query.value(2) << "--------------------------";
            itemArr.append(grp);
        }
        QJsonDocument doc(itemArr);
        res.resStr = doc.toJson(QJsonDocument::Compact);
    } else {
        qDebug() << "failed on getItems";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::getCharsOfItem(int itemId)
{
    result res;
    QSqlQuery query;
    QJsonArray itemArr;
    query.prepare(QString("SELECT * FROM text_chars WHERE id_item = %1;").arg(itemId));
    if(query.exec()){
        while (query.next()){
            QJsonObject grp;
            grp["itemId"] = query.value(0).toInt();
            grp["charId"] = query.value(1).toInt();
            grp["charName"] = query.value(2).toString();
            grp["charValue"] = query.value(3).toString();
            grp["charUnits"] = query.value(4).toString();

            itemArr.append(grp);
        }
        QJsonDocument doc(itemArr);
        res.resStr = doc.toJson(QJsonDocument::Compact);
    } else {
        qDebug() << "failed on getCharValues";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::placeOrder()
{
    result res;
    QSqlQuery query;
    query.prepare(QString("INSERT INTO orders(id_customer, order_date, paid) "
                          "VALUES ((SELECT id_user FROM users WHERE users.login_user = quote_ident(USER)), now(), 'false') "
                          "RETURNING id_order;"));
    if (query.exec()){
        query.first();
        res.resStr = QString("%1").arg(query.value(0).toInt());
    } else {
        qDebug() << "failed on placeOrder";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::addItemToOrder(int orderId, int itemId, int itemCount)
{
    result res;
    QSqlQuery query;
    query.prepare(QString("INSERT INTO orders_items VALUES('%1', '%2', '%3')").arg(orderId).arg(itemId).arg(itemCount));
    if (query.exec()){
        return res;
    } else {
        qDebug() << "failed on addItemToOrder";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::getPermissions(QString login)
{
    result res;
    QSqlQuery query;
    //query.prepare();
    if (query.exec(QString("SELECT get_permissons('%1');").arg(login))){
        query.last();
        res.resStr = query.value(0).toString();
    }else{
        qDebug() << "failed on getPermissions";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::getCharNames(int itemTypeId)
{
    result res;
    QSqlQuery query;
    QJsonArray grpArr;

    if(query.exec(QString("SELECT id_char, name_char FROM chars WHERE chars.id_item_owner_type = %1;").arg(itemTypeId))){
        while(query.next()){
            QJsonObject grp;
            grp["charId"] = query.value(0).toInt();
            grp["charName"] = query.value(1).toString();
            grpArr.append(grp);

        }
        QJsonDocument doc(grpArr);
        res.resStr = doc.toJson(QJsonDocument::Compact);
    }else{
        qDebug() << "failed on getCharNames";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;

}

result ClientDatabase::editItem(int nnid, QString nnme, float nprce, QList<itemChars> *nchrs)
{
    result res;
    QSqlQuery query;

    mainDB.transaction();
    if(query.exec(QString(
                      "UPDATE items SET name_item = '%1', price_item_usd = %2 WHERE id_item = %3;"
                      "DELETE FROM char_text_values WHERE id_item = %3;").arg(nnme).arg(nprce).arg(nnid))){
        for(int i = 0; i < nchrs->size(); i++){
            if(!query.exec(QString("INSERT INTO char_text_values "
                                   "VALUES(%1, %2, '%3', '%4');").arg(nchrs->at(i).charId).arg(nnid).arg(nchrs->at(i).charValue).arg(nchrs->at(i).charUnits))){
                qDebug() << "failed on addchars";
                qDebug() << query.lastError().databaseText();
                res.isError = true;
                res.errorCode = query.lastError().number();
            }

        }
    } else {
        mainDB.rollback();
        qDebug() << "failed on edititem";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::addItem(int typeId, QString nnme, float nprice, QList<itemChars> *nchrs)
{
    result res;
    QSqlQuery query;

    mainDB.transaction();
    if(query.exec(QString(
                      "INSERT INTO items(type_id_item, name_item, price_item_usd, count_in_storage_item) "
                      "VALUES(%1, '%2', %3, 1) "
                      "RETURNING id_item;").arg(typeId).arg(nnme).arg(nprice))){
        query.last();
        int nId = query.value(0).toInt();
        for(int i = 0; i < nchrs->size(); i++){
            if(!query.exec(QString("INSERT INTO char_text_values "
                                   "VALUES(%1, %2, '%3', '%4');").arg(nchrs->at(i).charId).arg(nId).arg(nchrs->at(i).charValue).arg(nchrs->at(i).charUnits))){
                qDebug() << "failed on addchars";
                qDebug() << query.lastError().databaseText();
                res.isError = true;
                res.errorCode = query.lastError().number();
            }
        }
        mainDB.commit();
    } else {
        mainDB.rollback();
        qDebug() << "failed on additem";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::deleteItem(int itemId)
{
    result res;
    QSqlQuery query;

    mainDB.transaction();

    if(query.exec(QString(
                      "DELETE FROM char_text_values WHERE id_item = %1; "
                      "DELETE FROM items WHERE id_item = %1; "
                      "DELETE FROM orders_items WHERE id_item = %1;").arg(itemId))){
        mainDB.commit();
    } else {
        mainDB.rollback();
        qDebug() << "failed on delitem";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;

}

result ClientDatabase::getUserList()
{
    result res;
    QSqlQuery query;
    QJsonArray grpArr;

    if(query.exec(QString("SELECT * FROM user_accounts ORDER BY id_user;"))){
        while(query.next()){
            QJsonObject grp;
            grp["userId"] = query.value(0).toInt();
            grp["userLogin"] = query.value(1).toString();
            grp["userPermissions"] = query.value(2).toString();
            grp["userEmail"] = query.value(3).toString();
            grp["userPassword"] = query.value(4).toString();
            grp["userRegdate"] = query.value(5).toDate().toString("dd:MM:yyyy");
            grpArr.append(grp);

        }
        QJsonDocument doc(grpArr);
        res.resStr = doc.toJson(QJsonDocument::Compact);
    }else{
        qDebug() << "failed on getCharNames";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::createUser(QString nlog, QString npass, QString nmail, QString nperms)
{
    result res;
    QSqlQuery query;

    if(query.exec(QString("SELECT create_user('%1', '%2', '%3', '%4');").arg(nlog).arg(npass).arg(nmail).arg(nperms))){
    } else {
        qDebug() << "failed on createUser";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::deleteUser(int userId)
{
    result res;
    QSqlQuery query;

    if(query.exec(QString("SELECT delete_user(%1);").arg(userId))){
    } else {
        qDebug() << "failed on deleteUser";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::modUser(int uId, QString umail, bool modpass, QString newPerm, QString pass  = "")
{
    result res;
    QSqlQuery query;

    if(query.exec(QString("SELECT mod_user(%1, '%2', '%3', '%4', '%5');").arg(uId).arg(umail).arg(modpass).arg(pass).arg(newPerm))){
    } else {
        qDebug() << "failed on modUser";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}

result ClientDatabase::getInfoAboutOrders()
{
    result res;
    QSqlQuery query;
    QJsonArray arr;
    QSqlQuery query2;

    if (query.exec(QString("SELECT * FROM customer_orders WHERE login_user = user;"))){
        while(query.next()){

            QJsonObject grp;
            grp["orderNum"] = query.value(1).toInt();
            grp["orderDate"] = query.value(3).toDate().toString("dd:MM:yyyy");
            grp["orderSum"] = query.value(4).toDouble();
            grp["userName"] = query.value(6).toString();
            grp["userSur"] = query.value(5).toString();
            grp["userMid"] = query.value(7).toString();
            //grp["itemArr"] = QJsonArray();
            qDebug() << query.value(1).toInt();

            if(query2.exec(QString("SELECT * FROM product_positions WHERE id_order = %1").arg(grp["orderNum"].toInt()))){
                QJsonArray grrr;
                while(query2.next()){
                    QJsonObject grpp;
                    grpp["itemName"] = query2.value(1).toString();
                    grpp["itemSum"] = query2.value(3).toDouble();
                    grpp["itemCount"] = query2.value(2).toInt();

                    grrr.append(grpp);
                }
                grp["itemArr"] = grrr;
                arr.append(grp);
            } else {
                qDebug() << "failed on getInfoOrdersItems";
                qDebug() << query2.lastError().databaseText();
                res.isError = true;
                res.errorCode = query2.lastError().number();
            }
        }

        QJsonDocument doc(arr);
        res.resStr = doc.toJson(QJsonDocument::Compact);
    } else {
        qDebug() << "failed on getInfoOrders";
        qDebug() << query.lastError().databaseText();
        res.isError = true;
        res.errorCode = query.lastError().number();
    }
    return res;
}
