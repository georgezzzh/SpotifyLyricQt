#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTranslator>
#include "spotify.h"
#include "client.h"
#include "neteasecloudmusicclient.h"
#include "qqmusicclient.h"
#include "settings.h"
#include "desklyric.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

typedef struct lyricPhrase{
    double time;
    QString lyric;
    bool highLight;
} lyricPhrase;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    //根据时间重新调整歌词的锚点
    void resetLyricDisplay();
    void configChange();
    void langInit();
private:
    Ui::MainWindow *ui;
    QTranslator* trans;
    Settings* set;
    SetStruct* setStruct;
    Spotify * sp;
    Client* client;
    QQMusicClient* qqClient;
    NeteaseCloudMusicClient* neteaseClient;
    DeskLyric* deskLyric;
    //保存歌曲名
    QString songFlag;
    //把歌词存在list中
    QList<lyricPhrase> list;
    int songBeginTime;
    //用于anchor，滚动所用
    int lastIndex;
    //检测鼠标是否滚动
    bool wheelScroll;
    //检查是否切换过客户端
    bool switchClient;
public slots:
    //获取到歌词,调用的槽函数
    void dealLyricSlot(QString lyric);
    //歌曲名变化,调用槽函数
    void dealSongName(QString name);

};
#endif // MAINWINDOW_H
