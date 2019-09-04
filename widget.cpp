#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("数字化时钟器");
    this->setMinimumSize(500,600);
    this->setMaximumSize(500, 600);
    hourPen = QPen(palette().foreground(), 2.0);
    minPen = QPen(palette().foreground(), 1.0);

    QTimer *timer = new QTimer(this);
    timer->start(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer, SIGNAL(timeout()), this, SLOT(Run_Clock()));
    resize(360, 360);

    this->player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("../Clocker/test.mp3"));//工作目录的上级目录下的工程目录
    player->setVolume(100);

    time= QTime::currentTime();
}

Widget::~Widget()
{

}

//初始化变量
const QPoint Widget::hourHand[4]{
    QPoint(3,5),
            QPoint(0,13),
            QPoint(-3, 5),
            QPoint(0, -40)
};

const QPoint Widget::minHand[4]{
    QPoint(3,5),
            QPoint(0,16),
            QPoint(-3, 5),
            QPoint(0, -68)
};
const QPoint Widget::secHand[4]{
    QPoint(3,5),
            QPoint(0,18),
            QPoint(-3, 5),
            QPoint(0, -85)
};

void Widget::Run_Clock()
{
    //时钟走动
    int h = time.hour();
    int m = time.minute();
    int s = time.second();
    s++;
    if(s>=60){
        s = 0;
        m++;
    }
    if(m>=60){
        m = 0;
        h++;
    }
    if(h>=24){
        h = 0;
    }
    time.setHMS(h, m, s);
}
//接口函数
void Widget::paintEvent(QPaintEvent *event)
{
    QFont font;
    font.setPointSize(10);//钟面数字字体大小
    setFont(font);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int min_side = qMin(this->width(), this->height())-20;
    //设置缩放比例
    painter.setViewport((width()-min_side)/2, (height()-min_side)/2, min_side, min_side);
    painter.setWindow(0, 0, 200, 200);
    painter.translate(100, 100);//坐标原点置为（100， 100）
    drawClockDial(&painter);
    drawHourHand(&painter);
    drawMinHand(&painter);
    player->play();
    drawSecHand(&painter);
    //player->stop();



    font.setPointSize(6);//text字体大小
    setFont(font);
    drawTextClock(&painter);

    painter.setBrush(Qt::white);//设置时针的颜色
    painter.drawEllipse(QPoint(0, 0), 2, 2);
}

void Widget::drawHourHand(QPainter *painter)
{
    painter->setBrush(Qt::black);
    painter->setPen(Qt::black);
    painter->save();//保存坐标状态，后面恢复要用
    //1小时30°
    painter->rotate(30.0*(time.hour()+time.minute()/60.0));
    painter->drawConvexPolygon(hourHand, 4);//绘制多边形方式绘制时针
    painter->restore();//restor pointer
}

void Widget::drawMinHand(QPainter *painter)
{
    painter->setBrush(Qt::blue);
    painter->setPen(Qt::blue);
    painter->save();

    painter->rotate(6.0*(time.minute()+time.second()/60.0));
    painter->drawConvexPolygon(minHand, 4);
    painter->restore();
}

void Widget::drawSecHand(QPainter *painter)
{
    painter->setBrush(Qt::red);
    painter->setPen(Qt::red);
    painter->save();

    painter->rotate(6.0*time.second());
    painter->drawConvexPolygon(secHand, 4);
    painter->restore();
}

void Widget::drawClockDial(QPainter *painter)
{
    for(int i = 1; i <= 60; i++)
    {
        painter->save();
        painter->rotate(6*i);//每次偏转6°
        if(i%5 == 0){
            painter->setPen(hourPen);
            painter->drawLine(0, -98, 0, -82);
            painter->drawText(-20, -82, 40, 40, Qt::AlignHCenter|Qt::AlignTop, QString::number(i/5));
        }
        else{
            painter->setPen(minPen);
            painter->drawLine(0, -98, 0, -88);
        }
        painter->restore();
    }
}

void Widget::drawTextClock(QPainter *painter)
{
    QDate date = QDate::currentDate();
    painter->setPen(Qt::blue);
    painter->save();

    painter->translate(0, 0);
    painter->drawText(-100, 100, 200,20,
                      Qt::AlignCenter,
                      QString::number(date.year())+"年"
                      +QString::number(date.month())+"月"
                      +QString::number(date.day())+"日 "
                      +QString::number(time.hour())+"时"
                      +QString::number(time.minute())+"分"
                      +QString::number(time.second())+"秒");
    painter->restore();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if( ((event->pos().rx()>100)&&(event->pos().rx()<this->width()-100))  &&( (event->pos().ry()>100)&&(event->pos().ry()<this->height()-100) ))
    {
        int h = time.hour();
        int m = time.minute();
        int s = time.second();
        h = QInputDialog::getInt(this, "时间设置", "时：", h, 0, 23);
        m = QInputDialog::getInt(this, "时间设置", "分：", m, 0, 59);
        s = QInputDialog::getInt(this, "时间设置", "秒：", h, 0, 59);
        time.setHMS(h, m, s);
    }
    else
    {
        this->time = QTime::currentTime();
    }
}
