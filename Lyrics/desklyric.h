#ifndef DESKLYRIC_H
#define DESKLYRIC_H

#include <QWidget>
enum Oritention{HORIZONTAL,VERTICAL};
class DeskLyric : public QWidget
{
    Q_OBJECT
public:
    explicit DeskLyric(QWidget *parent = nullptr);
    void setLyricText(QString text);
    void setDuration(double duration);
    void setOrientation(Oritention ori){this->orientation = ori;}
protected:
    void paintEvent(QPaintEvent *) override;
    void mouseMoveEvent(QMouseEvent* ev) override;
    void mousePressEvent(QMouseEvent* ev) override;
private:
    //唱过的歌词长度
    int lyricWidth;
    //整句歌词的长度
    int lineWidth;
    //持续时间
    double duration;
    double step;
    //用于鼠标拖动
    QPoint p;
    //用于显示横屏还是竖屏模式
    Oritention orientation;
    QString text ;
signals:

};

#endif // DESKLYRIC_H
