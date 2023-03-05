#pragma once

#include <QFrame>
#include <QDateTime>

namespace Ui {
class LiveFrame;
}

class HK_LivePlayer;

class LiveFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LiveFrame(QWidget *parent = nullptr);
    ~LiveFrame();

    void setPlay(const std::shared_ptr<HK_LivePlayer> & play);

    WId getWindowHandle() const;

protected:
    bool event(QEvent *event) override;
    void timerEvent(QTimerEvent *) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_record_clicked();

    void on_stop_clicked();

    void on_picture_clicked();

private:
    Ui::LiveFrame *ui;
    std::shared_ptr<HK_LivePlayer> myLivePlayer;

    int myTimer;
    QDateTime myStartTime;
};
