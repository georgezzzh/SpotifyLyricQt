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
    this->setWindowIcon(QIcon("://icon/set.ico"));
    this->setWindowTitle("Settings");
    this->setAutoFillBackground(true);//必须有这条语句
    this->setStruct = setStruct;
    this->trans = new QTranslator(this);
    qApp->installTranslator(trans);
    ui->lineEditFontSize->setStyleSheet("QLineEdit{font:black;}");
    ui->fontComboBox->setStyleSheet("QFontComboBox{font:black;}");
    ui->comboBoxLang->setStyleSheet("QFontComboBox{font:black;}");
    ui->comboBoxColor->setStyleSheet("QFontComboBox{font:black;}");
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
    //更新字体展示
    connect(ui->lineEditFontSize,SIGNAL(textChanged(QString)),this,SLOT(dealFontSize(QString)));
    connect(ui->fontComboBox,SIGNAL(currentFontChanged(QFont)),this,SLOT(dealFontFamily(QFont)));
    //json配置文件
    initConfigFile();
    //将config.json的内容写入Combox中
    changeTextBrowser();
    resetCombox();
}
void Settings::initConfigFile()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDir(appDataPath);
    if(!appDir.exists()){
        //创建父级目录
        appDir.mkdir(appDataPath);
    }
    this->configName = appDataPath +"/config.json";
    //读取配置
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)+"/config.json");
    if(!file.exists()) return;
    file.open(QIODevice::ReadOnly);
    QString config=file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(config.toUtf8());
    QJsonObject jsonObject  = document.object();
    QString lang = jsonObject.value("lang").toString();
    QString fontSize = jsonObject.value("fontSize").toString();
    QString fontFamily = jsonObject.value("fontFamily").toString();
    QString bgColor = jsonObject.value("bgColor").toString();
    this->setStruct->setAttributes(bgColor,fontFamily,fontSize,lang);
    QString deskLrcOri = jsonObject.value("deskLrcOri").toString();
    QString deskLrcStatus = jsonObject.value("deskLrcStatus").toString();
    setStruct->setDeskLrc(deskLrcStatus,deskLrcOri);
    if(lang =="简体中文"){
        trans->load("tr/Translation_CN.qm");
    }else if(lang == "English"){
        trans->load("tr/Translation_EN.qm");
    }
    ui->retranslateUi(this);

}
void Settings::resetCombox()
{
    ui->lineEditFontSize->setText(this->setStruct->fontSize);
    ui->comboBoxLang->setCurrentText(this->setStruct->lang);
    ui->fontComboBox->setCurrentText(this->setStruct->fontFamily);
    if(setStruct->bgColor=="dark"){
        this->setPalette(QPalette(QColor(34,34,34)));
        ui->comboBoxColor->setCurrentIndex(0);
    }else{
        this->setPalette(QPalette(QColor(255,255,255)));
        ui->comboBoxColor->setCurrentIndex(1);
    }
    //桌面歌词部分
    if(setStruct->deskLrcOri == "HORIZONTAL"){
        ui->comboBoxOri->setCurrentIndex(0);
    }else{
        ui->comboBoxOri->setCurrentIndex(1);
    }
    if(setStruct->deskLrcStatus == "on"){
        ui->radioButtonLrcStatus->setChecked(true);
    }else{
        ui->radioButtonLrcStatus->setChecked(false);
    }
}
void Settings::changeTextBrowser()
{
     QFont font;
     font.setPixelSize(setStruct->fontSize.toInt());
     font.setFamily(setStruct->fontFamily);
     //更新字体提示框中内容
     if(setStruct->fontSize!="0"){
         ui->textBrowser->setFont(font);
         ui->textBrowser->setText(ui->textBrowser->document()->toPlainText());
     }
     QJsonObject jsonObject;
     jsonObject.insert("fontSize",setStruct->fontSize);
     jsonObject.insert("fontFamily",setStruct->fontFamily);
     jsonObject.insert("lang",setStruct->lang);
     jsonObject.insert("bgColor",setStruct->bgColor);
     jsonObject.insert("deskLrcStatus",setStruct->deskLrcStatus);
     jsonObject.insert("deskLrcOri",setStruct->deskLrcOri);
     QJsonDocument jsonDoc;
     jsonDoc.setObject(jsonObject);
     QFile file(this->configName);
     if(file.exists()){
         file.remove();
     }
     if(file.open(QIODevice::WriteOnly)){
         file.write(jsonDoc.toJson());
     }
     file.close();
     //发送信号，通知首页面进行修改背景色等设置
     //需要修改语言项
     emit setChangeSignal();
}

void Settings::dealFontFamily(QFont font)
{
    setStruct->fontFamily = font.family();
    changeTextBrowser();
}
void Settings::dealFontSize(QString str)
{
    setStruct->fontSize = str;
    changeTextBrowser();
}

void Settings::on_comboBoxLang_currentIndexChanged(int index)
{
    if(index==0){
        setStruct->lang = "简体中文";
        //trans->load("tr/Translation_CN.qm");
    }else if(index ==1){
        setStruct->lang = "English";
        //trans->load("tr/Translation_EN.qm");
    }
    changeTextBrowser();
}

void Settings::on_comboBoxColor_currentIndexChanged(int index)
{
    //黑色
    if(index==0){
        this->setPalette(QPalette(QColor(51,51,51)));
        setStruct->bgColor = "dark";
    }
    //白色
    if(index ==1){
        this->setPalette(QPalette(QColor(255,255,255)));
        setStruct->bgColor = "white";
    }
    changeTextBrowser();
}


void Settings::on_comboBoxOri_currentIndexChanged(int index)
{
    if(index==0){
        setStruct->deskLrcOri = "HORIZONTAL";
    }else{
        setStruct->deskLrcOri = "VERTICAL";
    }
    changeTextBrowser();
}

void Settings::on_radioButtonLrcStatus_clicked(bool checked)
{
    if(checked){
        setStruct->deskLrcStatus = "on";
    }else{
        setStruct->deskLrcStatus = "off";
    }
    changeTextBrowser();
}
