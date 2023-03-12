#include "playbackframe.h"
#include "ui_playbackframe.h"

#include "sdk/hk_dvr.h"
#include "sdk/hk_playback.h"
#include "sdk/hk_error.h"

#include "utils.h"

PlaybackFrame::PlaybackFrame(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const size_t channel)
    : QFrame(parent)
    , ui(new Ui::PlaybackFrame)
    , myDVR(dvr)
    , myChannel(channel)
    , myTimer(0)
    , myLogSpeed(0)
{
    ui->setupUi(this);
    const QDateTime start = QDateTime::currentDateTime().addDays(-1);
    ui->startDateTime->setDateTime(start);
    ui->speed->setText(getLogSpeedString());
}

PlaybackFrame::~PlaybackFrame()
{
    delete ui;
}

WId PlaybackFrame::getWindowHandle() const
{
    return ui->widget->winId();
}

void PlaybackFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if (myPlayback)
    {
        try
        {
            myPlayback->resize();
        }
        catch (const HK_Error & error)
        {
            showError(this, error);
        }
    }
}

void PlaybackFrame::on_play_clicked()
{
    myPlayback.reset();
    killTimer(myTimer);
    myTimer = 0;
    myLogSpeed = 0;

    myOrgStart = ui->startDateTime->dateTime();
    const QDateTime end = myOrgStart.addMSecs(1000 * 600);

    try
    {
        if (ui->forward->isChecked())
        {
            myPlayback = myDVR->getPlayback(myChannel, getWindowHandle(), 
                qDateTime2NetDVR50(myOrgStart), qDateTime2NetDVR50(end),
                nullptr);
        }
        else
        {
            myPlayback = myDVR->getReversePlayback(myChannel, getWindowHandle(), 
                qDateTime2NetDVR(myOrgStart), qDateTime2NetDVR(end));
        }

        command(NET_DVR_PLAYSTART);
        myTimer = startTimer(1000);

        ui->progress->setMaximum(myOrgStart.secsTo(end));
        ui->progress->setValue(0);

        syncControls(false);
    }
    catch (const HK_Error & error)
    {
        resetPlayback();
        showError(this, error);
    }
}

bool PlaybackFrame::command(const DWORD cmd)
{
    if (myPlayback)
    {
        try
        {
            myPlayback->command(cmd);
            return true;
        }
        catch (const HK_Error & error)
        {
            showError(this, error);
            return false;
        }
    }
    else
    {
        return false;
    }
}

void PlaybackFrame::on_stop_clicked()
{
    resetPlayback();
}

void PlaybackFrame::resetPlayback()
{
    syncControls(true);

    myPlayback.reset();
    killTimer(myTimer);
    myTimer = 0;
}

void PlaybackFrame::timerEvent(QTimerEvent *)
{
    if (myPlayback)
    {
        try
        {
            const QDateTime osd = netDVR2QDateTime(myPlayback->getOSDTime());
            ui->progress->setValue(myOrgStart.secsTo(osd));
            ui->startDateTime->setDateTime(osd);
            ui->speed->setText(getLogSpeedString());
        }
        catch (const HK_Error & error)
        {
            resetPlayback();
            showError(this, error);
        }
    }
}

QString PlaybackFrame::getLogSpeedString() const
{
    const QString prefix(myLogSpeed < 0 ? "/" : "x");
    const QString s = prefix + QString::number(1 << std::abs(myLogSpeed));
    return s;
}

void PlaybackFrame::on_fast_clicked()
{
    if (command(NET_DVR_PLAYFAST))
    {
        ++myLogSpeed;
    }
}

void PlaybackFrame::on_slow_clicked()
{
    if (command(NET_DVR_PLAYSLOW))
    {
        --myLogSpeed;
    }
}

void PlaybackFrame::on_normal_clicked()
{
    if (command(NET_DVR_PLAYNORMAL))
    {
        myLogSpeed = 0;
    }
}

void PlaybackFrame::on_pause_clicked()
{
    command(NET_DVR_PLAYPAUSE);
}

void PlaybackFrame::on_step_clicked()
{
    command(NET_DVR_PLAYFRAME);
}

void PlaybackFrame::on_download_clicked()
{
    emit downloadOnChannel(myChannel, ui->startDateTime->dateTime());
}

void PlaybackFrame::syncControls(const bool enabled)
{
    ui->play->setEnabled(false != enabled);
    ui->startDateTime->setEnabled(false != enabled);
    ui->direction->setEnabled(false != enabled);

    ui->stop->setEnabled(true != enabled);
    ui->fast->setEnabled(true != enabled);
    ui->slow->setEnabled(true != enabled);
    ui->pause->setEnabled(true != enabled);
    ui->step->setEnabled(true != enabled);
    ui->normal->setEnabled(true != enabled);
}