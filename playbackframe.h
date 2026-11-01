#pragma once

#include "HCNetSDK.h"
#include <QDateTime>
#include <QFrame>

namespace Ui
{
class PlaybackFrame;
}

class HK_DVR;
class HK_Playback;

class PlaybackFrame : public QFrame
{
    Q_OBJECT

public:
    PlaybackFrame(QWidget *parent, const std::shared_ptr<HK_DVR> &dvr);
    ~PlaybackFrame();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    virtual void createPlayback() = 0;

    virtual bool canSeek() const;

private slots:
    void on_play_clicked();
    void on_stop_clicked();
    void on_fast_clicked();
    void on_slow_clicked();
    void on_normal_clicked();
    void on_pause_clicked();
    void on_step_clicked();
    void on_progress_sliderReleased();
    virtual void on_download_clicked() = 0;

protected:
    Ui::PlaybackFrame *ui;

    const std::shared_ptr<HK_DVR> myDVR;

    std::shared_ptr<HK_Playback> myPlayback;
    QDateTime myOrgStart;
    QDateTime myOrgEnd;

    int myTimer;
    int myLogSpeed;

    WId getWindowHandle() const;
    bool command(const DWORD cmd);
    void resetPlayback();
    void syncControls(const bool enabled);
    QString getLogSpeedString() const;
};
