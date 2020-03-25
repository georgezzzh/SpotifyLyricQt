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
    ui->lineEditFontSize->setStyleSheet("QLineEdit{font:black;}");
    ui->fontComboBox->setStyleSheet("QFontComboBox{font:black;}");
    ui->comboBoxLang->setStyleSheet("QFontComboBox{font:black;}");
    ui->comboBoxColor->setStyleSheet("QFontComboBox{font:black;}");
    this->changeTextBrowser();
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
    //更新字体展示
    connect(ui->lineEditFontSize,SIGNAL(textChanged(QString)),this,SLOT(dealFontSize(QString)));
    connect(ui->fontComboBox,SIGNAL(currentFontChanged(QFont)),this,SLOT(dealFontFamily(QFont)));

    //json配置文件
    initConfigFile();
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

}
void Settings::changeTextBrowser()
{
     int fontSize= ui->lineEditFontSize->text().toInt();
     QFont font =ui->fontComboBox->currentFont();
     font.setPixelSize(fontSize);
     //更新字体提示框中内容
     if(fontSize!=0){
         ui->textBrowser->setFont(font);
         ui->textBrowser->setText(ui->textBrowser->document()->toPlainText());
     }
     //限制fontSize的最大与最小值
     fontSize = (fontSize==0)?20:fontSize;
     fontSize = (fontSize>30)?30:fontSize;
     QString lang = ui->comboBoxLang->currentText();
     int bgColorIndex = ui->comboBoxColor->currentIndex();
     QString bgColor = (bgColorIndex==0)?"dark":"white";
     QJsonObject jsonObject;
     jsonObject.insert("fontSize",QString::number(fontSize));
     jsonObject.insert("fontFamily",font.family());
     jsonObject.insert("lang",lang);
     jsonObject.insert("bgColor",bgColor);
     QJsonDocument jsonDoc;
     jsonDoc.setObject(jsonObject);
     QFile file;
     file.setFileName(this->configName);
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
    changeTextBrowser();
}
void Settings::dealFontSize(QString str)
{
    changeTextBrowser();
}

void Settings::on_comboBoxLang_currentIndexChanged(const QString &arg1)
{
    changeTextBrowser();
}

void Settings::on_comboBoxColor_currentIndexChanged(int index)
{
    changeTextBrowser();
    //黑色
    if(index==0){
        this->setPalette(QPalette(QColor(51,51,51)));
    }
    //白色
    if(index ==1){
        this->setPalette(QPalette(QColor(255,255,255)));
    }

}
