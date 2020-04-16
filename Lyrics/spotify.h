#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QString>
#include <QThread>

class Spotify :public QThread
{
    Q_OBJECT
    public:
        explicit Spotify(QObject* parent = nullptr);
        QString getSongName();
        static QString song;
        void setRunning(bool flag){ this->running=flag; }
    protected:
        void run() override;
    private:
        bool running;
    signals:
        void threadOver(QString);

};

#endif // SPOTIFY_H
