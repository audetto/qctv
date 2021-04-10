#include "playframe.h"
#include "ui_playframe.h"

#include "hk_play.h"
#include "hk_error.h"
#include "utils.h"

#include <QSettings>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>

PlayFrame::PlayFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PlayFrame)
{
    ui->setupUi(this);
}

PlayFrame::~PlayFrame()
{
    delete ui;
}

void PlayFrame::resizeEvent(QResizeEvent *event)
{
    QFrame::resizeEvent(event);
    if (myPlay)
    {
        myPlay->resize();
    }
}

void PlayFrame::setPlay(const std::shared_ptr<HK_Play> & play)
{
    myPlay = play;
}

bool PlayFrame::event(QEvent *event)
{
    if (event->type() == QEvent::WinIdChange)
    {
        // I know of no way to tell NET_DVR that the window id has changed
        myPlay.reset();
    }
    return QFrame::event(event);
}

WId PlayFrame::getWindowHandle() const
{
    return ui->widget->winId();
}

void PlayFrame::on_snapshot_clicked()
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
    QString filename = QString("C%1_").arg(myPlay->getChannel()) + now.toString("yyyy-MM-dd_hh-mm-ss") + QString(".bmp");
    QString path = dir.filePath(filename);
    try
    {
        myPlay->snapshot(path.toStdString());
        QMessageBox::information(this, "Snapshot", QString("Snapshot saved to %1").arg(path));
    }
    catch (const HK_Error & error)
    {
        showError(this, error);
    }
}
