#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QLocale>
#include "client.h"
#include "spotify.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //关闭主窗口时,全部关闭
    a.connect(&a,&QApplication::lastWindowClosed,&a,&QApplication::closeAllWindows);
    MainWindow w;
    w.setWindowTitle("Spotify Lyric Qt");
    //设置图标
    w.setWindowIcon(QIcon("://icon/exeIcon.ico"));
    w.resize(800,600);
    w.show();
    return a.exec();
}
