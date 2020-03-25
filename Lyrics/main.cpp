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
    MainWindow w;
    w.setWindowTitle("Spotify Lyric Qt");
    //设置图标
    w.setWindowIcon(QIcon("://icon/main.ico"));
    w.resize(800,600);
    w.show();
    QLocale locale;
    if(locale.language()==QLocale::Chinese){
        cout<<"汉语";
    }
    else if(locale.language()==QLocale::English){
        cout<<"英语";
    }
    return a.exec();
}
