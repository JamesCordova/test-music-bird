#include "PillarItem.h"
#include <QRandomGenerator>
#include <QGraphicsScene>
#include <QDebug>

#include "Scene.h"

PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/imgs/pillar.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/imgs/pillar.png"))),
    pastBird(false)
{
    topPillar->setPos(QPointF(0, 0) - QPointF(topPillar->boundingRect().width()/2, topPillar->boundingRect().height() + 70));

    bottomPillar->setPos(QPointF(0, 0) + QPointF(-bottomPillar->boundingRect().width()/2, 70));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

    yPos = QRandomGenerator::global()->bounded(150);
    int xRandomizer = QRandomGenerator::global()->bounded(100);

    setPos(QPointF(0, 0) + QPointF(300 + xRandomizer, yPos));

    xAnimation = new QPropertyAnimation(this, "x", this);
    xAnimation->setStartValue(300 + xRandomizer);
    xAnimation->setEndValue(-300);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(1500);

    connect(xAnimation, &QPropertyAnimation::finished, [=]()
    {
        qDebug() << "PillarItem deleted";
        scene()->removeItem(this);
        delete this;
    });

    xAnimation->start();
}

PillarItem::~PillarItem()
{
    qDebug() << "PillarItem deleted";
    delete topPillar;
    delete bottomPillar;
}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::freezeInPlace()
{
    xAnimation->stop();
}

void PillarItem::pauseAnimation()
{
    xAnimation->pause();
}

void PillarItem::resumeAnimation()
{
    xAnimation->resume();
}

void PillarItem::setX(qreal x)
{
    m_x = x;

    if(x < -250 && !pastBird)
    {
        pastBird = true;
        QGraphicsScene * mscene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mscene);
        if(myScene)
        {
            myScene->increaseScore();
        }
    }

    if(collidesWithBird())
    {
        emit collideFail();
    }
    setPos(QPointF(0, 0) + QPointF(m_x, yPos));
}

bool PillarItem::collidesWithBird()
{
    QList<QGraphicsItem *> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());

    foreach (QGraphicsItem * item, collidingItems)
    {
        BirdItem * birdItem = dynamic_cast<BirdItem *>(item);
        if(birdItem)
        {
            qDebug() << "Colliding item: " << item;
            return true;
        }
    }

    return false;
}
