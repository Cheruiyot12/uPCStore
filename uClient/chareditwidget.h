#ifndef CHAREDITWIDGET_H
#define CHAREDITWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "../uServer/commands.h"
#include <QDebug>
#include <QTableWidgetItem>


class CharEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CharEditWidget(QWidget *parent = 0);
    QTableWidget* linktab;
    QGridLayout* mainLay;
    QGridLayout* leftLay;
    QGridLayout* rightLay;
    QList<chss> charss;

    QComboBox *c1Com;
    QComboBox *c2Com;

    QPushButton *adl;
    QPushButton *dll;



signals:
    void adls(QString d);
    void dels(QString d);

public slots:

    void loadChars(QJsonArray *jj);
    void loadLinks(QJsonArray *jj);
    void adLink();
    void delLink();
};

#endif // CHAREDITWIDGET_H
