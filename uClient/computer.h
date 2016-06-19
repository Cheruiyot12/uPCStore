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

struct linkedChar {
    int idChar1;
    int idChar2;
};

class Computer : public QObject
{
    Q_OBJECT
public:
    explicit Computer(QObject *parent = 0);
    QList<Item*> hardware;

signals:

public slots:
    QString toJson();
    bool addHardware(Item* itm, bool cc);
    void delHardware(Item* itm);
    void delHardware(int id);
    //void checkCompability(Item* itm);
    double recountPrice();
    bool isEmpty(){return hardware.empty();}

private:

    bool isBuild = false;


};

#endif // COMPUTER_H
