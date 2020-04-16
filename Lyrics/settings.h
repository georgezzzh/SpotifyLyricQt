#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QTranslator>
QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE
#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "
class SetStruct{
public:
  QString bgColor;
  QString fontFamily;
  QString fontSize;
  QString lang;
  QString deskLrcStatus;
  QString deskLrcOri;
  //语言不设置预置项
  SetStruct(QString bgColor="dark",QString fontFamily="Arial",
            QString fontSize="20",QString lang="")  {
      this->bgColor = bgColor;
      this->fontFamily = fontFamily;
      this->fontSize = fontSize;
      this->lang = lang;
      this->deskLrcStatus = "on";
      this->deskLrcOri = "HORIZONTAL";
  }
  void setDeskLrc(QString deskLrcStatus = "on",QString deskLrcOri = "HORIZONTAL"){
      this->deskLrcOri = deskLrcOri;
      this->deskLrcStatus = deskLrcStatus;
  }
  void setAttributes(QString bgColor,QString fontFamily,QString fontSize,QString lang){
      this->bgColor = bgColor;
      this->fontFamily = fontFamily;
      this->fontSize = fontSize;
      this->lang = lang;
  }
};
class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(SetStruct* setStruct,QWidget *parent = nullptr);
private:
    void initConfigFile();
    void resetCombox();
    void changeSet();
    Ui::Settings *ui;
    QString configName;
    SetStruct* setStruct;
    QTranslator* trans;
signals:
    void setChangeSignal();
private slots:
    void dealFontSize(QString str);
    void dealFontFamily(QFont font);
    void on_comboBoxColor_currentIndexChanged(int index);
    void on_comboBoxLang_currentIndexChanged(int index);
    void on_comboBoxOri_currentIndexChanged(int index);
    void on_radioButtonLrcStatus_clicked(bool checked);
};

#endif // SETTINGS_H
