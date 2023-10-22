#include "GameStart.h"
#include <QGraphicsScene>
#include <QGraphicsLinearLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QGraphicsLayoutItem>

GameStart::GameStart(QObject *parent)
    : QGraphicsScene{parent}
{
    buttons = new QButtonGroup{this};
    buttons->setExclusive(true);
    buttons->addButton(new QPushButton{"Start"});
    buttons->addButton(new QPushButton{"Quit"});
    buttons->addButton(new QPushButton{"About"});
    addItem(buttons);

}

