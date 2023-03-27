#include "playbackname.h"
#include "ui_playbackframe.h"

#include "sdk/hk_dvr.h"
#include "sdk/hk_playback.h"


PlaybackName::PlaybackName(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const QString & name, 
    const QDateTime & start, const QDateTime & end)
    : PlaybackFrame(parent, dvr)
    , myName(name)
{
    myOrgStart = start;
    myOrgEnd = end;
}

void PlaybackName::createPlayback()
{
    if (ui->forward->isChecked())
    {
        myPlayback = myDVR->getPlayback(myName.toStdString(), getWindowHandle(), nullptr);
    }
    else
    {
        myPlayback = myDVR->getReversePlayback(myName.toStdString(), getWindowHandle(), nullptr);
    }
}

void PlaybackName::on_download_clicked()
{
}
