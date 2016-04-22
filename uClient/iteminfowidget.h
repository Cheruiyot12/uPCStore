#ifndef ITEMINFOWIDGET_H
#define ITEMINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStringList>
#include <QMessageBox>

class ItemInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ItemInfoWidget(QStringList* names, QStringList* values, QStringList* units, QWidget *parent = 0);

signals:

public slots:
private:

};

#endif // ITEMINFOWIDGET_H
