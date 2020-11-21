#include "ui_settings.h"
#include "settings.h"
#include "mylabel.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>

Settings::Settings(SetStruct* setStruct,QWidget *parent) : QWidget(parent),ui(new Ui::Settings)
{    
    ui->setupUi(this);
    this->setFixedSize(578,414);
    this->setAutoFillBackground(true);//必须有这条语句
    this->setStruct = setStruct;
    this->trans = new QTranslator(this);
    qApp->installTranslator(trans);
    ui->lineEditFontSize->setStyleSheet("QLineEdit{font:black;}");
    ui->fontComboBox->setStyleSheet("QFontComboBox{font:black;}");
    ui->comboBoxLang->setStyleSheet("QFontComboBox{font:black;}");
    ui->comboBoxOri->setStyleSheet("QFontComboBox{font:black;}");
    //信号与槽
    connect(ui->labelFont,&MyLabel::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(ui->labelLang,&MyLabel::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(ui->labelBackground,&MyLabel::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(2);
    });
    connect(ui->labelDesktopLrc,&MyLabel::clicked,[=](){
        ui->stackedWidget->setCurrentIndex(3);
    });
    ui->stackedWidget->setCurrentIndex(0);
    //deskLrcBtnClick
    connect(ui->lrcStatus,SIGNAL(deskLrcBtnClick(bool)),this,SLOT(dealDeskLrcClick(bool)));
    connect(ui->bgColorStatus,SIGNAL(bgColorBtnClick(bool)),this,SLOT(dealBgColorClick(bool)));
    //更新字体展示
    connect(ui->lineEditFontSize,SIGNAL(textChanged(QString)),this,SLOT(dealFontSize(QString)));
    connect(ui->fontComboBox,SIGNAL(currentFontChanged(QFont)),this,SLOT(dealFontFamily(QFont)));
    //更新配置
    initConfig();
    //更新配置
    changeSet();
    resetCombox();
}
void Settings::initConfig()
{

    #if defined(WIN32)
        //for windows
        if(this->setStruct->lang =="简体中文"){
            trans->load("tr/Translation_CN.qm");
        }else{
            trans->load("tr/Translation_EN.qm");
        }
    #else
        //only for linux-os
        if(this->setStruct->lang =="简体中文"){
            trans->load("/opt/SpotifyLyricQt/tr/Translation_CN.qm");
        }else if(this->setStruct->lang == "English"){
            trans->load("/opt/SpotifyLyricQt/tr/Translation_EN.qm");
        }
    #endif

    ui->retranslateUi(this);
}
void Settings::resetCombox()
{
    ui->lineEditFontSize->setText(this->setStruct->fontSize);
    ui->comboBoxLang->setCurrentText(this->setStruct->lang);
    ui->fontComboBox->setCurrentText(this->setStruct->fontFamily);
    if(setStruct->bgColor=="dark"){
        this->setPalette(QPalette(QColor(34,34,34)));
    }else{
        this->setPalette(QPalette(QColor(255,255,255)));
    }
    //桌面歌词部分
    if(setStruct->deskLrcOri == "HORIZONTAL"){
        ui->comboBoxOri->setCurrentIndex(0);
    }else{
        ui->comboBoxOri->setCurrentIndex(1);
    }
    //ui->lrcStatus
    setStruct->deskLrcStatus=="on"?ui->lrcStatus->setCheck(true):ui->lrcStatus->setCheck(false);
    //
    setStruct->bgColor=="dark"?ui->bgColorStatus->setCheck(true):ui->bgColorStatus->setCheck(false);
    //fonts
    QFont font;
    font.setPixelSize(setStruct->fontSize.toInt());
    font.setFamily(setStruct->fontFamily);
    //更新字体提示框中内容
    if(setStruct->fontSize!="0"){
        ui->textBrowser->setFont(font);
        ui->textBrowser->setText(ui->textBrowser->document()->toPlainText());
    }
}
void Settings::changeSet()
{
     //modify set file
     this->setStruct->writeConfigFile();
     //发送信号，通知首页面进行修改背景色等设置
     //需要修改语言项
     emit setChangeSignal();
}
void Settings::dealFontFamily(QFont font)
{
    setStruct->fontFamily = font.family();
    changeSet();
}
void Settings::dealFontSize(QString str)
{
    setStruct->fontSize = str;
    changeSet();
}
void Settings::dealDeskLrcClick(bool status)
{
    qDebug()<<"apple button clicked:"<<status;
    setStruct->deskLrcStatus = status?"on":"off";
    changeSet();
}
void Settings::dealBgColorClick(bool status){
    //黑色
    if(status==true){
        this->setPalette(QPalette(QColor(51,51,51)));
        setStruct->bgColor = "dark";
    }
    //白色
    if(status ==false){
        this->setPalette(QPalette(QColor(255,255,255)));
        setStruct->bgColor = "white";
    }
    changeSet();
}
void Settings::on_comboBoxLang_currentIndexChanged(int index)
{
    if(index==0){
        setStruct->lang = "简体中文";
    }else if(index ==1){
        setStruct->lang = "English";
        //trans->load("tr/Translation_CN.qm");
        //trans->load("tr/Translation_EN.qm");
    }
    changeSet();
}

void Settings::on_comboBoxOri_currentIndexChanged(int index)
{
    if(index==0){
        setStruct->deskLrcOri = "HORIZONTAL";
    }else{
        setStruct->deskLrcOri = "VERTICAL";
    }
    changeSet();
}



