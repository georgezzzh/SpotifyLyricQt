#ifndef APPLEBUTTON_H
#define APPLEBUTTON_H

#include <QWidget>
#include <QPushButton>
#define cout qDebug().noquote()<<"["<<__FILE__<<":"<<__LINE__<<"]: "
enum MODE{BG_COLOR_BTN,LRC_BTN};
class AppleButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AppleButton(MODE mode,QWidget *parent = nullptr);
    void setCheck(bool ck){this->m_checked=ck;}
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent*) override;
private:
    bool m_checked;
    MODE  mode;
    QString m_textOn;
    QString m_textOff;
    void drawSlider();
signals:
    void deskLrcBtnClick(bool);
    void bgColorBtnClick(bool);

};

#endif // APPLEBUTTON_H
