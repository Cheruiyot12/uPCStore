#ifndef CLIENTDATABASE_H
#define CLIENTDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QDataStream>

struct result{
    bool isError = false;
    int errorCode = 0;
    QString resStr = "";
};

class ClientDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ClientDatabase(QObject *parent = 0);

signals:

public slots:
    bool loginToDb(QString log, QString pass);
    bool auth(QString log, QString pass);
    result getGroups();
    result getItemsFromGroup(int grId);
    result getCharsOfItem(int itemId);
    result placeOrder();
    result addItemToOrder(int orderId, int itemId, int itemCount);
private:
    QSqlDatabase mainDB;

};

#endif // CLIENTDATABASE_H
