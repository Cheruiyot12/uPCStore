#ifndef ITEMGROUP_H
#define ITEMGROUP_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class ItemGroup : public QObject
{
    Q_OBJECT
public:
    explicit ItemGroup(QObject *parent = 0);
    int getId(){return id;}
    QString getName(){return name;}

signals:

public slots:
    QJsonObject toJson();
    void fromJson(QJsonObject* json);
private:
    int id;
    QString name;

};

#endif // ITEMGROUP_H
