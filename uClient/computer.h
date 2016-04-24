#ifndef COMPUTER_H
#define COMPUTER_H

#include <QObject>
#include <item.h>
#include <QList>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QString>
#include <QDebug>

class Computer : public QObject
{
    Q_OBJECT
public:
    explicit Computer(QObject *parent = 0);

signals:

public slots:
    QString toJson();
    void addHardware(Item* itm);
    void delHardware(Item* itm);
    void delHardware(int id);
    //void checkCompability(Item* itm);
    double recountPrice();
    bool isEmpty(){return hardware.empty();}

private:
    QList<Item*> hardware;
    bool isBuild = false;


};

#endif // COMPUTER_H
