#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QString>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    //发送请求歌曲id的http请求
    virtual void sendHttpRequest(QString title)=0;
    void setSongTitle(QString title){this->songTitle=title;}
protected:
    //检查本地是否有缓冲,有缓冲返回true,同时发送信号;否则返回false
    bool readBuffer();
    //进行缓冲
    void takeBuffer(QString lyric);
    QString songTitle;
    QString appDataPath;
signals:
    //发送歌词信号
    void sendLyricSignal(QString lyric);
};

#endif // CLIENT_H
