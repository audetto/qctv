#ifndef LIVEFRAME_H
#define LIVEFRAME_H

#include <QFrame>
#include <QDateTime>

namespace Ui {
class LiveFrame;
}

class HK_Play;

class LiveFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LiveFrame(QWidget *parent = nullptr);
    ~LiveFrame();

    void setPlay(const std::shared_ptr<HK_Play> & play);
    void resizeEvent(QResizeEvent *event) override;

    WId getWindowHandle() const;

protected:
    bool event(QEvent *event) override;
    void timerEvent(QTimerEvent *);

private slots:
    void on_record_clicked();

    void on_stop_clicked();

    void on_picture_clicked();

private:
    Ui::LiveFrame *ui;
    std::shared_ptr<HK_Play> myPlay;

    int myTimer;
    QDateTime myStartTime;
};

#endif // LIVEFRAME_H
