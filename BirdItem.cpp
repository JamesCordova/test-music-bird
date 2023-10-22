#include "BirdItem.h"
#include <QTimer>
#include <QGraphicsScene>

BirdItem::BirdItem(QPixmap pixmap) :
    wingPosition(WingPosition::Up),
    wingDirection(false)
{
    setPixmap(pixmap);

    QTimer * birdWingTimer = new QTimer(this);
    connect(birdWingTimer, &QTimer::timeout, [=]()
    {
        updatePixmap();
    });

    birdWingTimer->start(80);

    groundPosition = scenePos().y() + 250;
    setPos(-250, y());

    yAnimation = new QPropertyAnimation(this, "y", this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

    // yAnimation->start(); // void startFlying()

    rotationAnimation = new QPropertyAnimation(this, "rotation", this);
    // rotateTo(90, 1000, QEasingCurve::InQuad); // void startFlying()

    /* rotationAnimation->setStartValue(y());
    rotationAnimation->setEndValue(90);
    rotationAnimation->setEasingCurve(QEasingCurve::InQuad);
    rotationAnimation->setDuration(400);

    rotationAnimation->start(); */

}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();

}

void BirdItem::fallToGroundIfNecessary()
{
    if(y() < groundPosition)
    {
        rotationAnimation->stop();
        // yAnimation->stop();

        yAnimation->setStartValue(y());
        yAnimation->setEndValue(groundPosition);
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setDuration(1000);

        yAnimation->start();

        rotateTo(90, 1000, QEasingCurve::InQuad); // InCubic
    }
}

void BirdItem::updatePixmap()
{
    if(wingPosition == WingPosition::Middle)
    {
        if(wingDirection == true)
        {
            // UP
            setPixmap(QPixmap(":/imgs/bird_up.png"));
            wingPosition = WingPosition::Up;
            wingDirection = false;
        }
        else
        {
            // DOWN
            setPixmap(QPixmap(":/imgs/bird_down.png"));
            wingPosition = WingPosition::Down;
            wingDirection = true;
        }
    }
    else
    {
        // MIDDLE
        setPixmap(QPixmap(":/imgs/bird_middle.png"));
        wingPosition = WingPosition::Middle;
    }
//    setTransformOriginPoint(boundingRect().center());
//    setRotation(rotation() + 5);

}

qreal BirdItem::rotation() const
{
    return m_rotation;
}

void BirdItem::setRotation(qreal rotation)
{
    m_rotation = rotation;
    QPointF c = boundingRect().center();

    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(m_rotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);

}

qreal BirdItem::y() const
{
    return m_y;
}

void BirdItem::shootUp()
{
    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();

    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->height()/10);
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(300);

    connect(yAnimation, &QPropertyAnimation::finished, [=]()
    {
        fallToGroundIfNecessary();
    });

    yAnimation->start();

    rotateTo(-20,200,QEasingCurve::OutCubic);

}

void BirdItem::startFlying()
{
    setPos(QPointF(-250, 0));
    yAnimation->start();
    rotateTo(90, 1000, QEasingCurve::InQuad);

}

void BirdItem::pauseFlying()
{
    yAnimation->pause();
    rotationAnimation->pause();
}

void BirdItem::resumeFlying()
{
    yAnimation->resume();
    rotationAnimation->resume();
}

void BirdItem::freezeInPlace()
{
    yAnimation->stop();
    rotationAnimation->stop();
}

void BirdItem::setY(qreal y)
{
   // moveBy(0, y - m_y);
    m_y = y;
    setPos(QPointF(-250, 0) + QPointF(0, m_y));
}

