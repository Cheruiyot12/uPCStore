#include "loginwidget.h"

LoginWidget::LoginWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mainlay = new QGridLayout();
    this->setLayout(mainlay);
    mainlay->addWidget(new QLabel("Login: ", 0, 0));
    mainlay->addWidget(new QLabel("Password: "), 1, 0);
    mainlay->addWidget(new QLabel("IP address: "), 2, 0);

    logLine = new QLineEdit();
    passLine = new QLineEdit();
    ipLine = new QLineEdit();
    ipLine->setText("localhost");
    mainlay->addWidget(logLine, 0, 1);
    mainlay->addWidget(passLine, 1, 1);
    mainlay->addWidget(ipLine, 2, 1);

    logIn = new QPushButton("Log In");
    mainlay->addWidget(logIn,3,0);
    QPushButton* conn = new QPushButton("Connect");
    mainlay->addWidget(conn,3,1);

    connect(conn, SIGNAL(clicked(bool)), this, SLOT(onConnect()));
    connect(logIn, SIGNAL(clicked(bool)), this, SLOT(onLogin()));

}

void LoginWidget::onLogin()
{
    emit this->loginSig(logLine->text(), passLine->text());
}

void LoginWidget::onConnect()
{
    QString qst = QString("ws://%1:6969").arg(ipLine->text());
    emit this->connectSig(QUrl(qst));
}
