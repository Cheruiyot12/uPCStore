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
    query.prepare(QString("INSERT INTO orders(id_customer, order_date) "
                          "VALUES ((SELECT id_user FROM users WHERE users.login_user = quote_ident(USER)), now()) "
                          "RETURNING id_order;"));
    if (query.exec()){
        query.last();
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
