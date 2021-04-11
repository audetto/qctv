#include "liveframe.h"
#include "ui_liveframe.h"

#include "hk_liveplayer.h"
#include "hk_error.h"
#include "utils.h"

#include <QSettings>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>

LiveFrame::LiveFrame(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::LiveFrame)
    , myTimer(0)
{
    ui->setupUi(this);
}

LiveFrame::~LiveFrame()
{
    delete ui;
}

void LiveFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if (myLivePlayer)
    {
        myLivePlayer->resize();
    }
}

void LiveFrame::setPlay(const std::shared_ptr<HK_LivePlayer> & play)
{
    myLivePlayer = play;
}

bool LiveFrame::event(QEvent *event)
{
    if (event->type() == QEvent::WinIdChange)
    {
        // I know of no way to tell NET_DVR that the window id has changed
        myLivePlayer.reset();
    }
    return QFrame::event(event);
}

WId LiveFrame::getWindowHandle() const
{
    return ui->widget->winId();
}

void LiveFrame::on_picture_clicked()
{
    QSettings settings;
    QDir dir = settings.value("output/pictures", "/tmp/qctv").toString();
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            return;
        }
    }

    QDateTime now = QDateTime::currentDateTime();
    QString filename = QString("C%1_").arg(myLivePlayer->getChannel()) + now.toString("yyyy-MM-dd_hh-mm-ss") + QString(".bmp");
    QString path = dir.filePath(filename);
    try
    {
        myLivePlayer->snapshot(path.toStdString());
        ui->status->setText(path);
    }
    catch (const HK_Error & error)
    {
        showError(this, error);
    }
}

void LiveFrame::timerEvent(QTimerEvent *)
{
    const qint64 msec = myStartTime.msecsTo(QDateTime::currentDateTime());
    const QTime time = QTime::fromMSecsSinceStartOfDay(msec);
    ui->time->setText(time.toString());
}

void LiveFrame::on_record_clicked()
{
    ui->record->setEnabled(false);
    ui->stop->setEnabled(true);

    QSettings settings;
    QDir dir = settings.value("output/videos", "/tmp/qctv").toString();
    if (!dir.exists())
    {
        if (!dir.mkpath("."))
        {
            return;
        }
    }

    QDateTime now = QDateTime::currentDateTime();
    QString filename = QString("C%1_").arg(myLivePlayer->getChannel()) + now.toString("yyyy-MM-dd_hh-mm-ss") + QString(".mp4");
    QString path = dir.filePath(filename);
    try
    {
        myLivePlayer->record(path.toStdString());
        ui->status->setText(path);
        myTimer = startTimer(1000);
        myStartTime = QDateTime::currentDateTime();
    }
    catch (const HK_Error & error)
    {
        showError(this, error);
    }
}

void LiveFrame::on_stop_clicked()
{
    ui->record->setEnabled(true);
    ui->stop->setEnabled(false);
    myLivePlayer->stop();
    killTimer(myTimer);
    myTimer = 0;
}
