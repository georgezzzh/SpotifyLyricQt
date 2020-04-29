#include "qqmusicclient.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

QQMusicClient::QQMusicClient(QObject *parent) : Client(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(songidSlot(QNetworkReply*)));
    networkManager2 = new QNetworkAccessManager(this);
    connect(networkManager2,SIGNAL(finished(QNetworkReply*)),this,SLOT(lyricSlot(QNetworkReply*)));
}
void QQMusicClient::sendHttpRequest(QString title,bool forceHttp)
{
    if(forceHttp){
        removeBuffer();
    }
    if(!readBuffer()){
        QString url = "https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w="+title+"&format=json";
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        networkManager->get(request);
    }
}

QString QQMusicClient::parseSongId(QString doc)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(doc.toUtf8(),&jsonError);
    if(doc.isNull()||jsonError.error != QJsonParseError::NoError)
        return "";
    QJsonArray jsonArray = document.object().value("data").toObject()
            .value("song").toObject().value("list").toArray();
    if(jsonArray.size()<1) return "";
    return jsonArray.at(0).toObject().value("songmid").toString();
}
QString QQMusicClient::parseSongLyrics(QString lyric)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(lyric.toUtf8(),&jsonError);
    if(lyric.isNull()||jsonError.error != QJsonParseError::NoError)
        return "";
    return document.object().value("lyric").toString();
}
void QQMusicClient::lyricSlot(QNetworkReply* reply)
{
    QString lyric = parseSongLyrics(QString(reply->readAll()));
    //需要对歌词中的信息进行转义
    lyric.replace("&apos;","'");
    //进行缓冲
    takeBuffer(lyric);
    cout<<"QQ音乐进行网络请求,"<<songTitle;
    //发送歌词信号
    emit sendLyricSignal(lyric);
}
void QQMusicClient::songidSlot(QNetworkReply* reply){
    QString songmid = parseSongId(QString(reply->readAll()));
    cout <<"songmid:"<<songmid;
    //未查询到歌曲
    if(songmid==""){
        emit sendLyricSignal("");
        return;
    }
    QString url ="https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid="+songmid+"&format=json&nobase64=1";
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Referer","https://y.qq.com/portal/player.html");
    networkManager2->get(request);
}
