#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{

}
void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    emit clicked();
}
