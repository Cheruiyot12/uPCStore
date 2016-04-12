#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <mainwindow.h>
#include <loginwidget.h>
#include <connector.h>
#include <QJsonObject>
#include <QJsonDocument>


class Core : public QObject
{
    Q_OBJECT
public:
    explicit Core(QObject *parent = 0);

signals:

public slots:
    void showInterface();
    void logIn(QString log, QString pass);
    void onSucessLogin();
    void errorHandler(int code);
private:
    MainWindow* mw;
    LoginWidget* lw;
    Connector* con;

};

#endif // CORE_H
