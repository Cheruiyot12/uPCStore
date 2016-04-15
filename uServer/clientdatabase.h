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

class ClientDatabase : public QObject
{
    Q_OBJECT
public:
    explicit ClientDatabase(QObject *parent = 0);

signals:

public slots:
    bool loginToDb(QString log, QString pass);
    bool auth(QString log, QString pass);
    QString getGroups();
private:
    QSqlDatabase mainDB;

};

#endif // CLIENTDATABASE_H
