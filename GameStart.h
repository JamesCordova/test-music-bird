#ifndef GAMESTART_H
#define GAMESTART_H

#include <QGraphicsScene>
#include <QButtonGroup>

class GameStart : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameStart(QObject *parent = nullptr);

signals:

private:
    QButtonGroup * buttons;

};

#endif // GAMESTART_H
