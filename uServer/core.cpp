#include "core.h"

Core::Core(QObject *parent) : QObject(parent), server(new QWebSocketServer(QStringLiteral("uPCServer"),
                                                                           QWebSocketServer::NonSecureMode,
                                                                           this))
{
    initServer(6969);
}

void Core::initServer(quint16 port)
{
    if(server->listen(QHostAddress::Any, port)){
        qDebug() << "Server started";
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    } else {
        qDebug() << "Server not started";
    }
}

void Core::newConnection()
{
    qDebug() << "new CLIENT!!!!!!!!!!!!!!!";
    Client* newCli = new Client(server->nextPendingConnection());
    cliList << newCli;
}

void Core::clientDisconnected(Client *cli)
{
    qDebug() << "Client disconnected";
    cliList.removeAt(cliList.indexOf(cli));
}
