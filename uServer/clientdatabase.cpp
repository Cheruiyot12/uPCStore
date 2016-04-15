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
    query.prepare(QString("SELECT authorize('%1', '%2')").arg(log).arg(pass));
    if(query.exec()){
        query.last();
        qDebug() << query.value(0) << log;
        return query.value(0).toBool();
    }else{
        qDebug() << "failed on auth";
        qDebug() << query.lastError().databaseText();
    }
}

QString ClientDatabase::getGroups()
{
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
        return doc.toJson(QJsonDocument::Compact);
    } else {
        qDebug() << "failed on getGroups";
        qDebug() << query.lastError().databaseText();
        return "NULL";
    }
}
