#include "Widget.h"
#include "./ui_Widget.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "PillarItem.h"
#include "BirdItem.h"

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    scene = new Scene(this);
    scene->setSceneRect(-250, -270, 500, 540);

    QGraphicsPixmapItem * pixItem = new QGraphicsPixmapItem(QPixmap(":/imgs/sky5.jpg"));
    scene->addItem(pixItem);
    pixItem->setPos(QPointF(0, 0) - QPointF(pixItem->boundingRect().width()/2, pixItem->boundingRect().height()/2));

    // Lines for reference *delete in future*
//    scene->addLine(QLineF(-400, 0, 400, 0), QPen(Qt::green, 2));
//    scene->addLine(QLineF(0, -400, 0, 400), QPen(Qt::green, 2));

//    PillarItem * pillar = new PillarItem();
//    scene->addItem(pillar);


    // Setting up the music
    QMediaPlayer * music = new QMediaPlayer();
    music->setSource(QUrl("qrc:/sounds/bg_2.mp3"));
    QAudioOutput * audio = new QAudioOutput();

    // starts the music
    music->setAudioOutput(audio);
    music->play();

    // loop
    music->setLoops(QMediaPlayer::Infinite);


    ui->graphicsView->setScene(scene);


    scene->addBird();
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    scene->startGame();
}

