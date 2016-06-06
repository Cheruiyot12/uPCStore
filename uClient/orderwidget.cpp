#include "orderwidget.h"

orderWidget::orderWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout *mainLay = new QGridLayout;
    this->setLayout(mainLay);
    QGridLayout *leftLay = new QGridLayout;
    mainLay->addLayout(leftLay, 0, 0);

    orderNumber = new QComboBox();
    userName = new QLabel();
    userSur = new QLabel();
    userMid = new QLabel();
    orderSum = new QLabel();
    orderDate = new QLabel();
    leftLay->addWidget(new QLabel("Номер:"),0, 0);
    leftLay->addWidget(orderNumber, 0, 1);
    leftLay->addWidget(new QLabel("Дата:"),1, 0);
    leftLay->addWidget(orderDate, 1, 1);
    leftLay->addWidget(new QLabel("Фамилия:"),2, 0);
    leftLay->addWidget(userSur, 2, 1);
    leftLay->addWidget(new QLabel("Имя:"),3, 0);
    leftLay->addWidget(userName, 3, 1);
    leftLay->addWidget(new QLabel("Отчество:"),4, 0);
    leftLay->addWidget(userMid, 4, 1);
    leftLay->addWidget(new QLabel("Сумма:"),5, 0);
    leftLay->addWidget(orderSum, 5, 1);

    itemsIn = new QTableWidget(0, 3);
    QStringList heds = {"Название", "Количество", "Стоимость"};
    mainLay->addWidget(itemsIn, 0, 1);
    itemsIn->setHorizontalHeaderLabels(heds);
    QPushButton *genPrice = new QPushButton("Создать чек");

    mainLay->addWidget(genPrice, 1, 1);

    connect(genPrice, SIGNAL(clicked(bool)), this, SLOT(genPrice()));

    connect(orderNumber, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectionChanged(int)));
    this->setMinimumWidth(640);
}

void orderWidget::loadOrderNumbers(QJsonArray *arr)
{
    ords.clear();
    orderNumber->clear();

    for(int i = 0; i < arr->count(); i++){
        QJsonObject obj = arr->at(i).toObject();

        ordr ord;
        ord.orderNum = obj.value("orderNum").toInt();
        ord.orderDate = obj.value("orderDate").toString();
        ord.orderSum = obj.value("orderSum").toDouble();
        ord.userName = obj.value("userName").toString();
        ord.userSur = obj.value("userSur").toString();
        ord.userMid = obj.value("userMid").toString();
        //QJsonDocument doct;
        //doct = QJsonDocument::fromJson(obj.value("itemArr").toString().toUtf8());
        QJsonArray itArr = obj.value("itemArr").toArray();

        for(int j = 0; j < itArr.count(); j++){
            QJsonObject objc = itArr.at(j).toObject();
            ordItem ordi;

            ordi.name = objc["itemName"].toString();
            //qDebug() << ordi.name;
            ordi.sum = objc["itemSum"].toDouble();
            ordi.count = objc["itemCount"].toInt();
            ord.itms.append(ordi);
        }
        ords.append(ord);
        orderNumber->addItem(QString("%1").arg(obj.value("orderNum").toInt()));
    }
    this->show();

    //genPrice();

}

void orderWidget::onSelectionChanged(int selId)
{
    if(selId > -1){
        itemsIn->setRowCount(0);
        ordr ord = ords[selId];
        orderDate->setText(ord.orderDate);
        orderSum->setText(QString("%1").arg(ord.orderSum));
        userName->setText(ord.userName);
        userSur->setText(ord.userSur);
        userMid->setText(ord.userMid);

        for(int i = 0; i < ord.itms.size(); i++){
            itemsIn->insertRow(itemsIn->rowCount());

            itemsIn->setItem(itemsIn->rowCount()-1, 0, new QTableWidgetItem(ord.itms[i].name));
            itemsIn->setItem(itemsIn->rowCount()-1, 2, new QTableWidgetItem(QString("%1").arg(ord.itms[i].sum)));
            itemsIn->setItem(itemsIn->rowCount()-1, 1, new QTableWidgetItem(QString("%1").arg(ord.itms[i].count)));
        }
    } else {
        orderDate->clear();
        orderSum->clear();
        userName->clear();
        userSur->clear();
        userMid->clear();

        itemsIn->setRowCount(0);
    }
}

void orderWidget::genPrice()
{
    if(orderNumber->currentIndex()>-1){
        QString path = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения", "C:\\");

        QXlsx::Document xlsx;

        xlsx.write("A1", "Заказ №");
        xlsx.write("A2", "Дата:");
        xlsx.write("A4", "Фамилия:");
        xlsx.write("A5", "Имя:");
        xlsx.write("A6", "Отчество:");

        QXlsx::Format form;
        form.setHorizontalAlignment(QXlsx::Format::AlignHCenter);

        xlsx.mergeCells("B8:D8", form);
        xlsx.write("B8", "Товарные позиции:", form);
        xlsx.write("B9", "Название", form);
        xlsx.write("C9", "Количество", form);
        xlsx.write("D9", "Сумма", form);

        xlsx.setColumnWidth(1, 12);
        xlsx.setColumnWidth(2, 25);
        xlsx.setColumnWidth(3, 12);
        xlsx.setColumnWidth(4, 15);

        ordr ord = ords[orderNumber->currentIndex()];

        xlsx.write("B1", ord.orderNum);
        xlsx.write("B2", ord.orderDate);
        xlsx.write("B4", ord.userSur);
        xlsx.write("B5", ord.userName);
        xlsx.write("B6", ord.userMid);

        int i = 0;

        for (i = 0; i < ord.itms.size(); i++){
            xlsx.write(QString("B%1").arg(i+10), ord.itms.at(i).name);
            xlsx.write(QString("C%1").arg(i+10), ord.itms.at(i).count);
            xlsx.write(QString("D%1").arg(i+10), ord.itms.at(i).sum);
        }

        xlsx.write(QString("A%1").arg(i+12), "Итого:");
        xlsx.write(QString("D%1").arg(i+12), ord.orderSum);


        xlsx.saveAs(path+"\\CompSave.xlsx");
    }
}
