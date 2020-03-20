#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextBrowser>
#include <QMessageBox>
#include <QRegExp>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>

#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setStyleSheet("QWidget{background-color:white;}");
    ui->lyricLabel->setStyleSheet("QTextBrowser{font-size:22px;background-color:#222222;"
                                  "border-radius:8px;}");
    ui->lyricLabel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->title->setAlignment(Qt::AlignCenter);
    //菜单栏设置
    QMenuBar *mBar = menuBar();
    QMenu* pOption = mBar->addMenu("选项");
    QAction* pLang = pOption->addAction("语言");
    connect(pLang,&QAction::triggered,[=](){
       cout <<"语言被按" ;
    });
    QMenu* pAbout = mBar->addMenu("关于");
    QAction* pThis = pAbout->addAction("本项目");
    connect(pThis,&QAction::triggered,[=](){
        QMessageBox::about(this,"About","Spotify曲词同步工具, Based by QT");
    });
    this->songFlag = "";
    sp = new Spotify(this);
    connect(sp,SIGNAL(threadOver(QString)),this,SLOT(dealSongName(QString)));
    //关闭函数时,线程退出
    connect(this,&MainWindow::destroyed,this,[=](){
       if(sp->isRunning()){
           sp->setRunning(false);
           sp->exit(0);
           sp->quit();
           sp->wait();
       }
    });
    sp->start();
    //绑定槽函数
    client = new Client(this);
    connect(client,SIGNAL(sendLyricSignal(QString)),this,SLOT(dealLyricSlot(QString)));
}
void MainWindow::resetLyricDisplay()
{
    if(this->list.size()<1){
        ui->lyricLabel->setText("未知错误");
        return;
    }
    int now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    int diff = now - this->songBeginTime;
    QString text;
    int lastIndexUpdate = 0;
    for(int i=0; i<this->list.size();i++){
        //未开始时, anchor设在首句
        if(lastIndex<3 && i==0){
            text = text+"<p><a name='myAnchor'><font color='white'>"+list[i].lyric+"</font><a></p>";
        }
        else if(lastIndex-i == 3){
            text = text+"<p><a name='myAnchor'><font color='white'>"+list[i].lyric+"</font><a></p>";
        }else{
            if(list[i].time<(diff/1000) && !list[i].highLight){
                text = text+"<p><font color='green'>"+list[i].lyric+"</font></p>";
                list[i].highLight = true;
                lastIndexUpdate = i;
            }else{
                text = text+"<p><font color='white'>"+list[i].lyric+"</font><p>";
            }
        }
    }
    //更新anchor的位置
    lastIndex = (lastIndexUpdate == 0)?lastIndex:lastIndexUpdate;
    ui->lyricLabel->setText(text);
    //应该检测鼠标滚动，当鼠标滚动时，不执行scrollToAnchor函数
    ui->lyricLabel->scrollToAnchor("myAnchor");

}

void MainWindow::dealSongName(QString songName)
{
    if(songName=="-1"){
        ui->title->setText("未找到Spotify进程");
    }else if(songName == ""){
        ui->title->setText("未能获取到播放的歌目");
    }else{
        //歌曲未切换时,不做处理
        if(songName == songFlag){
            //检查是否有必要更新歌词
            int now = QDateTime::currentDateTime().toMSecsSinceEpoch();
            double diff = (now-songBeginTime)/1000;
            bool update = false;
            for(int i=0;i<list.size() && list[i].time<diff;i++){
                if(!list[i].highLight)
                    update = true;
            }
            if(update)
                this->resetLyricDisplay();
            return;
        }
        //歌名切换设置时间戳
        int timeStamp=QDateTime::currentDateTime().toMSecsSinceEpoch();
        this->songBeginTime = timeStamp;
        this->lastIndex =0;
        songFlag = songName;
        ui->title->setText(songName);
        client->setSongTitle(songName);
        client->sendHttpRequest(songName);
        //一开始程序崩溃的原因:应该是在非mainwindow.cpp修改了UI了,指定父对象为this
    }
}

void MainWindow::dealLyricSlot(QString lyric)
{
    if(lyric == ""){
        ui->lyricLabel->setText("服务端未查询到该歌词");
    }else{
        lyric +="\n";
        lyric.replace(QRegExp("[\r]"),"");
        this->list.clear();
        QRegExp rx("\\[([\\d]{2}):([\\d|\\.]+)\\](.*)\n");
        rx.setMinimal(true);
        int pos=0;
        while((pos = rx.indexIn(lyric,pos))!=-1){
            pos += rx.matchedLength();
            lyricPhrase tmp;
            tmp.time=rx.cap(1).toInt()*60+ rx.cap(2).toDouble();
            tmp.lyric=rx.cap(3);
            tmp.highLight = false;
            this->list.append(tmp);
        }
        this->resetLyricDisplay();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

