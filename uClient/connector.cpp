#include "connector.h"

Connector::Connector(QObject *parent) : QObject(parent)
{
    connect(&cliSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(&cliSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(&cliSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(onTextMsg(QString)));
}

void Connector::onConnected()
{
    qDebug() << "connected";
    connected = true;
}

void Connector::onDisconnected()
{
    qDebug() << "disconnected";
    connected = false;
}

void Connector::connect2server(QUrl url)
{

    qDebug() << "try to connect " << url.toString();

    cliSocket.open(url);
}

void Connector::onTextMsg(QString msg)
{
    qDebug() << msg;
    QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
    QJsonObject obj = doc.object();

    switch (obj["command"].toInt()) {
    case successLogin:
    {
        emit this->onSuccessLogin();
        break;
    }
    case error:
    {
        emit this->onErr(obj["eRRoRcode"].toInt());
    }
    default:
        break;
    }
}

void Connector::onBinMsg(QByteArray msg)
{

}

void Connector::sendTextMess(QString msg)
{
    cliSocket.sendTextMessage(msg);
}
