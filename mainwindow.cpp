#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hk_dvr.h"
#include "hk_sdk.h"
#include "hk_error.h"
#include "liveframe.h"
#include "playbackframe.h"
#include "utils.h"

#include <QMdiSubWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr)
    : QMainWindow(parent)
    , myDVR(dvr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon(QIcon::fromTheme(":/hk.ico"));
    this->setWindowIcon(icon);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPlay_triggered()
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

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    const QString title = QString("Playback Camera %1").arg(1 + channel);
    window->setWindowTitle(title);
    window->show();

}
