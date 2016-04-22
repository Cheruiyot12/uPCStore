#include "computer.h"

Computer::Computer(QObject *parent) : QObject(parent)
{

}

QJsonObject Computer::toJson()
{
    //TODO
}

void Computer::addHardware(Item *itm)
{
    hardware.append(itm);
}

void Computer::delHardware(Item *itm)
{
    hardware.removeOne(itm);
}

void Computer::delHardware(int id)
{
    for (int i = 0; i < hardware.size(); i++){
        if(hardware.at(i)->getId() == id){
            hardware.removeOne(hardware.at(i));
            break;
        }
    }
}

/*void Computer::checkCompability(Item *itm)
{
    //TODO
}*/
