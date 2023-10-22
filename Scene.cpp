#include "Scene.h"
#include "PillarItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent), gameOn(false), gamePause(false), score(0), bestScore(0), remainingTime(1000)
{
    setUpPillarTimer();


}

void Scene::addBird()
{
    bird = new BirdItem(QPixmap(":/imgs/bird_middle.png"));
    addItem(bird);
}

void Scene::startGame()
{
    // Bird
    bird->startFlying();
    // Pillars
    if(!pillarTimer->isActive())
    {
        clearPillars();
        setGameOn(true);
        hideGameOver();
        pillarTimer->start(1000);
    }

}

void Scene::pauseGame()
{
    bird->pauseFlying();
    // get remaining time and pause timer to get in future resume game
    QList<QGraphicsItem *> allItems = items();
    for(QGraphicsItem * item : allItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar)
        {
            pillar->pauseAnimation();
        }
    }
    remainingTime = pillarTimer->remainingTime();
    qDebug() << "Remaining time: " << remainingTime;
    pillarTimer->stop();

    setGamePause(true);
}

void Scene::continueGame()
{
    bird->resumeFlying();
    QList<QGraphicsItem *> allItems = items();
    for(QGraphicsItem * item : allItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar)
        {
            pillar->resumeAnimation();
        }
    }

    if(remainingTime <= 0)
    {
        remainingTime = 1000;
    }
    pillarTimer->start(1000);
    setGamePause(false);
}

void Scene::setUpPillarTimer()
{
    pillarTimer = new QTimer(this);
    connect(pillarTimer, &QTimer::timeout, [=]()
    {
        pillar = new PillarItem();
        connect(pillar, &PillarItem::collideFail, [=]()
        {
            pillarTimer->stop();
            freezeBirdAndPillarsInPlace();
            setGameOn(false);
            showGameOver();
            setScore(0);
        });
        addItem(pillar);
    });
    // pillarTimer->start(1000);  // void startGame()
}

void Scene::freezeBirdAndPillarsInPlace()
{
//  Freeze bird
    bird->freezeInPlace();

//  Freeze pillars
    QList<QGraphicsItem *> allItems = items();
    for(QGraphicsItem * item : allItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar)
        {
            pillar->freezeInPlace();
        }
    }

}

bool Scene::getGamePause() const
{
    return gamePause;
}

void Scene::setGamePause(bool newGamePause)
{
    gamePause = newGamePause;
}

int Scene::getScore() const
{
    return score;
}

void Scene::setScore(int newScore)
{
    score = newScore;
}

bool Scene::getGameOn() const
{
    return gameOn;
}

void Scene::setGameOn(bool newGameOn)
{
    gameOn = newGameOn;
}

void Scene::increaseScore()
{
    score++;
    if(score > bestScore)
    {
        bestScore = score;
    }
    qDebug() << "Score: " << score;
    qDebug() << "Best score: " << bestScore;
}

void Scene::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        if(gameOn && !gamePause)
        {
            bird->shootUp();
        }
        else if(gamePause)
        {
            continueGame();
        }
        else
        {
            startGame();
        }
    }
    // press scape button
    if(event->key() == Qt::Key_Escape)
    {
        if(!gamePause && gameOn)
        {
            pauseGame();
        }
        else if(gameOn)
        {
            continueGame();
        }
        else
        {
            startGame();
        }
    }
    QGraphicsScene::keyPressEvent(event);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(gameOn && !gamePause)
        {
            bird->shootUp();
        }
        else if(gamePause)
        {
            continueGame();
        }
        else
        {
            startGame();
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void Scene::showGameOver()
{
    gameOverImg = new QGraphicsPixmapItem(QPixmap(":/imgs/game_over.png"));
    addItem(gameOverImg);
    gameOverImg->setPos(QPointF(0, 0) - QPointF(gameOverImg->boundingRect().width()/2, gameOverImg->boundingRect().height()/2 + 50));

    scoreText = new QGraphicsTextItem(QString("Score: ") + QString::number(score));

    QString htmlString = "<p style=\"color:#000000; font-size: 24px\"> SCORE: "
                         + QString::number(score) + "</p>" + "<p style=\"color:#000000; font-size: 24px;\"> BEST SCORE: "
                         + QString::number(bestScore) + "</p>";

    QFont font("Cascadia Code", 24, QFont::Bold);

    scoreText->setHtml(htmlString);
    scoreText->setFont(font);

    addItem(scoreText);

    scoreText->setPos(QPointF(0, 0) - QPointF(scoreText->boundingRect().width()/2, scoreText->boundingRect().height()/2) + QPointF(0, 100));
    setGameOn(false);

}
void Scene::hideGameOver()
{
    if(gameOverImg)
    {
        this->removeItem(gameOverImg);
//        delete gameOverImg;
        gameOverImg = nullptr;
    }
    if(scoreText)
    {
        this->removeItem(scoreText);
//        delete scoreText;
        scoreText = nullptr;
    }
    setGameOn(true);
}

void Scene::clearPillars()
{
    QList<QGraphicsItem *> allItems = items();
    for(QGraphicsItem * item : allItems)
    {
        PillarItem * pillar = dynamic_cast<PillarItem *>(item);
        if(pillar)
        {
            removeItem(pillar);
            delete pillar;
        }
    }
}
