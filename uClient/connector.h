#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QtWebSockets>
#include <QWebSocket>
#include <QDebug>
#include <../uServer/commands.h>

class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);

signals:
    void onSuccessLogin();
    void onErr(int);

    void onNewGroup(QJsonObject obj);
    void groupsToWidget();
    void clearGr();

    void onNewItem(QJsonObject obj);
    void itemsToList();
    void clearItms();
public slots:
    void connect2server(QUrl url);
    void sendTextMess(QString msg);
    bool getConnected(){return connected;}
private slots:
    void onConnected();
    void onDisconnected();
    void onTextMsg(QString msg);
    void onBinMsg(QByteArray msg);
private:
    QWebSocket cliSocket;
    bool connected = false;
};

#endif // CONNECTOR_H
