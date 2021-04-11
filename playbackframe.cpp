#include "playbackframe.h"
#include "ui_playbackframe.h"

#include "hk_dvr.h"
#include "hk_playback.h"
#include "hk_error.h"

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
    ui->dateTimeEdit->setDateTime(start);
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
        myPlayback->resize();
    }
}

void PlaybackFrame::on_play_clicked()
{
    myPlayback.reset();
    killTimer(myTimer);
    myTimer = 0;
    myLogSpeed = 0;

    const QDateTime start = ui->dateTimeEdit->dateTime();
    const QDateTime end = start.addMSecs(1000 * 3600);

    try
    {
        myPlayback = myDVR->getPlayback(myChannel, getWindowHandle(), start, end);
        command(NET_DVR_PLAYSTART);
        myTimer = startTimer(1000);

        ui->play->setEnabled(false);
        ui->dateTimeEdit->setEnabled(false);

        ui->stop->setEnabled(true);
        ui->fast->setEnabled(true);
        ui->slow->setEnabled(true);
        ui->pause->setEnabled(true);
        ui->step->setEnabled(true);
        ui->normal->setEnabled(true);
    }
    catch (const HK_Error & error)
    {
        showError(this, error);
    }
}

void PlaybackFrame::command(const DWORD cmd)
{
    if (myPlayback)
    {
        try
        {
            myPlayback->command(cmd);
        }
        catch (const HK_Error & error)
        {
            showError(this, error);
        }
    }
}

void PlaybackFrame::on_stop_clicked()
{
    ui->play->setEnabled(true);
    ui->dateTimeEdit->setEnabled(true);

    ui->stop->setEnabled(false);
    ui->fast->setEnabled(false);
    ui->slow->setEnabled(false);
    ui->pause->setEnabled(false);
    ui->step->setEnabled(false);
    ui->normal->setEnabled(false);

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
            ui->dateTimeEdit->setDateTime(osd);
            ui->speed->setText(getLogSpeedString());
        }
        catch (const HK_Error & error)
        {
            showError(this, error);
            killTimer(myTimer);
            myTimer = 0;
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
    command(NET_DVR_PLAYFAST);
    ++myLogSpeed;
}

void PlaybackFrame::on_slow_clicked()
{
    command(NET_DVR_PLAYSLOW);
    --myLogSpeed;
}

void PlaybackFrame::on_normal_clicked()
{
    command(NET_DVR_PLAYNORMAL);
    myLogSpeed = 0;
}

void PlaybackFrame::on_pause_clicked()
{
    command(NET_DVR_PLAYPAUSE);
}

void PlaybackFrame::on_step_clicked()
{
    command(NET_DVR_PLAYFRAME);
}
