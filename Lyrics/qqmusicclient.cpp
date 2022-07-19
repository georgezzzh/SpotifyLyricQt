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
        //2022-07-19 更新
        //QString url="https://c.y.qq.com/splcloud/fcgi-bin/smartbox_new.fcg?key="+title
        //QString url = "https://c.y.qq.com/soso/fcgi-bin/client_search_cp?p=1&n=2&w="+title+"&format=json";
        QString url="https://c.y.qq.com/splcloud/fcgi-bin/smartbox_new.fcg?key="+title;
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        networkManager->get(request);
    }
}

QStringList QQMusicClient::parseSongId(QString doc)
{
    cout<<"recevie messgae:"<<doc;
    QJsonParseError jsonError;
    QStringList list;
    QJsonDocument document = QJsonDocument::fromJson(doc.toUtf8(),&jsonError);
    if(doc.isNull()||jsonError.error != QJsonParseError::NoError)
        return list;
    QJsonArray jsonArray = document.object().value("data").toObject()
            .value("song").toObject().value("itemlist").toArray();
    if(jsonArray.size()<1) return list;
    for(int i=0;i<jsonArray.size();i++)
        list<<jsonArray.at(i).toObject().value("mid").toString();
    return list;
}
QString QQMusicClient::parseSongLyrics(QString lyric)
{
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(lyric.toUtf8(),&jsonError);
    if(lyric.isNull()||jsonError.error != QJsonParseError::NoError)
        return "";
    return document.object().value("lyric").toString();
}
void QQMusicClient::requestQQMusicLyric()
{
    //未查询到歌曲
    if(this->songMidList.size()<1){
        emit sendLyricSignal("");
        return;
    }
    QString songmid = songMidList.at(0);
    songMidList.removeFirst();
    QString url ="https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid="+songmid+"&format=json&nobase64=1";
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Referer","https://y.qq.com/portal/player.html");
    networkManager2->get(request);
}
void QQMusicClient::lyricSlot(QNetworkReply* reply)
{
    QString lyric = parseSongLyrics(QString(reply->readAll()));
    //需要对歌词中的信息进行转义
    cout<<"parse lyric:"<<lyric;
    if((lyric=="" || lyric.contains("此歌曲为没有填词的纯音乐")) && this->songMidList.size()>0){
        cout<<"again visit webserver1";
        requestQQMusicLyric();
    }else{
        if(lyric!=""){
            lyric.replace("&apos;","'");
            //进行缓冲
            takeBuffer(lyric);
            cout<<"QQ音乐进行网络请求,"<<songTitle;
        }
        //发送歌词信号
        emit sendLyricSignal(lyric);
    }

}
void QQMusicClient::songidSlot(QNetworkReply* reply){
    QStringList songmids = parseSongId(QString(reply->readAll()));
    //赋值给成员变量
    //这里不能只依赖一个mid, 把所有mid都拿出来找
    this->songMidList = songmids;
    cout<<"songmidList:"<<songmids;
    //未查询到歌曲
    if(songmids.size()<1){
        emit sendLyricSignal("");
        return;
    }
    QString songmid = songMidList.at(0);
    songMidList.removeFirst();
    QString url ="https://c.y.qq.com/lyric/fcgi-bin/fcg_query_lyric_new.fcg?songmid="+songmid+"&format=json&nobase64=1";
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    request.setRawHeader("Referer","https://y.qq.com/portal/player.html");
    networkManager2->get(request);
}
