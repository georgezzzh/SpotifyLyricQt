#include "neteasecloudmusicclient.h"
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

#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "
NeteaseCloudMusicClient::NeteaseCloudMusicClient(QObject *parent) : Client(parent)
{
    networkManager = new QNetworkAccessManager(this);
    this->connect(networkManager,SIGNAL(finished(QNetworkReply*)),this, SLOT(songidSlot(QNetworkReply*)));
    networkManager2 = new QNetworkAccessManager(this);
    connect(networkManager2,SIGNAL(finished(QNetworkReply*)),this,SLOT(lyricsSlot(QNetworkReply*)));
}

void NeteaseCloudMusicClient::sendHttpRequest(QString title)
{
    //无缓冲时进行网络请求
    if(!readBuffer()){
        QNetworkRequest request = QNetworkRequest(QUrl("http://www.sonydafa.com:3000/search?keywords="+title));
        networkManager->get(request);
    }
}
QString NeteaseCloudMusicClient::parseSongId(QString doc)
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
QString NeteaseCloudMusicClient::parseSongLyrics(QString doc)
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
void NeteaseCloudMusicClient::lyricsSlot(QNetworkReply* reply)
{
    //cout << "歌词返回码:"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QString str = QString::fromUtf8(reply->readAll());
    QString lyric = parseSongLyrics(str);
    //进行缓冲
    takeBuffer(lyric);
    cout<<"网易云网络请求,"<<songTitle;
    //发送歌词信号
    emit sendLyricSignal(lyric);
}
void NeteaseCloudMusicClient::songidSlot(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();
    QString str = QString::fromUtf8(data);
    //cout << "返回码:"<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).value<int>();
    QString songId = parseSongId(str);
    cout << "songId:"<<songId;
    if(songId==""){
        emit sendLyricSignal("");
        return;
    }
    //接收Id成功之后再发送获取歌词的请求
    QNetworkRequest request = QNetworkRequest(QUrl("http://www.sonydafa.com:3000/lyric?id="+songId));
    networkManager2->get(request);
}
