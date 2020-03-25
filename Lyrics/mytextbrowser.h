#ifndef MYTEXTBROWSER_H
#define MYTEXTBROWSER_H


#include <QWidget>
#include <QTextBrowser>

class MyTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit MyTextBrowser(QWidget *parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* ev) override;
signals:
    void myWhellScroll();

};

#endif // MYTEXTBROWSER_H
