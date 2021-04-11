#ifndef PLAYBACKFRAME_H
#define PLAYBACKFRAME_H

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

protected:
    void resizeEvent(QResizeEvent *event) override;
    void timerEvent(QTimerEvent *) override;

private slots:
    void on_play_clicked();
    void on_stop_clicked();
    void on_fast_clicked();
    void on_slow_clicked();
    void on_normal_clicked();
    void on_pause_clicked();
    void on_step_clicked();

private:
    Ui::PlaybackFrame *ui;

    const std::shared_ptr<HK_DVR> myDVR;
    const size_t myChannel;

    std::shared_ptr<HK_Playback> myPlayback;

    int myTimer;
    int myLogSpeed;

    WId getWindowHandle() const;
    void command(const DWORD cmd);
    QString getLogSpeedString() const;
};

#endif // PLAYBACKFRAME_H
