#ifndef SETSTRUCT_H
#define SETSTRUCT_H
#include <QString>

class SetStruct
{
private:
    QString configName;
public:
  QString bgColor;
  QString fontFamily;
  QString fontSize;
  //简体中文,English
  QString lang;
  QString deskLrcStatus;
  QString deskLrcOri;
  //语言不设置预置项
  SetStruct(QString bgColor="white",QString fontFamily="Arial",
            QString fontSize="20",QString lang="")  {
      this->bgColor = bgColor;
      this->fontFamily = fontFamily;
      this->fontSize = fontSize;
      this->lang = lang;
      this->deskLrcStatus = "off";
      this->deskLrcOri = "HORIZONTAL";
      initConfigFile();
  }
  void setDeskLrc(QString deskLrcStatus = "off",QString deskLrcOri = "HORIZONTAL");
  void setAttributes(QString bgColor,QString fontFamily,QString fontSize,QString lang);
  void initConfigFile();
  void writeConfigFile();

};

#endif // SETSTRUCT_H
