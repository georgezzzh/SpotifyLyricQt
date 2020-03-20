#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDebug>
#include "client.h"
#include "spotify.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Spotify Lyric Qt");
    //设置图标
    w.setWindowIcon(QIcon("../icon.ico"));
    w.resize(800,600);
    w.show();
    return a.exec();
}
