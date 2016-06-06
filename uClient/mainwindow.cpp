#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* mainWid = new QWidget();
    mainWid->setMinimumSize(640, 480);
    this->setCentralWidget(mainWid);
    this->setWindowTitle("Computer configurator by Alkor Shikyaro");
    QGridLayout* mainLay = new QGridLayout();
    mainWid->setLayout(mainLay);

    QGridLayout* listLay = new QGridLayout();
    mainLay->addLayout(listLay, 0, 0);
    listLay->addWidget(new QLabel("Выберите из списка категорию товара, которую требуется добавить к сборке:"), 0, 0);

    catBox = new QComboBox();
    listLay->addWidget(catBox, 1, 0);

    mainList = new QListWidget();
    listLay->addWidget(mainList, 2, 0);


    QVBoxLayout* finLay = new QVBoxLayout();
    mainLay->addLayout(finLay,0,1);
    QGroupBox* finBox = new QGroupBox("Оценка");
    finLay->addWidget(finBox);
    QGroupBox* confBox = new QGroupBox("Конфигурация");
    finLay->addWidget(confBox);
    finBox->setMinimumWidth(150);

    compList = new QListWidget();
    QVBoxLayout* clay = new QVBoxLayout();
    confBox->setLayout(clay);
    clay->addWidget(compList);

    compList->setContextMenuPolicy(Qt::CustomContextMenu);

    QGridLayout *flay = new QGridLayout(finBox);
    flay->addWidget(new QLabel("Цена:"),0,0);
    priceLabel = new QLabel("0");
    flay->addWidget(priceLabel,0,1);

    //ItemInfoWidget* inftest = new ItemInfoWidget(QStringList({"test", "test1", "test2"}), QStringList({"000", "150", "69"}), QStringList({"km", "kg", "GHz"}));

    //inftest->show();
    QPushButton* sendOrder = new QPushButton("Отправить заказ");
    flay->addWidget(sendOrder,1,0);

    QPushButton *crePrice = new QPushButton("Создать отчет");
    flay->addWidget(crePrice);
    connect(crePrice, SIGNAL(clicked(bool)), this, SLOT(onCrePrice()));

    QMenuBar* mainMenuBar = new QMenuBar();
    mainLay->setMenuBar(mainMenuBar);

    QMenu* mainMenu = new QMenu("Главное", mainMenuBar);
    mainMenuBar->addMenu(mainMenu);
    QAction* ext = new QAction("Выход", mainMenu);
    mainMenu->addAction(ext);
    connect(ext, SIGNAL(triggered(bool)), this, SLOT(close()));

    adminMenu = new QMenu("Управление", mainMenuBar);
    mainMenuBar->addMenu(adminMenu);
    adminMenu->addAction("Запросить права", this, SLOT(requestPermissionsClicked()));

    QMenu* orderMenu = new QMenu("Заказы", mainMenuBar);
    mainMenuBar->addMenu(orderMenu);
    orderMenu->addAction("Список заказов", this, SLOT(openOrderMenu()));

    QMenu* aboutMenu = new QMenu("О программе", mainMenuBar);
    mainMenuBar->addMenu(aboutMenu);


    connect(catBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGroupIndexChanged(int)));
    connect(mainList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClickedToAdd(QListWidgetItem*)));
    connect(compList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCompContextMenu(QPoint)));
    connect(sendOrder, SIGNAL(clicked(bool)), this, SLOT(sendOrderClicked()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::showErr(QString text)
{
    QMessageBox::warning(this, "eRRoR", text);
}

void MainWindow::onNewGroup(QString name)
{
    catBox->addItem(name);
}

void MainWindow::clearGroups()
{
    catBox->clear();
}

void MainWindow::onGroupIndexChanged(int ind)
{
    if(ind != -1)
        emit this->currentGroupChangedSig(ind);
}

void MainWindow::onNewItem(QString name)
{
    mainList->addItem(new QListWidgetItem(name));
}

void MainWindow::clearItems()
{
    mainList->clear();
}

void MainWindow::onItemRowChanged(int row)
{
    if(row != -1)
        emit this->currenrItemChangedSig(row);
}

void MainWindow::onItemToComp(QString name, int id)
{
    QListWidgetItem* itm = new QListWidgetItem(name);
    itm->setData(1000, id);
    compList->addItem(itm);
}

void MainWindow::onItemDelComp(int id)
{
    for(int i = 0; i < compList->count(); i++){
        if(compList->item(i)->data(1000).toInt() == id){
            //compList->removeItemWidget();
            delete compList->item(i);
            break;
        }
    }
}

void MainWindow::onItemClickedToAdd(QListWidgetItem *li)
{
    emit this->onItemClickedToAddSig(mainList->row(li));
}

void MainWindow::showCompContextMenu(const QPoint &p)
{
    if(!compList->selectedItems().empty()){
        QPoint globalPos = compList->mapToGlobal(p);
        QMenu compMenu;
        compMenu.addAction("Показать информацию", this, SLOT(showInfoAboutItem()));
        compMenu.addAction("Удалить из сборки", this, SLOT(deleteItemFromComp()));

        compMenu.exec(globalPos);
    }
}

void MainWindow::showInfoAboutItem()
{
    emit this->onShowInfoAboutItemSig(compList->selectedItems().at(0)->data(1000).toInt());
}

void MainWindow::procInfoAboutItem(QStringList* st1, QStringList* st2, QStringList* st3)
{
    ItemInfoWidget* inf = new ItemInfoWidget(st1, st2, st3);
    inf->setAttribute(Qt::WA_DeleteOnClose);
    inf->setWindowFlags(Qt::Dialog);
    inf->setWindowModality(Qt::WindowModal);
    inf->show();
}

void MainWindow::deleteItemFromComp()
{
    //this->onItemDelComp(compList->selectedItems().at(0)->data(1000));
    //for(int i = 0; i < compList->selectedItems().size(); i++){
    emit this->onItemClickedToDelSig(compList->selectedItems().at(0)->data(1000).toInt());
    //}
}

void MainWindow::setPrice(double price)
{
    //qDebug() << price << "_-------------------------------------";
    priceLabel->setText(QString("%1 $").arg(price));
}

void MainWindow::sendOrderClicked()
{
    QMessageBox warnMess;
    //warnMess.setText("Внимание");
    warnMess.setWindowTitle("Подтверждение");
    warnMess.setText("Вы хотите отправить заказ?\nПосле отправки содержимое заказа нельзя будет изменить.");
    warnMess.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    warnMess.setMinimumWidth(400);
    //Unity3d is missing you
    int ret = warnMess.exec();

    if(QMessageBox::Ok == ret){
        emit this->onPlaceOrderSig();
    }
}

void MainWindow::requestPermissionsClicked()
{
    emit this->onRequestPermissionsSig();
}

void MainWindow::activateSellerMode()
{
    mainList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mainList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showMainContextMenu(QPoint)));
}

void MainWindow::showMainContextMenu(const QPoint &p)
{
    QPoint globalPos = mainList->mapToGlobal(p);
    QMenu compMenu;
    compMenu.addAction("Добавить товар", this, SLOT(addItemSel()));
    if(!mainList->selectedItems().empty()){

        compMenu.addAction("Изменить товар", this, SLOT(modItemSel()));
        compMenu.addAction("Удалить товар", this, SLOT(delItemSel()));
    }

    compMenu.exec(globalPos);
}

void MainWindow::addItemSel()
{
    emit this->selectItemToMod(-1);
}

void MainWindow::modItemSel()
{
    if(!mainList->selectedItems().empty())
        emit this->selectItemToMod(mainList->row(mainList->selectedItems().at(0)));
}

void MainWindow::delItemSel()
{
    emit this->deleteSelectedItem(mainList->row(mainList->selectedItems().at(0)));

}

void MainWindow::showEditWidget(int nid, QString nname, float nprice, int ncount, openMode mode)
{
    if(editWidget != nullptr)
        delete editWidget;
    ItemEditWidget* ed = new ItemEditWidget(nid, nname, nprice, ncount, mode, this);
    connect(ed, SIGNAL(getChrs()), this, SLOT(onLoadChrsReq()));
    connect(ed, SIGNAL(onSaveSig(openMode,int,QString,float,QList<itemChars>*)),
            this, SLOT(onSavReq(openMode,int,QString,float,QList<itemChars>*)));
    //ed->setAttribute(Qt::WA_DeleteOnClose);
    ed->setWindowFlags(Qt::Dialog);
    ed->setWindowModality(Qt::WindowModal);
    editWidget = ed;
    ed->show();
    ed->onLoad();


}

void MainWindow::onLoadChrsReq()
{
    emit this->onLoadChrsReqSig();
}

void MainWindow::loadChrsToCurrWid(QList<itemChars> *chr)
{
    editWidget->loadChrs(chr);
}

void MainWindow::loadChNamsToCurrWid(QList<chars >*chr)
{
    editWidget->loadChrsNam(chr);
}

void MainWindow::onSavReq(openMode sMode, int nnid, QString nnme, float nprce, QList<itemChars>* nchrs)
{
    emit this->onSaveReqSig(sMode, nnid, nnme, nprce, nchrs);
}

void MainWindow::activateAdminMode()
{
    adminMenu->addAction("Управление пользователями", this, SLOT(openUserEdit()));
}

void MainWindow::openUserEdit()
{
    emit this->openUserEditSig();
}

void MainWindow::onCrePrice()
{
    if(!compList->count()==0){
        QString filepath = QFileDialog::getExistingDirectory(this, "Выберите папку для сохранения", "C:\\");
        emit this->crePriceSig(filepath);
    }
}

void MainWindow::openOrderMenu()
{
    emit this->openOrderMenuSig();
}
