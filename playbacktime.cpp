#include "playbacktime.h"
#include "ui_playbackframe.h"

#include "sdk/hk_dvr.h"
#include "sdk/hk_playback.h"

#include "utils.h"

PlaybackTime::PlaybackTime(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const size_t channel)
    : PlaybackFrame(parent, dvr)
    , myChannel(channel)
{

}

void PlaybackTime::createPlayback()
{
    myOrgStart = ui->startDateTime->dateTime();
    myOrgEnd = myOrgStart.addMSecs(1000 * 600);

    if (ui->forward->isChecked())
    {
        myPlayback = myDVR->getPlayback(myChannel, getWindowHandle(), 
            qDateTime2NetDVR50(myOrgStart), qDateTime2NetDVR50(myOrgEnd),
            nullptr);
    }
    else
    {
        myPlayback = myDVR->getReversePlayback(myChannel, getWindowHandle(), 
            qDateTime2NetDVR(myOrgStart), qDateTime2NetDVR(myOrgEnd));
    }
}

void PlaybackTime::on_download_clicked()
{
    emit downloadOnChannel(myChannel, ui->startDateTime->dateTime());
}
