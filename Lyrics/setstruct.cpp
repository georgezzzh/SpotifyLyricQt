#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include "setstruct.h"
#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

void SetStruct::setAttributes(QString bgColor,QString fontFamily,QString fontSize,QString lang){
    this->bgColor = bgColor;
    this->fontFamily = fontFamily;
    this->fontSize = fontSize;
    this->lang = lang;

}
void SetStruct::setDeskLrc(QString deskLrcStatus,QString deskLrcOri){
    this->deskLrcOri = deskLrcOri;
    this->deskLrcStatus = deskLrcStatus;
}
void SetStruct::initConfigFile(){
        QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir appDir(appDataPath);
        if(!appDir.exists()){
            //创建父级目录
            appDir.mkdir(appDataPath);
        }
        this->configName = appDataPath +"/config.json";
        cout<<"configName:"<<configName;
        //读取配置
        QFile file(configName);
        if(!file.exists()) {
            qDebug()<<"config file does not exist";
         return;
        }
        file.open(QIODevice::ReadOnly);
        QString config=file.readAll();
        QJsonDocument document = QJsonDocument::fromJson(config.toUtf8());
        QJsonObject jsonObject  = document.object();
        QString lang = jsonObject.value("lang").toString();
        QString fontSize = jsonObject.value("fontSize").toString();
        QString fontFamily = jsonObject.value("fontFamily").toString();
        QString bgColor = jsonObject.value("bgColor").toString();
        QString deskLrcOri = jsonObject.value("deskLrcOri").toString();
        QString deskLrcStatus = jsonObject.value("deskLrcStatus").toString();
        this->setAttributes(bgColor,fontFamily,fontSize,lang);
        this->setDeskLrc(deskLrcStatus,deskLrcOri);
}
void SetStruct::writeConfigFile(){
    cout<<"modify changeset file";
    QJsonObject jsonObject;
    jsonObject.insert("fontSize",this->fontSize);
    jsonObject.insert("fontFamily",this->fontFamily);
    jsonObject.insert("lang",this->lang);
    jsonObject.insert("bgColor",this->bgColor);
    jsonObject.insert("deskLrcStatus",this->deskLrcStatus);
    jsonObject.insert("deskLrcOri",this->deskLrcOri);
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
}
