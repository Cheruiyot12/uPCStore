#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = 0);

signals:
    void addUser(QString);
    void modUser(QString);
    void delUser(QString);

    void requestUsers();

public slots:
    void loadUsers(QJsonArray *arr);

private:
    QTableWidget *userwid;
    QLineEdit *logLin, *mailLin, *passLin, *cardNumLin, *cardNumDate, *nameLin, *surLin, *midLin;
    QComboBox *groupBox;
    QPushButton *aduBut, *moduBat, *deluBat;

private slots:
    void onSelect();
    void onEdit();
    void onAdd();
    void onDelete();

};

#endif // USERWIDGET_H
