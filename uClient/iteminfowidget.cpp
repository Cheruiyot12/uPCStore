#include "iteminfowidget.h"

ItemInfoWidget::ItemInfoWidget(QStringList *names, QStringList *values, QStringList *units, QWidget *parent) : QWidget(parent)
{
    QScrollArea* mainScroll = new QScrollArea;
    QWidget* scrWid = new QWidget;
    QVBoxLayout* mainLay = new QVBoxLayout(this);
    QGridLayout* scrLay = new QGridLayout(scrWid);

    mainLay->addWidget(mainScroll);
    mainScroll->setWidget(scrWid);
    mainScroll->setWidgetResizable(true);
    if(names->size() == 0 && values->size() == 0 && units->size() == 0){
        QMessageBox::warning(this, "Ошибка", "Информация об этом товаре отсутствует");
        this->close();
    }else{
    for(int i = 0; i < names->size(); ++i){
        scrLay->addWidget(new QLabel("Нaзвание:"),0,0);
        scrLay->addWidget(new QLabel("Значение:"),0,1);
        scrLay->addWidget(new QLabel("Единицы измерения:"),0,2);
        scrLay->addWidget(new QLabel(names->at(i)),i+1,0);
        scrLay->addWidget(new QLabel(values->at(i)),i+1,1);
        scrLay->addWidget(new QLabel(units->at(i)),i+1,2);
    }
    }
    QPushButton* exitButton = new QPushButton("Выход");
    mainLay->addWidget(exitButton);

    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(close()));
}

