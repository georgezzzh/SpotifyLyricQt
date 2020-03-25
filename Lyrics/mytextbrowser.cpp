#include "mytextbrowser.h"
#include <QWheelEvent>

MyTextBrowser::MyTextBrowser(QWidget *parent) : QTextBrowser(parent)
{

}
void MyTextBrowser::wheelEvent(QWheelEvent *ev)
{
    QTextBrowser::wheelEvent(ev);
    //发送信号
    emit myWhellScroll();
}
