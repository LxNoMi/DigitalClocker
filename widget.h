#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QFont>
#include <QSoundEffect>
#include <QMediaPlayer>
#include <QMouseEvent>
#include <QInputDialog>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *event);
    void drawHourHand(QPainter *painter);
    void drawMinHand(QPainter *painter);
    void drawSecHand(QPainter *painter);
    void drawClockDial(QPainter *painter);
    void drawTextClock(QPainter *painter);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void Run_Clock();

private:
    static const QPoint hourHand[4];
    static const QPoint minHand[4];
    static const QPoint secHand[4];
    QPen hourPen;
    QPen minPen;
    QPen secPen;
    QMediaPlayer * player;
    QTime time;
};

#endif // WIDGET_H
