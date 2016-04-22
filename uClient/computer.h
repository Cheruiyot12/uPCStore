#ifndef COMPUTER_H
#define COMPUTER_H

#include <QObject>
#include <item.h>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>
#include <QDebug>

class Computer : public QObject
{
    Q_OBJECT
public:
    explicit Computer(QObject *parent = 0);

signals:

public slots:
    QJsonObject toJson();
    void addHardware(Item* itm);
    void delHardware(Item* itm);
    void delHardware(int id);
    //void checkCompability(Item* itm);

private:
    QList<Item*> hardware;
    bool isBuild = false;


};

#endif // COMPUTER_H
