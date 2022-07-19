#include "applebutton.h"
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QEvent>
#include <QMouseEvent>
#include "setstruct.h"
void AppleButton::paintEvent(QPaintEvent* ev)
{
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);    //解决绘图有锯齿
    painter->setPen(Qt::NoPen);
    QColor m_bgColorOn = QColor("#00CC66");
    QColor m_bgColorOff = QColor("#777777");
    QColor bgColor = m_checked ? m_bgColorOn : m_bgColorOff;
    if (isEnabled()) {
       //bgColor.setAlpha(60);
    }
   painter->setBrush(bgColor);
   QRect rect(0, 0, width(), height());
   int side = qMin(width(), height());
   //左侧半圆
   QPainterPath path1;
   path1.addEllipse(rect.x(), rect.y(), side, side);
   //右侧半圆
   QPainterPath path2;
   path2.addEllipse(rect.width() - side, rect.y(), side, side);

   //中间的矩形
   QPainterPath path3;
   path3.addRect(rect.x() + side / 2, rect.y(), rect.width() - side, height());

   QPainterPath path = path1 + path2 + path3;
   painter->drawPath(path);
   //绘制文本
   //滑块半径
   int m_space = 3; //george added
   QColor m_textColor = QColor("white");
   int sliderWidth = qMin(height(), width()) - m_space * 2 - 5;
   cout<<"current mcheck,"<<m_checked<<",mode"<<mode;
   if (m_checked){
       QRect textRect(0, 0, width() - sliderWidth, height());
       painter->setPen(QPen(m_textColor));
       QString m_textOn =this->m_textOn;
       painter->drawText(textRect, Qt::AlignCenter, m_textOn);
   } else {
       QRect textRect(sliderWidth, 0, width() - sliderWidth, height());
       painter->setPen(QPen(m_textColor));
       QString m_textOff = this->m_textOff;
       painter->drawText(textRect, Qt::AlignCenter, m_textOff);
   }
   drawSlider();
   painter->end();
}
void AppleButton::drawSlider()
{
    QPainter* painter=new QPainter(this);
    painter->setRenderHint(QPainter::Antialiasing);    //解决绘图有锯齿
    //draw the slider
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor("white")));
    int m_space = 2; //george added, the margin which between slider and border
    int sliderWidth = qMin(width(), height()) - m_space * 2;
    int m_startX = m_checked?width()-sliderWidth-m_space*2:0;
    QRect rect(m_space + m_startX, m_space, sliderWidth, sliderWidth);
    painter->drawEllipse(rect);
    painter->end();
}
void AppleButton::mousePressEvent(QMouseEvent* ev)
{
    m_checked = !m_checked;
    cout<<"clicked,"<<this->mode;
    if(this->mode==MODE::LRC_BTN){
        emit deskLrcBtnClick(m_checked);
    }else if(this->mode==MODE::BG_COLOR_BTN){
        emit bgColorBtnClick(m_checked);
    }
    this->update();

}
AppleButton::AppleButton(MODE mode,QWidget *parent) : QPushButton(parent)
{
    qDebug()<<"appleMock pushbutton";
    this->mode = mode;
    this->m_checked = false;
    this->m_textOn=QString("On");
    this->m_textOff=QString("Off");
    SetStruct set;
    if(set.lang=="English"){
        this->m_textOn=QString("On");
        this->m_textOff=QString("Off");
    }
    this->setFixedSize(80,30);
    this->update();
}
