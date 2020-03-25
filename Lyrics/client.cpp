#include "client.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QString>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>
#include <QStandardPaths>
#include <QDir>


#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

Client::Client(QObject *parent):QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    this->connect(networkManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(songidSlot(QNetworkReply*)));
    networkManager2 = new QNetworkAccessManager(this);
    connect(networkManager2,SIGNAL(finished(QNetworkReply*)),this,SLOT(lyricsSlot(QNetworkReply*)));
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

void Client::sendHttpRequest(QString title)
{
    //先查看是否有缓冲文件
    QString lyricFilePath = appDataPath+"/"+title+".txt";
    QFile lyricFile(lyricFilePath);
    if(appDataPath!="" && lyricFile.exists()){
        lyricFile.open(QIODevice::ReadOnly);
        QString lyric = lyricFile.readAll();
        lyricFile.close();
        cout<<"file:"<<title<<",已缓冲.";
        //应该需要确保文件没有损坏,之后再修正
        emit sendLyricSignal(lyric);
    }else{
        QNetworkRequest request = QNetworkRequest(QUrl("http://www.sonydafa.com:3000/search?keywords="+title));
        networkManager->get(request);
    }
}
QString Client::parseSongId(QString doc)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(doc.toUtf8(),&jsonError);
    if(document.isNull() || jsonError.error != QJsonParseError::NoError)
        return "";
    QJsonObject jsonObject = document.object().value("result").toObject();
    if(jsonObject.contains("songs") && jsonObject.value("songs").isArray()){
        QJsonArray array = jsonObject.value("songs").toArray();
        if(array.size()<1)
            return "";
        QJsonObject first = array.at(0).toObject();
        if(first.contains("id")){
            return QString::number(first.value("id").toInt());
        }
    }
    return "";
}
//解析不成功, 返回""
QString Client::parseSongLyrics(QString doc)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(doc.toUtf8(),&jsonError);
    if(document.isNull() || jsonError.error != QJsonParseError::NoError)
        return "";
    QJsonObject jsonObject = document.object().value("lrc").toObject();
    if(jsonObject.contains("lyric")){
        QString lyric=jsonObject.value("lyric").toString();
        return  lyric;
    }
    return "";
}
void Client::lyricsSlot(QNetworkReply* reply)
{
    cout<<"发送信号成功";
    cout << "歌词返回码:"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QString str = QString::fromUtf8(reply->readAll());
    QString lyric = parseSongLyrics(str);
    //进行缓冲
    QString lyricFilePath = appDataPath+"/"+this->songTitle+".txt";
    if(lyric != ""){
        QFile lyricFile(lyricFilePath);
        if(lyricFile.exists()){
            lyricFile.remove();
        }
        if(lyricFile.open(QIODevice::WriteOnly)){
            lyricFile.write(lyric.toUtf8());
        }
        lyricFile.close();
    }
    cout<<this->songTitle<<",本地无缓冲,网络请求";
    //发送歌词信号
    emit sendLyricSignal(lyric);
}
void Client::songidSlot(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();
    QString str = QString::fromUtf8(data);
    cout << "返回码:"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QString songId = parseSongId(str);
    cout << "songId:"<<songId;
    //接收Id成功之后再发送获取歌词的请求
    QNetworkRequest request = QNetworkRequest(QUrl("http://www.sonydafa.com:3000/lyric?id="+songId));
    networkManager2->get(request);
}
