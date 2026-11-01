#include "playbacktime.h"
#include "sdk/hk_dvr.h"
#include "sdk/hk_playback.h"
#include "ui_playbackframe.h"
#include "utils.h"

PlaybackTime::PlaybackTime(QWidget *parent, const std::shared_ptr<HK_DVR> &dvr, const size_t channel)
    : PlaybackFrame(parent, dvr)
    , myChannel(channel)
{
}

void PlaybackTime::createPlayback()
{
    myOrgStart = ui->startDateTime->dateTime();
    const std::chrono::milliseconds duration = std::chrono::minutes(30);
    myOrgEnd = myOrgStart.addDuration(duration);

    const BYTE byPlayMode = ui->forward->isChecked() ? 0 : 1;
    myPlayback = myDVR->getPlayback(myChannel, getWindowHandle(), nullptr, qDateTime2NetDVR50(myOrgStart),
                                    qDateTime2NetDVR50(myOrgEnd), byPlayMode);
}

void PlaybackTime::on_download_clicked()
{
    emit downloadOnChannel(myChannel, ui->startDateTime->dateTime());
}
