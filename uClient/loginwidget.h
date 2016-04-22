#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QUrl>
#include <QPalette>

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = 0);

signals:
    void loginSig(QString, QString);
    void connectSig(QUrl);

public slots:
    void onLogin();
    void onConnect();
private:
    QLineEdit* logLine;
    QLineEdit* passLine;
    QLineEdit* ipLine;
    QPushButton* logIn;
};

#endif // LOGINWIDGET_H
