#ifndef APPLEBUTTON_H
#define APPLEBUTTON_H

#include <QWidget>
#include <QPushButton>
class AppleButton : public QPushButton
{
    Q_OBJECT
public:
    explicit AppleButton(QWidget *parent = nullptr);
    void setCheck(bool ck){this->m_checked=ck;}
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent*) override;
private:
    bool m_checked;
    void drawSlider();
signals:
    void appleClick(bool);

};

#endif // APPLEBUTTON_H
