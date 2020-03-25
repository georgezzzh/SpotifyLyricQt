#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextBrowser>
#include <QMessageBox>
#include <QRegExp>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QFileDialog>
#include <QWheelEvent>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

//一开始程序崩溃的原因:应该是在非mainwindow.cpp修改了UI了,指定父对象为this

#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //进度条
    QString sss="QSlider::add-page:Horizontal{background-color: rgb(87, 97, 106);height:4px;}";
    sss += "QSlider::sub-page:Horizontal{height:4px;background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(231,80,229, 255), stop:1 rgba(7,208,255, 255));}";
    sss += "QSlider::groove:Horizontal{background:transparent;height:6px;}";
    sss += "QSlider::handle:Horizontal{height: 30px;width:8px;border-image: url(://icon/dot.png);margin: -8 0px;}";
    ui->horizontalSlider->setStyleSheet(sss);
    //当前播放时间
    connect(ui->horizontalSlider,&QSlider::valueChanged,this,[=](){
        int val = ui->horizontalSlider->value();
        QString text;
        text = (val/60<10)? "0"+QString::number(val/60):QString::number(val/60);
        text += (val%60<10)?":0"+QString::number(val%60):":"+QString::number(val%60);
        ui->currentTimeLabel->setText(text);
    });
    //滑动进度条触发的事件
    connect(ui->horizontalSlider,&QSlider::sliderMoved,this,[=](){
       int val=ui->horizontalSlider->value();
       int i=0;
       int diff = (QDateTime::currentMSecsSinceEpoch()-songBeginTime)/1000;
       songBeginTime +=(diff-val)*1000;
       //修改高亮歌词
       while(i<list.size() && list[i].time<val){
           list[i].highLight = true;
           i++;
       }
       lastIndex=(i>3)? i-3: 0;
       while(i<list.size()){
           list[i].highLight = false;
           i++;
       }
       cout<<"滑动进度条, value:"<<val;
    });
    //菜单栏设置
    ui->lyricLabel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->title->setAlignment(Qt::AlignCenter);
    this->wheelScroll = false;
    //设置配置
    this->setStruct = new SetStruct();
    configInit();
    ui->menubar->setStyleSheet("QMenubar{font-color:black;}");
    set = new Settings(this->setStruct);
    connect(this->set,&Settings::setChangeSignal,this,[=](){
        //更新保存的设置配置
        configInit();
    });
    connect(ui->actionSettings,&QAction::triggered,[=](){
        set->setWindowModality(Qt::WindowModality::ApplicationModal);
        set->show();
    });
    connect(ui->actionAboutThis,&QAction::triggered,[=](){
        QMessageBox::about(this,"About","Spotify曲词同步工具, Based on QT<br/><a href='https://github.com/georgezzzh/SpotifyLyricQT'>源代码</a>");
    });
    connect(ui->actionExport,&QAction::triggered,[=](){
        QString deskPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString suffix = "/"+songFlag+".txt";
        deskPath = deskPath+suffix;
        QString newPath = QFileDialog::getSaveFileName(this,"save",deskPath,"TXT(*.txt)");
        if(!newPath.isEmpty()){
            QString oldPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
            oldPath = oldPath + "/lyricText"+suffix;
            if(QFile(oldPath).exists()){
                bool copyOk=QFile::copy(oldPath,newPath);
                copyOk?cout<<"复制文件成功":cout<<"复制文件失败";
            }
        }
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
    //绑定鼠标滚轮滚动
    connect(ui->lyricLabel,&MyTextBrowser::myWhellScroll,this,[=](){
        cout<<"收到鼠标中键滚动信号";
        this->wheelScroll = true;
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetLyricDisplay()
{
    if(this->list.size()<1){
        ui->lyricLabel->setText("Unknown Error");
        return;
    }
    int now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    int diff = now - this->songBeginTime;
    QString text;
    int lastIndexUpdate = 0;
    QString fontColorStyle;
    if(this->setStruct->bgColor == "dark"){
        fontColorStyle = " color='white' ";
    }else{
        fontColorStyle = " color='black' ";
    }
    for(int i=0; i<this->list.size();i++){
        //未开始时, anchor设在首句
        if(lastIndex<3 && i==0){
            text = text+"<p><a name='myAnchor'><font"+fontColorStyle+">"+list[i].lyric+"</font><a></p>";
        }
        else if(lastIndex-i == 3){
            text = text+"<p><a name='myAnchor'><font"+fontColorStyle+">"+list[i].lyric+"</font><a></p>";
        }else{
            if(list[i].time<(diff/1000) && !list[i].highLight){
                text = text+"<h3><font color='green'>"+list[i].lyric+"</font></h3>";
                list[i].highLight = true;
                lastIndexUpdate = i;
            }else{
                text = text+"<p"+fontColorStyle+"><font"+fontColorStyle+">"+list[i].lyric+"</font><p>";
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
            //更新进度条
            ui->horizontalSlider->setValue(diff);
            bool update = false;
            for(int i=0;i<list.size() && list[i].time<diff;i++){
                if(!list[i].highLight)
                    update = true;
            }
            //当鼠标不滚动时再切换
            if(update && !this->wheelScroll)
                this->resetLyricDisplay();
            //重新修改鼠标中键滚动状态
            this->wheelScroll = false;
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
        //设置滚动条
        if(list.size()-1 >= 0){
            ui->horizontalSlider->setMaximum(list[list.size()-1].time);
        }
        this->resetLyricDisplay();
    }
}

void MainWindow::configInit()
{
    QJsonParseError jsonError;
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dirPath);
    QFile setFile;
    setFile.setFileName(dirPath+"/config.json");
    if(!dir.exists() || !setFile.exists() || !setFile.open(QIODevice::ReadOnly)){
        return;
    }
    QJsonDocument document = QJsonDocument::fromJson(setFile.readAll(),&jsonError);
    if(document.isNull() || jsonError.error != QJsonParseError::NoError)
        return ;
    QString bgColor = document.object().value("bgColor").toString();
    QString fontFamily = document.object().value("fontFamily").toString();
    QString fontSize = document.object().value("fontSize").toString();
    QString lang = document.object().value("lang").toString();
    if(bgColor!="") this->setStruct->bgColor = bgColor;
    if(fontFamily!="") this->setStruct->fontFamily = fontFamily;
    if(fontSize!="") this->setStruct->fontSize = fontSize;
    if(lang!="") this->setStruct->lang = lang;
    //改变背景颜色
    //dark mode
    this->setAutoFillBackground(true);//必须有这条语句
    QString fontSizeStyle ="font-size:"+setStruct->fontSize+"px;font-family:"+setStruct->fontFamily+";";
    if(setStruct->bgColor=="dark"){
        this->setPalette(QPalette(QColor(34,34,34)));
        ui->lyricLabel->setStyleSheet("QTextBrowser{background-color:#222222;border-radius:8px;"+fontSizeStyle+"}");
    }else{
        this->setPalette(QPalette(QColor(255,255,255)));
        ui->lyricLabel->setStyleSheet("QTextBrowser{background-color:#ffffff;border-radius:8px;"+fontSizeStyle+"}");
    }

}

