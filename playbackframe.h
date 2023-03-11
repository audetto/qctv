#pragma once

#include <QFrame>

#include "HCNetSDK.h"

namespace Ui {
class PlaybackFrame;
}

class HK_DVR;
class HK_Playback;

class PlaybackFrame : public QFrame
{
    Q_OBJECT

public:
    explicit PlaybackFrame(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const size_t channel);
    ~PlaybackFrame();

signals:
    void downloadOnChannel(const size_t channel, const QDateTime & start);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private slots:
    void on_play_clicked();
    void on_stop_clicked();
    void on_fast_clicked();
    void on_slow_clicked();
    void on_normal_clicked();
    void on_pause_clicked();
    void on_step_clicked();
    void on_download_clicked();

private:
    Ui::PlaybackFrame *ui;

    const std::shared_ptr<HK_DVR> myDVR;
    const size_t myChannel;

    std::shared_ptr<HK_Playback> myPlayback;

    int myTimer;
    int myLogSpeed;

    WId getWindowHandle() const;
    bool command(const DWORD cmd);
    void resetPlayback();
    QString getLogSpeedString() const;
};
