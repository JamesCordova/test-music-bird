#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QTimer>

#include "PillarItem.h"
#include "BirdItem.h"

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);

    void addBird();

    void startGame();
    void pauseGame();
    void continueGame();

    bool getGameOn() const;
    void setGameOn(bool newGameOn);

    void increaseScore();

    int getScore() const;
    void setScore(int newScore);

    bool getGamePause() const;
    void setGamePause(bool newGamePause);

signals:

public slots:
protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    void showGameOver();
    void hideGameOver();

    void clearPillars();
    void setUpPillarTimer();
    void freezeBirdAndPillarsInPlace();

    QTimer * pillarTimer;
    PillarItem * pillar;

    qreal remainingTime;

    BirdItem * bird;

    bool gameOn;
    bool gamePause;
    int score;
    int bestScore;

    QGraphicsPixmapItem * gameOverImg;
    QGraphicsTextItem * scoreText;


    // QGraphicsScene interface

};

#endif // SCENE_H
