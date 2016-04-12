#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <clientdatabase.h>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <commands.h>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QWebSocket* sock, QObject *parent = 0);

signals:
    void cliDisc(Client*);

public slots:
private:
    QWebSocket* mainSocket;
    ClientDatabase* dataBase;
    bool isLoggedIn = false;

    bool isDebug = true;
    void sendTextMes(QString msg);
private slots:
    void onTextMessage(QString msg);
    void onDisconnect();
};

#endif // CLIENT_H
