#include "desklyric.h"
#include <QApplication>
#include <QTimer>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QThread>
#include <QDateTime>

#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "
void DeskLyric::setLyricText(QString text)
{
    this->text = (text=="") ? " " : text;
}

DeskLyric::DeskLyric(QWidget *parent) : QWidget(parent)
{
    //去掉右下角三角形,在statusBar把setGridSize的enable属性点击为false即可
    this->setAttribute(Qt::WA_QuitOnClose,false);
    this->orientation=Oritention::HORIZONTAL;
    //设置成无边框,并始终在最上方
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setWindowTitle("desktop lyric");
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setCursor(Qt::OpenHandCursor);
    int deskwidth=QApplication::desktop()->width();
    int deskheight=QApplication::desktop()->height();
    if(orientation==Oritention::HORIZONTAL){
        this->leftTop.setX(100);
        this->leftTop.setY(deskheight-200);
        //函数四个参数: x y w h
        setGeometry(leftTop.x(),leftTop.y(),deskwidth-200,60);
    }else{
        this->leftTop.setX(100);
        this->leftTop.setY(200);
        setGeometry(leftTop.x(),leftTop.y(),60,deskheight-200);
    }
    this->lyricWidth = 0;
    this->duration = 0;
    this->text = "...";
    QTimer * timer= new QTimer();
    timer->setInterval(50);
    connect(timer,&QTimer::timeout,[=](){
        this->update();
    });
    timer->start();
}
void DeskLyric::setDuration(double duration)
{
    this->duration = duration;
    this->lyricWidth = 0;
    //this->duration时间内更新完,50ms更新一次,每次的长度是 this->text->length()/(this->duration/0.05);
    QPainter painter;
    if(duration!=0){
        this->step = lineWidth/(this->duration/0.07);
        //cout<<"texLen:"<<lineWidth<<"duration:"<<duration<<",step:"<<step<<","<<text;
    }
    else
        this->step = 100;
}
void DeskLyric::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        //鼠标点击坐标-窗口相对屏幕坐标
        p = ev->globalPos()-this->frameGeometry().topLeft();
    }
}
void DeskLyric::mouseMoveEvent(QMouseEvent *ev)
{
    this->move(ev->globalPos()-p);
    this->leftTop.setX(frameGeometry().x());
    this->leftTop.setY(frameGeometry().y());
}
void DeskLyric::paintEvent(QPaintEvent *){
    int deskwidth=QApplication::desktop()->width();
    int deskheight=QApplication::desktop()->height();
    if(orientation==Oritention::HORIZONTAL){
        //函数四个参数: x y w h
        setGeometry(leftTop.x(),leftTop.y(),deskwidth-200,60);
    }else{
        setGeometry(leftTop.x(),leftTop.y(),60,deskheight-200);
    }
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QFont font("Times",30,QFont::Bold);
    painter.setFont(font);
    //用指定的颜色设置画刷
    painter.setBrush(QBrush(QColor(29,185,84)));
    //painter.setBrush(QBrush(QColor(187,187,187)));
    painter.setPen(Qt::NoPen);
    QPainterPath textPath;
    //获取当前字体高度
    int h=painter.fontMetrics().height();
    //下面运算为了使得文字居中
    QString dispText = text;
    int x=(this->width()-painter.fontMetrics().horizontalAdvance(dispText))/2;
    int y=(this->height()-h)/2;
    //防止出现负数
    x = (x<0)?0:x;
    y = (y<0)?0:y;
    //为了纵向而修改
    //保存横屏或者纵屏幕最高宽度
    int lineMaxLen;
    if(orientation==Oritention::HORIZONTAL){
        lineMaxLen = this->width();
    }else{
        lineMaxLen = this->height();
    }
    //长度溢出
    //保留前半部分,截取后半部分
    int charLen = painter.fontMetrics().horizontalAdvance("字");
    int lyricLen = painter.fontMetrics().horizontalAdvance(dispText);
    //整句歌词的长度
    int len = painter.fontMetrics().horizontalAdvance(text);
    if(len>100){
        lineWidth = len;
    }
    if(lyricLen > lineMaxLen && lyricWidth < lineMaxLen ){
         int len = lineMaxLen/charLen;
         dispText = dispText.left(len);
         //cout<<"截取前半部分,"<<dispText;
    }
    int passInt = lyricWidth/charLen;
    if(lyricLen > lineMaxLen && lyricWidth > lineMaxLen){
        dispText = text.mid(passInt);
        text = text.mid(passInt);
        lyricWidth = 0;
        //cout<<"截取后半部分."<<dispText;
    }
    //分别对横屏和竖屏绘制两种
    if(orientation==Oritention::HORIZONTAL){
        textPath.addText(x,y+painter.fontMetrics().ascent(),font,dispText);
    }else{
        painter.rotate(90);
        y = (this->height()-painter.fontMetrics().horizontalAdvance(dispText))/2;
        x = -(this->width()+h)/2;
        textPath.addText(y,x+painter.fontMetrics().ascent(),font,dispText);
    }
    //绘制完整的一句歌词
    painter.drawPath(textPath);
    //唱过的歌词显示的颜色
    painter.setPen(QColor(29,185,84));
    lyricWidth += step;
    //绘制唱过的颜色
    //ubuntu block
    /*
    if(orientation==Oritention::HORIZONTAL){
        painter.drawText(x,y,lyricWidth,h,Qt::AlignLeft,dispText);
    }else{
        painter.drawText(y,x,lyricWidth,h,Qt::AlignLeft,dispText);
    }
    */
}
