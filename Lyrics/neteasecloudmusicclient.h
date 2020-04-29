#ifndef NETEASECLOUDMUSICCLIENT_H
#define NETEASECLOUDMUSICCLIENT_H

#include <QObject>
#include "client.h"

class NeteaseCloudMusicClient : public Client
{
    Q_OBJECT
public:
    explicit NeteaseCloudMusicClient(QObject *parent = nullptr);
    void sendHttpRequest(QString title,bool forceHttp);
private:
    QNetworkAccessManager* networkManager;
    QNetworkAccessManager* networkManager2;
    QString parseSongId(QString doc);
    QString parseSongLyrics(QString doc);
signals:
public slots:
    void songidSlot(QNetworkReply *reply);
    void lyricsSlot(QNetworkReply* reply);
};

#endif // NETEASECLOUDMUSICCLIENT_H
