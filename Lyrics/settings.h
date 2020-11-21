#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QTranslator>
#include "setstruct.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Settings; }
QT_END_NAMESPACE
#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "

class Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Settings(SetStruct* setStruct,QWidget *parent = nullptr);
    void changeSet();
    void resetCombox();
private:
    void initConfig();


    Ui::Settings *ui;

    SetStruct* setStruct;
    QTranslator* trans;
signals:
    void setChangeSignal();
private slots:
    void dealFontSize(QString str);
    void dealFontFamily(QFont font);
    void on_comboBoxLang_currentIndexChanged(int index);
    void on_comboBoxOri_currentIndexChanged(int index);
    void dealDeskLrcClick(bool status);
    void dealBgColorClick(bool status);
};

#endif // SETTINGS_H
