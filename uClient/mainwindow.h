#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QListWidget>
#include <QGroupBox>
#include <QComboBox>
#include <iteminfowidget.h>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <itemeditwidget.h>
#include <QFileDialog>
#include <QCheckBox>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QCheckBox *ComBox;
    QAction* chAct;
public slots:
    void showErr(QString text);

    void onNewGroup(QString name);
    void clearGroups();

    void onNewItem(QString name);
    void clearItems();
    void onItemToComp(QString name, int id);
    void onItemDelComp(int id);
    void procInfoAboutItem(QStringList *st1, QStringList *st2, QStringList *st3);
    void setPrice(double price);
    void activateSellerMode();
    void showEditWidget(int nid, QString nname, float nprice, int ncount, openMode mode);
    void onLoadChrsReq();
    void loadChrsToCurrWid(QList<itemChars>*);
    void loadChNamsToCurrWid(QList<chars> *chr);
    void onSavReq(openMode sMode, int nnid, QString nnme, float nprce, QList<itemChars> *nchrs);
    void activateAdminMode();
    void openUserEdit();
    void openOrderMenu();
private:
    QComboBox* catBox;
    QListWidget* mainList;
    QListWidget* compList;
    ItemInfoWidget* infoWidget;
    QLabel* priceLabel;
    ItemEditWidget* editWidget = nullptr;
    QMenu* adminMenu;

private slots:
    void onGroupIndexChanged(int ind);
    void onItemRowChanged(int row);
    void onItemClickedToAdd(QListWidgetItem* li);
    void showCompContextMenu(const QPoint &p);
    void showInfoAboutItem();
    void deleteItemFromComp();
    void sendOrderClicked();
    void requestPermissionsClicked();
    void showMainContextMenu(const QPoint &p);
    void addItemSel();
    void delItemSel();
    void modItemSel();
    void onCrePrice();

signals:
    void currentGroupChangedSig(int id);
    void currenrItemChangedSig(int id);
    void onItemClickedToAddSig(int id);
    void onItemClickedToDelSig(int id);
    void onShowInfoAboutItemSig(int id);
    void onPlaceOrderSig();
    void onRequestPermissionsSig();
    void selectItemToMod(int num);
    void onLoadChrsReqSig();
    void onSaveReqSig(openMode sMode, int nnid, QString nnme, float nprce, QList<itemChars>* nchrs);
    void deleteSelectedItem(int ind);
    void openUserEditSig();
    void crePriceSig(QString);
    void openOrderMenuSig();
};

#endif // MAINWINDOW_H
