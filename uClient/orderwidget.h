#ifndef ORDERWIDGET_H
#define ORDERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QJsonArray>
#include <QJsonObject>
#include <QGridLayout>
#include <QTableWidget>
#include <QList>
#include <QStringList>
#include <QJsonDocument>
#include <QDate>
#include <QDebug>
#include <xlsxdocument.h>
#include <QFileDialog>
struct ordItem{
    QString name;
    int count;
    double sum;
};
struct ordr{
    int orderNum;
    QString orderDate;
    double orderSum;
    QString userName, userSur, userMid;
    QList<ordItem> itms;
};



class orderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit orderWidget(QWidget *parent = 0);

signals:

public slots:
    void loadOrderNumbers(QJsonArray *arr);
    void onSelectionChanged(int selId);
    void genPrice();

private:
    QComboBox *orderNumber;
    QLabel* orderDate, *orderSum, *userName, *userSur, *userMid;
    QTableWidget *itemsIn;

    QList<ordr> ords;
};

#endif // ORDERWIDGET_H
