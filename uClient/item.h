#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QJsonObject>

class Item : public QObject
{
    Q_OBJECT
public:
    explicit Item(QObject *parent = 0);
    Item(int nid, QString nname, int nprice, QObject *parent = 0);
    int getId(){return id;}
    QString getName(){return name;}
    int getPrice(){return price;}

signals:

public slots:
    QJsonObject toJson();
    void fromJson(QJsonObject* json);
private:
    int id;
    QString name;
    float price;
};

#endif // ITEM_H
