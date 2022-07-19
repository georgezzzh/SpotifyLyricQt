#ifndef QQMUSICCLIENT_H
#define QQMUSICCLIENT_H

#include <QObject>
#include "client.h"

class QQMusicClient : public Client
{
    Q_OBJECT
public:
    explicit QQMusicClient(QObject *parent = nullptr);
    void sendHttpRequest(QString title,bool forceHttp);
    void requestQQMusicLyric();
private:
    QNetworkAccessManager* networkManager;
    QNetworkAccessManager* networkManager2;
    QStringList songMidList;
    QStringList parseSongId(QString doc);
    QString parseSongLyrics(QString lyric);
signals:

public slots:
  void songidSlot(QNetworkReply*);
  void lyricSlot(QNetworkReply*);

};

#endif // QQMUSICCLIENT_H
