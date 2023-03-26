#pragma once

#include "playbackframe.h"


class PlaybackName : public PlaybackFrame
{
    Q_OBJECT

public:
    PlaybackName(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const QString & name, const QDateTime & start, 
        const QDateTime & end);

protected:
    virtual void createPlayback() override;

private:
    virtual void on_download_clicked() override;

private:
    const QString myName;
};
