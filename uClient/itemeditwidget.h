#ifndef ITEMEDITWIDGET_H
#define ITEMEDITWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QDebug>
#include "../uServer/commands.h"

enum openMode{
    editing,
    adding
};

class ItemEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItemEditWidget(int nid, QString nname, float nprice, int ncount, openMode mode, QWidget *parent = 0);

signals:
    void getChrs();
    void onSaveSig(openMode sMode, int nnid, QString nnme, float nprce, QList<itemChars>* nchrs);



public slots:
    void loadChrs(QList<itemChars>*chrs);
    void loadChrsNam(QList<chars>*chrs);

private:
    int id, typeId, count;
    QString name;
    openMode mod;
    QStringList grps;
    QComboBox* catbox;
    QLineEdit* namel, *pricel, *countl;
    QTableWidget* charTabWid;
    bool charsLoaded = false;
    QPushButton* exitb, *saveb, *loadBut;
    QComboBox* charType;
    QLineEdit *val, *un;
    QPushButton *adC, *modC, *delC, *saveBut;

private slots:
    void onLoad();
    void onSelect();
    void onAdC();
    void onModC();
    void onDelC();
    void onSave();


};

#endif // ITEMEDITWIDGET_H
