#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "sdk/hk_dvr.h"
#include "sdk/hk_sdk.h"
#include "sdk/hk_error.h"
#include "liveframe.h"
#include "playbackframe.h"
#include "downloadframe.h"
#include "abilityviewer.h"
#include "utils.h"

#include <QMdiSubWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , myDVR(dvr)
{
    ui->setupUi(this);

    QIcon icon(QIcon::fromTheme(":/resources/hk.ico"));
    this->setWindowIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionLive_triggered()
{
    if (ui->action1->isChecked())
    {
        livePlay(0);
    }
    if (ui->action2->isChecked())
    {
        livePlay(1);
    }
}

void MainWindow::livePlay(const size_t channel)
{
    LiveFrame * frame = new LiveFrame();

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    const QString title = QString("Live Camera %1").arg(1 + channel);
    window->setWindowTitle(title);
    window->show();

    WId id = frame->getWindowHandle();
    try
    {
        frame->setPlay(myDVR->getLivePlayer(channel, id));
    }
    catch (const HK_Error & error)
    {
        showError(this, error);
    }
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_triggered()
{
    const QString sdkVersion = QString::fromStdString(HK_SDK::getSDKVersion());
    const QString playerVersion = QString::fromStdString(HK_SDK::getPlayerVersion());
    const QString message = QString("HCNetSDK\tV%1\nPlayCtrl\tV%2").arg(sdkVersion, playerVersion);
    QMessageBox::about(this, QApplication::applicationName(), message);
}

void MainWindow::on_actionPlayback_triggered()
{
    if (ui->action1->isChecked())
    {
        playback(0);
    }
    if (ui->action2->isChecked())
    {
        playback(1);
    }
}

void MainWindow::playback(const size_t channel)
{
    PlaybackFrame * frame = new PlaybackFrame(nullptr, myDVR, channel);
    QObject::connect(frame, &PlaybackFrame::downloadOnChannel, this, &MainWindow::openDownloadOnChannel);

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    const QString title = QString("Playback Camera %1").arg(1 + channel);
    window->setWindowTitle(title);
    window->show();
}

void MainWindow::on_actionDownload_triggered()
{
    const QDateTime start = QDateTime::currentDateTime().addDays(-1);
    openDownloadOnChannel(std::nullopt, start);
}

void MainWindow::openDownloadOnChannel(const std::optional<size_t> channel, const QDateTime & start)
{
    DownloadFrame * frame = new DownloadFrame(nullptr, myDVR, channel, start);

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    window->setWindowTitle("Download video");
    window->show();
}

void MainWindow::on_actionAbilities_triggered()
{
    AbilityViewer * frame = new AbilityViewer(nullptr, myDVR);

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    window->setWindowTitle("Ability viewer");
    window->show();
}
