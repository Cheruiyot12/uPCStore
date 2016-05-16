#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>
#include <QList>
#include <QJsonArray>
#include <../uServer/commands.h>

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = 0);
    Item(int nid, QString nname, int nprice, QObject *parent = 0);
    int getId(){return id;}
    QString getName(){return name;}
    double getPrice(){return price;}

    void setName(QString name){this->name = name;}
    void setPrice(double price){this->price = price;}
    void addChars(QJsonArray* arr);
    QList<itemChars> *getChars();
    bool isChLoaded(){return charsLoaded;}

signals:

public slots:
    QJsonObject toJson();
    void fromJson(QJsonObject* json);
private:
    int id;
    QString name;
    double price;
    QList<itemChars> chars;
    bool charsLoaded = false;

};

#endif // ITEM_H
