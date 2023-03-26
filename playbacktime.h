#pragma once

#include "playbackframe.h"


class PlaybackTime : public PlaybackFrame
{
    Q_OBJECT

public:
    PlaybackTime(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const size_t channel);

signals:
    void downloadOnChannel(const size_t channel, const QDateTime & start);

protected:
    virtual void createPlayback() override;

private:
    virtual void on_download_clicked() override;

private:
    const size_t myChannel;
};
