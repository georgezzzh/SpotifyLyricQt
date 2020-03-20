#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QString>
#include <QThread>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    QString parseSongId(QString doc);
    QString parseSongLyrics(QString doc);
    //void setUi(Ui::MainWindow* ui) {this->uu = ui;}
    //发送请求歌曲id的http请求
    void sendHttpRequest(QString title);
    void setSongTitle(QString title){this->songTitle=title;}
private:
    QNetworkAccessManager* networkManager;
    QNetworkAccessManager* networkManager2;
    QString songTitle;
    QString appDataPath;
signals:
    //发送歌词信号
    void sendLyricSignal(QString lyric);
public slots:
    void songidSlot(QNetworkReply *reply);
    void lyricsSlot(QNetworkReply* reply);
};

#endif // CLIENT_H
