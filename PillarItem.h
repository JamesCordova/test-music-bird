#ifndef PILLARITEM_H
#define PILLARITEM_H

#include <QGraphicsItemGroup>
#include <QGraphicsPixmapItem>
#include <QPropertyAnimation>

class PillarItem : public QObject , public QGraphicsItemGroup
{
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX)
public:
    explicit PillarItem();
    ~PillarItem();

    qreal x() const;

    void freezeInPlace();
    void pauseAnimation();
    void resumeAnimation();

signals:
    void collideFail();

public slots:
    void setX(qreal x);

private:

    bool collidesWithBird();

    QGraphicsPixmapItem * topPillar;
    QGraphicsPixmapItem * bottomPillar;
    QPropertyAnimation * xAnimation;

    qreal m_x;
    int yPos;

    bool pastBird;

};

#endif // PILLARITEM_H
