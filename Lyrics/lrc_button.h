#ifndef LRC_BUTTON_H
#define LRC_BUTTON_H

#include "applebutton.h"
#include <QWidget>
#include <QPushButton>

class Lrc_button : public AppleButton
{
    Q_OBJECT
public:
    explicit Lrc_button(QWidget *parent = nullptr);

signals:

};

#endif // LRC_BUTTON_H
