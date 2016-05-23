#include "userwidget.h"
#include "../uServer/commands.h"

UserWidget::UserWidget(QWidget *parent) : QWidget(parent)
{
    QGridLayout* mainlay = new QGridLayout();
    this->setLayout(mainlay);
    this->setMinimumWidth(640);

    userwid = new QTableWidget();
    userwid->setColumnCount(5);
    QStringList ss = {"Логин", "Права", "E-mail" ,"Пароль", "Дата регистрации"};
    userwid->setHorizontalHeaderLabels(ss);
    userwid->setSelectionBehavior(QAbstractItemView::SelectRows);
    userwid->setSelectionMode(QAbstractItemView::SingleSelection);
    userwid->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainlay->addWidget(userwid, 0, 0);

    logLin = new QLineEdit();
    groupBox = new QComboBox();
    mailLin = new QLineEdit();
    passLin = new QLineEdit();

    QGridLayout *lineLay = new QGridLayout();
    lineLay->addWidget(logLin, 0,0);
    lineLay->addWidget(groupBox, 0, 1);
    lineLay->addWidget(mailLin, 0, 2);
    lineLay->addWidget(passLin, 0, 3);

    groupBox->addItems(QStringList({"user", "seller", "admin"}));

    mainlay->addLayout(lineLay, 1, 0);

    QGridLayout *butLay = new QGridLayout();
    aduBut = new QPushButton("Добавить");
    moduBat = new QPushButton("Изменить");
    deluBat = new QPushButton("Удалить");
    butLay->addWidget(aduBut, 0 ,0);
    butLay->addWidget(moduBat, 0, 1);
    butLay->addWidget(deluBat, 0, 2);

    mainlay->addLayout(butLay, 2, 0);

    connect(userwid, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onSelect()));
    connect(aduBut, SIGNAL(clicked(bool)), this, SLOT(onAdd()));
    connect(moduBat, SIGNAL(clicked(bool)), this, SLOT(onEdit()));
    connect(deluBat, SIGNAL(clicked(bool)), this, SLOT(onDelete()));

}

void UserWidget::loadUsers(QJsonArray *arr)
{
    userwid->setRowCount(0);
    for(int i = 0; i < arr->count(); i++){
        QJsonObject obj = arr->at(i).toObject();

        userwid->insertRow(userwid->rowCount());
        QTableWidgetItem *namItm = new QTableWidgetItem();
        namItm->setText((obj.value("userLogin").toString()));
        namItm->setData(1000, obj.value("userId").toInt());
        namItm->setData(1001, obj.value("userLogin").toString());
        userwid->setItem(userwid->rowCount()-1, 0, namItm);
        userwid->setItem(userwid->rowCount()-1, 1, new QTableWidgetItem(obj.value("userPermissions").toString()));
        userwid->setItem(userwid->rowCount()-1, 2, new QTableWidgetItem(obj.value("userEmail").toString()));
        userwid->setItem(userwid->rowCount()-1, 3, new QTableWidgetItem(obj.value("userPassword").toString()));
        userwid->setItem(userwid->rowCount()-1, 4, new QTableWidgetItem(obj.value("userRegdate").toString()));
    }
}

void UserWidget::onSelect()
{
    if(userwid->currentRow()>-1){
        logLin->setText(userwid->item(userwid->currentRow(), 0)->text());
        groupBox->setCurrentText(userwid->item(userwid->currentRow(), 1)->text());
        mailLin->setText(userwid->item(userwid->currentRow(), 2)->text());
    }
}

void UserWidget::onAdd()
{
    QJsonObject usr;
    usr["command"] = addUserC;
    usr["userLogin"] = logLin->text();
    usr["userPermissions"] = groupBox->currentText();
    usr["userEmail"] = mailLin->text();
    usr["userPassword"] = passLin->text();
    QJsonDocument doc(usr);

    emit this->addUser(doc.toJson(QJsonDocument::Compact));
}

void UserWidget::onEdit()
{
    if(userwid->currentRow()>-1){
        QJsonObject usr;
        usr["command"] = modUserC;
        usr["userId"] = userwid->item(userwid->currentRow(), 0)->data(1000).toInt();
        usr["userLogin"] = userwid->item(userwid->currentRow(), 0)->data(1001).toString();
        usr["userPermissions"] = groupBox->currentText();
        usr["userEmail"] = mailLin->text();
        if(passLin->text().length() != 0){
            usr["modPass"] = true;
            usr["userPassword"] = passLin->text();
        } else {
            usr["modPass"] = false;
        }
        QJsonDocument doc(usr);

        emit this->addUser(doc.toJson(QJsonDocument::Compact));
    }
}

void UserWidget::onDelete()
{
    if(userwid->currentRow()>-1){
        QJsonObject usr;
        usr["command"] = delUserc;
        usr["userId"] = userwid->item(userwid->currentRow(), 0)->data(1000).toInt();
        QJsonDocument doc(usr);

        emit this->addUser(doc.toJson(QJsonDocument::Compact));
    }
}
