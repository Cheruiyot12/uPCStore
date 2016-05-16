#include "itemeditwidget.h"

ItemEditWidget::ItemEditWidget(int id, QString nname, float nprice, int ncount, openMode mode = editing, QWidget *parent) : QWidget(parent)
{
    QGridLayout *mlay = new QGridLayout(this);
    this->setLayout(mlay);

    QGridLayout* infLay = new QGridLayout();
    mlay->addLayout(infLay, 0,0);

    mod = mode;

    infLay->addWidget(new QLabel("Id:"),0,0);
    //mlay->addWidget(new QLabel("Категория:"),1,0);
    infLay->addWidget(new QLabel("Название:"),1,0);
    infLay->addWidget(new QLabel("Цена:"),2,0);
    //mlay->addWidget(new QLabel("Количество:"),4,0);

    QLabel *idlabel = new QLabel(QString("%1").arg(id));
    infLay->addWidget(idlabel,0,1);

    //catbox = new QComboBox();
    //catbox->addItems(ngrps);
    //mlay->addWidget(catbox,1,1);

    namel = new QLineEdit(nname);
    infLay->addWidget(namel,1,1);

    pricel = new QLineEdit(QString("%1").arg(nprice));
    infLay->addWidget(pricel,2,1);

    //countl = new QLineEdit(QString("%1").arg(ncount));
    //mlay->addWidget(countl,4,1);
    charTabWid = new QTableWidget();
    charTabWid->setColumnCount(3);
    charTabWid->setHorizontalHeaderLabels(QStringList({"Название", "Значение", "Ед. измерения"}));
    loadBut = new QPushButton("Загрузить характеристики");
    mlay->addWidget(loadBut, 3, 0);
    mlay->addWidget(charTabWid, 4, 0);
    QGridLayout* boxlay = new QGridLayout();
    mlay->addLayout(boxlay, 5, 0);
    charType = new QComboBox();
    val = new QLineEdit();
    un = new QLineEdit();
    adC = new QPushButton("+");
    modC = new QPushButton("*");
    delC = new QPushButton("-");
    boxlay->addWidget(charType,0,0);
    boxlay->addWidget(val,0,1);
    boxlay->addWidget(un,0,2);
    boxlay->addWidget(adC, 0, 3);
    boxlay->addWidget(delC, 0, 4);
    boxlay->addWidget(modC, 0, 5);
    charTabWid->setSelectionBehavior(QAbstractItemView::SelectRows);
    charTabWid->setSelectionMode(QAbstractItemView::SingleSelection);
    charTabWid->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QGridLayout* dlay = new QGridLayout;
    mlay->addLayout(dlay,6,0);
    saveBut = new QPushButton("Сохранить");
    dlay->addWidget(saveBut,0,0);




    connect(loadBut, SIGNAL(clicked(bool)), this, SLOT(onLoad()));
    connect(charTabWid, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onSelect()));

    connect(adC, SIGNAL(clicked(bool)), this, SLOT(onAdC()));
    connect(modC, SIGNAL(clicked(bool)), this, SLOT(onModC()));
    connect(delC, SIGNAL(clicked(bool)), this, SLOT(onDelC()));
    connect(saveBut, SIGNAL(clicked(bool)), this, SLOT(onSave()));

    //onLoad();
}

void ItemEditWidget::loadChrs(QList<itemChars> *chrs)
{
    //charTabWid->clear();
    if(!charsLoaded){
        for(int i = 0; i < chrs->size(); i++){
            charTabWid->insertRow(charTabWid->rowCount());
            charTabWid->setItem(charTabWid->rowCount()-1, 0, new QTableWidgetItem(chrs->at(i).charname));
            charTabWid->setItem(charTabWid->rowCount()-1, 1, new QTableWidgetItem(chrs->at(i).charValue));
            charTabWid->setItem(charTabWid->rowCount()-1, 2, new QTableWidgetItem(chrs->at(i).charUnits));
        }
        connect(charTabWid, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onSelect()));
    }
    charsLoaded = true;

}

void ItemEditWidget::onLoad()
{
    emit this->getChrs();
}

void ItemEditWidget::onSelect()
{
    if(charTabWid->currentRow()>=0 && !(charTabWid->selectedItems().empty()) && un && val && charType){

        val->setText(charTabWid->item(charTabWid->currentRow(), 1)->text());
        un->setText(charTabWid->item(charTabWid->currentRow(), 2)->text());
        //int in = catbox->findText(charTabWid->item(charTabWid->currentRow(), 0)->text());
        //int in = catbox->findText("KURWA!!!");
        charType->setCurrentText(charTabWid->item(charTabWid->currentRow(), 0)->text());
    }

}

void ItemEditWidget::loadChrsNam(QList<chars> *chrs)
{
    for (int i = 0; i < chrs->size(); i++){
        charType->addItem(chrs->at(i).charname, chrs->at(i).charId);
    }
    //charsLoaded = true;
}

void ItemEditWidget::onAdC()
{
    charTabWid->insertRow(charTabWid->rowCount());
    charTabWid->setItem(charTabWid->rowCount()-1, 0, new QTableWidgetItem(charType->currentText()));
    charTabWid->setItem(charTabWid->rowCount()-1, 1, new QTableWidgetItem(val->text()));
    charTabWid->setItem(charTabWid->rowCount()-1, 2, new QTableWidgetItem(un->text()));
}

void ItemEditWidget::onDelC()
{
    if(charTabWid->rowCount()>0){
        charTabWid->removeRow(charTabWid->currentRow());
    }
}

void ItemEditWidget::onModC()
{
    if(charTabWid->currentRow()>=0){
        charTabWid->setItem(charTabWid->currentRow(), 0, new QTableWidgetItem(charType->currentText()));
        charTabWid->setItem(charTabWid->currentRow(), 1, new QTableWidgetItem(val->text()));
        charTabWid->setItem(charTabWid->currentRow(), 2, new QTableWidgetItem(un->text()));
    }
}

void ItemEditWidget::onSave()
{
    QList<itemChars> *ldedChrs = new QList<itemChars>();
   // if(charsLoaded){
        for(int i = 0; i < charTabWid->rowCount(); i++){
            itemChars itmc;
            itmc.charname = charTabWid->item(i,0)->text();
            itmc.charUnits = charTabWid->item(i,2)->text();
            itmc.charValue = charTabWid->item(i,1)->text();
            ldedChrs->append(itmc);
        }
    //}
    emit this->onSaveSig(mod, id, namel->text(), pricel->text().toFloat(), ldedChrs);
}
