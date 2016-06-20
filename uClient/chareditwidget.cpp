#include "chareditwidget.h"

CharEditWidget::CharEditWidget(QWidget *parent) : QWidget(parent)
{
    mainLay = new QGridLayout();
    this->setLayout(mainLay);
    rightLay = new QGridLayout();
    mainLay->addLayout(rightLay, 0, 1);

    c1Com = new QComboBox();
    c2Com = new QComboBox();
    linktab = new QTableWidget();

    rightLay->addWidget(linktab,0,0);

    QGridLayout* bl1 = new QGridLayout();
    rightLay->addLayout(bl1,1,0);

    bl1->addWidget(c1Com,0,0);
    bl1->addWidget(c2Com,0,1);

    adl = new QPushButton("+");
    dll = new QPushButton("-");

    bl1->addWidget(adl, 0, 2);
    bl1->addWidget(dll, 0, 3);

    linktab->setColumnCount(2);
    linktab->setHorizontalHeaderLabels(QStringList({"Хар-ка 1", "Хар-ка 2"}));

    connect(adl, SIGNAL(clicked(bool)), this, SLOT(adLink()));
    connect(dll, SIGNAL(clicked(bool)), this, SLOT(delLink()));

}

void CharEditWidget::loadChars(QJsonArray *jj)
{
    c1Com->clear();
    c2Com->clear();
    //linktab->clear();
    for(int i = 0; i < jj->count(); i++){
        QJsonObject obj;
        obj = jj->at(i).toObject();
        c1Com->addItem(obj["typeName"].toString()+"."+obj["charName"].toString());
        c1Com->setItemData(i, obj["charId"].toInt(), 1001);
        c1Com->setItemData(i, obj["typeId"].toInt(), 1002);

        c2Com->addItem(obj["typeName"].toString()+"."+obj["charName"].toString());
        c2Com->setItemData(i, obj["charId"].toInt(), 1001);
        c2Com->setItemData(i, obj["typeId"].toInt(), 1002);
    }
}

void CharEditWidget::loadLinks(QJsonArray *jj)
{
    linktab->setRowCount(0);
    for(int i = 0; i < jj->count(); i++){
        linktab->insertRow(linktab->rowCount());
        QJsonObject obj;
        obj=jj->at(i).toObject();
        QTableWidgetItem* it1 = new QTableWidgetItem(obj["typeName1"].toString()+"."+obj["charName1"].toString());
        it1->setData(1000, obj["charId1"].toInt());
        linktab->setItem(linktab->rowCount()-1, 0, it1);


        QTableWidgetItem* it2 = new QTableWidgetItem(obj["typeName2"].toString()+"."+obj["charName2"].toString());
        it2->setData(1000, obj["charId2"].toInt());
        linktab->setItem(linktab->rowCount()-1, 1, it2);
    }
}

void CharEditWidget::adLink()
{
    if(c1Com->count() != 0){
        QJsonObject ob;
        ob["charId1"] = c1Com->currentData(1001).toInt();
        ob["charId2"] = c2Com->currentData(1001).toInt();
        ob["command"] = addLink;
        QJsonDocument dc(ob);
        emit this->adls(dc.toJson(QJsonDocument::Compact));
    }
}

void CharEditWidget::delLink()
{
    if(linktab->rowCount()>0 && linktab->currentRow()>-1){
        QJsonObject ob;
        ob["charId1"] = linktab->item(linktab->currentRow(), 0)->data(1000).toInt();
        ob["charId2"] = linktab->item(linktab->currentRow(), 1)->data(1000).toInt();
        ob["command"] = dellLink;
        QJsonDocument dc(ob);
        qDebug() << dc.toJson(QJsonDocument::Compact);
        emit this->dels(dc.toJson(QJsonDocument::Compact));
    }
}
