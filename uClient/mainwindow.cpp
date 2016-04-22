#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* mainWid = new QWidget();
    mainWid->setMinimumSize(640, 480);
    this->setCentralWidget(mainWid);
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

    //ItemInfoWidget* inftest = new ItemInfoWidget(QStringList({"test", "test1", "test2"}), QStringList({"000", "150", "69"}), QStringList({"km", "kg", "GHz"}));

    //inftest->show();

    connect(catBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onGroupIndexChanged(int)));
    connect(mainList, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(onItemClickedToAdd(QListWidgetItem*)));
    connect(compList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showCompContextMenu(QPoint)));
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
    QPoint globalPos = compList->mapToGlobal(p);
    QMenu compMenu;
    compMenu.addAction("Показать информацию", this, SLOT(showInfoAboutItem()));
    compMenu.addAction("Удалить из сборки", this, SLOT(deleteItemFromComp()));

    compMenu.exec(globalPos);
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
