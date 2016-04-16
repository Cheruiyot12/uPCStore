#include "core.h"

Core::Core(QObject *parent) : QObject(parent), server(new QWebSocketServer(QStringLiteral("uPCServer"),
                                                                           QWebSocketServer::NonSecureMode,
                                                                           this))
{
    //generateConfig();
    readConfig();
    generateBat();
    //backUpDb();
    initServer(port);
}

void Core::initServer(quint16 port)
{
    if(server->listen(QHostAddress::Any, port)){
        qDebug() << "Server started";
        connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
        //backUpDb();
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

void Core::generateBat()
{
    QString batStr = QString("@echo off\n"
                             "SET PGPASSWORD=%1\n"
                             "cd /D %2\npg_dump -U %3 -F t %4 > %5\\uPCStore_%6.tar").arg(dbPass,
                                                                                          postPath,
                                                                                          dbUser,
                                                                                          dbName,
                                                                                          backPath,
                                                                                          QDate::currentDate().toString("dd.MM.yyyy"));
    QFile backfile("backupDB.bat");
    if(!backfile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "can't gen bat file";
        return;
    }
    backfile.write(batStr.toUtf8());
}

void Core::backUpDb()
{
    QProcess backuper;
    QString progPath = "backupDB.bat";

    backuper.start(progPath);

    backuper.waitForFinished(-1);
}

void Core::readConfig()
{
    QFile configFile("config.conf");
    if(!configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "cannot open file to read config... regenerating";
        generateConfig();
        readConfig();
        return;
    }

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(configFile.readAll(), &parseError);
    if(parseError.error == QJsonParseError::NoError)
    {
        if(jsonDoc.isObject())
        {
            port = jsonDoc.object().value("port").toString().toUShort();
            dbPort = jsonDoc.object().value("databasePort").toString().toUShort();

            dbIP = jsonDoc.object().value("databaseIP").toString();
            dbUser = jsonDoc.object().value("databaseUser").toString();
            dbName = jsonDoc.object().value("databaseName").toString();
            dbPass = jsonDoc.object().value("databaseUserPassword").toString();
            postPath = jsonDoc.object().value("postgreBinPath").toString();
            backPath = jsonDoc.object().value("backupDestination").toString();

        }
    } else {
        qDebug()<<parseError.errorString();
    }

}

void Core::backupDB()
{

}

void Core::generateConfig()
{
    QJsonObject configObj;
    configObj["port"] = "6969";
    configObj["databaseIP"] = "localhost";
    configObj["databasePort"] = "5432";
    configObj["databaseUser"] = "postgres";
    configObj["databaseUserPassword"] = "alkor";
    configObj["databaseName"] = "uPCStore";
    configObj["postgreBinPath"] = "C:\\Program Files\\PostgreSQL\\9.5\\bin";
    configObj["backupDestination"] = "D:\\pgbackup";

    QJsonDocument confDoc(configObj);

    QFile configFile("config.conf");
    if(!configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "cannot open file to generate config";
        return;
    }
    configFile.write(confDoc.toJson());
}
