#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QJsonObject>
#include <QDebug>

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = 0);
    Item(int nid, QString nname, int nprice, QObject *parent = 0);
    int getId(){return id;}
    QString getName(){return name;}
    double getPrice(){return price;}

signals:

public slots:
    QJsonObject toJson();
    void fromJson(QJsonObject* json);
private:
    int id;
    QString name;
    double price;
};

#endif // ITEM_H
