#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <client.h>
#include <QProcess>
#include <QProcessEnvironment>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>
#include <QMessageBox>
#include <QDateTime>
#include <qacrypt.h>
#include <openssl/dh.h>

class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

signals:

public slots:
    void initServer(quint16 port);
    void clientDisconnected(Client* cli);
    void backUpDb();
    void generateBat();
private slots:
    void newConnection();
    void readConfig();
    void generateConfig();
    void backupDB();
private:
    QWebSocketServer* server;
    QList<Client*> cliList;
    quint16 port, dbPort;
    QString dbIP, dbUser, dbPass, dbName, postPath, backPath;
};

#endif // CORE_H
