#include "client.h"
#include <QString>
#include <QtDebug>
#include <QStandardPaths>
#include <QDir>


#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

Client::Client(QObject *parent):QObject(parent)
{
    //缓冲的路径名
    this->appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if(appDataPath == "") return;
    QDir dir;
    if(!dir.exists(appDataPath) && dir.mkdir(appDataPath)){
        cout<<"创建父级目录:"+appDataPath;
    }
    appDataPath += "/lyricText";
    if(!dir.exists(appDataPath) && dir.mkdir(appDataPath)){
        cout<<"创建子级目录:"+appDataPath ;
    }
}
bool Client::readBuffer()
{
    //先查看是否有缓冲文件
    QFile lyricFile(songTitle);
    if(appDataPath!="" && lyricFile.exists()){
        lyricFile.open(QIODevice::ReadOnly);
        QString lyric = lyricFile.readAll();
        lyricFile.close();
        cout<<"file:"<<songTitle<<",已缓冲.";
        //应该需要确保文件没有损坏,之后再修正
        emit sendLyricSignal(lyric);
        return true;
    }
    return false;
}
void Client::removeBuffer()
{
    QFile lyricFile(songTitle);
    if(lyricFile.exists()){
        lyricFile.remove();
    }
    cout<<songTitle<<",已经删除";
}
void Client::takeBuffer(QString lyric)
{
    if(lyric == "") return;
    QFile lyricFile(songTitle);
    if(lyricFile.exists()){
        lyricFile.remove();
    }
    if(lyricFile.open(QIODevice::WriteOnly)){
        lyricFile.write(lyric.toUtf8());
    }
    lyricFile.close();
}
void Client::setSongTitle(QString title)
{
    songTitle = appDataPath+"/"+title+".txt";
}
