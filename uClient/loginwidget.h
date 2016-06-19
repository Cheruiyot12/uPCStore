#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QUrl>
#include <QPalette>
#include <QMessageBox>

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
    void enableLogin();
private:
    QLineEdit* logLine;
    QLineEdit* passLine;
    QLineEdit* ipLine;
    QPushButton* logIn;
    QPushButton* conn;
};

#endif // LOGINWIDGET_H
