#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QtWebSockets>
#include <QWebSocket>
#include <QDebug>
#include <../uServer/commands.h>
#include <qacrypt.h>
#include <openssl/err.h>
//#include <cryptor.h>

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

    void sendItemValuesToShow(QStringList*, QStringList*, QStringList*);
    void sendItemValuesToEdit(QList<itemChars> *, QJsonArray *);

    void orderPlacedSig(int id);

    void obtaintedPermsSig(QString perms);
    void onCharNames(QList<chars> *);

    void succDelItem();

    void onUserList(QJsonArray *arr);
    void onSuccUserMod();

    void onOrderList(QJsonArray *arr);
    void enableLogin();

    void GodOfCharsAppears(QJsonArray *ar);
    void LinksKing(QJsonArray *ar);
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
    bool useAes = false;
    QByteArray aesKey;
    DH* myPrivKey;
    QByteArray serverOpenKey;


};

#endif // CONNECTOR_H
