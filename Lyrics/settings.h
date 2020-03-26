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
  SetStruct(QString bgColor="dark",QString fontFamily="Arial",
            QString fontSize="20",QString lang="")  {
      this->bgColor = bgColor;
      this->fontFamily = fontFamily;
      this->fontSize = fontSize;
      this->lang = lang;
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
    void changeTextBrowser();
    Ui::Settings *ui;
    QString configName;
    SetStruct* setStruct;
    QTranslator* trans;
signals:
    void setChangeSignal();
private slots:
    void dealFontSize(QString str);
    void dealFontFamily(QFont font);
    void on_comboBoxLang_currentIndexChanged(const QString &arg1);
    void on_comboBoxColor_currentIndexChanged(int index);
};

#endif // SETTINGS_H
