#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QtDBus>
#include <iostream>
#define cout qDebug().noquote()<<"["<<__LINE__<<"]"
QString MainWindow::getPlaySong()
{
    QDBusInterface iface("org.mpris.MediaPlayer2.spotify",
                         "/org/mpris/MediaPlayer2",
                         "org.freedesktop.DBus.Properties",
                         QDBusConnection::sessionBus());
    if(!iface.isValid()){
        qDebug()<<"接口出错";
        qDebug()<<QDBusConnection::sessionBus().lastError().message();
    }
    QDBusMessage reply = iface.call("Get","org.mpris.MediaPlayer2.Player"
                                    ,"Metadata");
    //未能获取到正确信息
    if(reply.type() != QDBusMessage::ReplyMessage){
        cout<<"未能获取到正确信息";
        return "";
    }
    //cout<<"reply:"<<reply;
    QList<QVariant> outArgs = reply.arguments();
    QDBusVariant first =  outArgs.at(0).value<QDBusVariant>();
    //qDebug()<<"type:"<<first.variant().value<QDBusArgument>().currentType();
    const QDBusArgument arg = first.variant().value<QDBusArgument>();
    QString artist;
    QString title;
    arg.beginMapEntry();
    while(!arg.atEnd()){
        arg.beginMap();
        QString key;
        QVariant value;
        arg>>key>>value;
        if(key=="xesam:title"){
            title = value.toString();
        }
        if(key == "xesam:albumArtist"){
            QStringList list=value.toStringList();
            for(int i=0;i<list.size();i++){
                artist += list.at(i)+",";
            }
        }
        //cout<<key<<","<<value;
        arg.endMapEntry();
    }
    arg.endMap();
    artist = artist.left(artist.size()-1);
    return artist+" - "+title;
}
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    QString title = getPlaySong();
    cout << title;
}
MainWindow::~MainWindow()
{
    delete ui;
}

