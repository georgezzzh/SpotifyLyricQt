#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QTranslator>
#include "spotify.h"
#include "client.h"
#include "settings.h"
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
    void configInit();
private:
    Ui::MainWindow *ui;
    QTranslator* trans;
    Settings* set;
    SetStruct* setStruct;
    Spotify * sp;
    Client* client;
    //保存歌曲名
    QString songFlag;
    //把歌词存在list中
    QList<lyricPhrase> list;
    int songBeginTime;
    //用于anchor，滚动所用
    int lastIndex;
    //检测鼠标是否滚动
    bool wheelScroll;
public slots:
    //获取到歌词,调用的槽函数
    void dealLyricSlot(QString lyric);
    //歌曲名变化,调用槽函数
    void dealSongName(QString name);

};
#endif // MAINWINDOW_H
