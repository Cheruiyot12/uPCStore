#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <client.h>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

signals:

public slots:
    void initServer(quint16 port);
    void clientDisconnected(Client* cli);
private slots:
    void newConnection();
private:
    QWebSocketServer* server;
    QList<Client*> cliList;
};

#endif // CORE_H
