#include "core.h"

Core::Core(QObject *parent) : QObject(parent), server(new QWebSocketServer(QStringLiteral("uPCServer"),
                                                                           QWebSocketServer::NonSecureMode,
                                                                           this))
{
    //generateConfig();
    readConfig();
    generateBat();
    backUpDb();
    initServer(port);

    /*DH* privkey;
    int codes;
    int secret_size;

    privkey = DH_new();
    DH_generate_parameters_ex(privkey, 512, DH_GENERATOR_2, NULL);

    DH_check(privkey, &codes);
    if(codes!=0){
        qDebug() << "DH check failed";
        abort();
    }
    DH_generate_key(privkey);
    //BN_bn2bin()

    DH* pubkey;
    int pubcodes;
    int secret_size_pub;

    pubkey = DH_new();

    //Bin2Bn
    QByteArray privP, privG;
    privP.resize(BN_num_bytes(privkey->p));
    privG.resize(BN_num_bytes(privkey->g));
    BN_bn2bin(privkey->p, (unsigned char*)privP.data());
    BN_bn2bin(privkey->g, (unsigned char*)privG.data());
    //Bin2Bn

    //Bn2Bin
    pubkey->p = BN_new();
    BN_bin2bn((unsigned char*)privP.constData(), privP.length(), pubkey->p);
    //Bn2Bin

    pubkey->g = BN_new();
    BN_bin2bn((unsigned char*)privG.constData(), privG.length(), pubkey->g);
    //pubkey->g = privkey->g;


    DH_check(pubkey, &pubcodes);
    if(pubcodes!=0){
        qDebug() << "DH check failed";
        abort();
    }

    DH_generate_key(pubkey);

    unsigned char* shared_sec1 = new unsigned char[1024];
    unsigned char* shared_sec2 = new unsigned char[1024];

    secret_size = DH_compute_key(shared_sec1, pubkey->pub_key, privkey);
    secret_size_pub = DH_compute_key(shared_sec2, privkey->pub_key, pubkey);


    //qDebug() << strcmp((char*)shared_sec1, (char*)shared_sec2);

    //printf("%s", (char*)shared_sec1);
    //printf("%s", (char*)shared_sec2);
    //QByteArray sh1 = QByteArray::fromRawData((char*)shared_sec1, secret_size);
    //QByteArray sh2 = QByteArray::fromRawData((char*)shared_sec2, secret_size_pub);


    //qDebug() << sh1.toBase64();
    //qDebug() << sh2.toBase64();
*/


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
    connect( newCli, SIGNAL(cliDisc(Client*)), this, SLOT(clientDisconnected(Client*)));
    cliList << newCli;
}

void Core::clientDisconnected(Client *cli)
{
    qDebug() << "Client disconnected";

    cliList.removeAt(cliList.indexOf(cli));
    delete cli;
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
