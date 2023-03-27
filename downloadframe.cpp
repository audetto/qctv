#include "downloadframe.h"
#include "ui_downloadframe.h"

#include "sdk/hk_playback.h"
#include "sdk/hk_dvr.h"
#include "sdk/hk_error.h"
#include "utils.h"
#include "receiver.h"

#include <memory>
#include <QStandardPaths>
#include <QMessageBox>
#include <QCloseEvent>


DownloadFrame::DownloadFrame(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr, const std::optional<size_t> channel, const QDateTime & start)
    : QWidget(parent)
    , ui(new Ui::DownloadFrame)
    , myDVR(dvr)
    , myTimer(0)
    , mySaveFileDialog(this)
{
    ui->setupUi(this);

    if (channel)
    {
        ui->camera->setCurrentIndex(*channel);
    }

    ui->startDateTime->setDateTime(start);

    const QDateTime end = start.addMSecs(1000 * 60 * 10);  // 10 minute
    ui->endDateTime->setDateTime(end);

    mySaveFileDialog.setDirectory(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    mySaveFileDialog.setNameFilter("*.mp4");
}

DownloadFrame::~DownloadFrame()
{
    delete ui;
}

void DownloadFrame::on_startButton_clicked()
{
    const size_t channel = ui->camera->currentIndex();
    const QDateTime start = ui->startDateTime->dateTime();
    const QDateTime end = ui->endDateTime->dateTime();

    try
    {
        const auto & rec = std::make_shared<Receiver>(ui->path->text());
        QObject::connect(rec.get(), &Receiver::dataReceived, this, &DownloadFrame::addData);

        mySize = 0;
        myPlayback = myDVR->getPlayback(channel, 0, rec, qDateTime2NetDVR50(start), qDateTime2NetDVR50(end));
        myPlayback->command(NET_DVR_PLAYSTART);
        myTimer = startTimer(250);

        ui->status->setText("Downloading");
        ui->progress->setMaximum(0);
        ui->progress->setValue(0);
        ui->startDateTime->setEnabled(false);
        ui->endDateTime->setEnabled(false);
        ui->camera->setEnabled(false);

        ui->startButton->setEnabled(false);
        ui->stopButton->setEnabled(true);
    }
    catch (const HK_Error & error)
    {
        resetDownload();
        showError(this, error);
    }
    catch (const std::exception & error)
    {
        resetDownload();
        QMessageBox::critical(this, "File error", error.what());
    }

}

void DownloadFrame::on_stopButton_clicked()
{
    resetDownload();
    ui->status->setText("Interrupted");
}

void DownloadFrame::resetDownload()
{
    ui->startDateTime->setEnabled(true);
    ui->endDateTime->setEnabled(true);
    ui->camera->setEnabled(true);

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);

    myPlayback.reset();
    killTimer(myTimer);
    myTimer = 0;
}

void DownloadFrame::timerEvent(QTimerEvent * event)
{
    if (myPlayback)
    {
        if (mySize >= ui->progress->maximum())
        {
            ui->progress->setMaximum(mySize * 3);
        }
        ui->progress->setValue(mySize);
        ui->size->setText(QString("%L1").arg(mySize));

        const size_t pos = myPlayback->getDownloadPos();
        if (pos == 100)
        {
            ui->progress->setValue(ui->progress->maximum());
            ui->status->setText("Done");
            resetDownload();
        }
        else if (pos == 200)
        {
            ui->status->setText("Error");
            resetDownload();
        }
    }
}

void DownloadFrame::closeEvent(QCloseEvent *event)
{
    if (myPlayback)
    {
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void DownloadFrame::addData(size_t size)
{
    mySize += size;
}

void DownloadFrame::on_browse_clicked()
{
    const QString fileName = mySaveFileDialog.getSaveFileName();
    ui->path->setText(fileName);
}
