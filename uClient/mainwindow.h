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


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
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
private:
    QComboBox* catBox;
    QListWidget* mainList;
    QListWidget* compList;
    ItemInfoWidget* infoWidget;
    QLabel* priceLabel;
private slots:
    void onGroupIndexChanged(int ind);
    void onItemRowChanged(int row);
    void onItemClickedToAdd(QListWidgetItem* li);
    void showCompContextMenu(const QPoint &p);
    void showInfoAboutItem();
    void deleteItemFromComp();
    void sendOrderClicked();
signals:
    void currentGroupChangedSig(int id);
    void currenrItemChangedSig(int id);
    void onItemClickedToAddSig(int id);
    void onItemClickedToDelSig(int id);
    void onShowInfoAboutItemSig(int id);
    void onPlaceOrderSig();
};

#endif // MAINWINDOW_H
