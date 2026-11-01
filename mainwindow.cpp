#include "mainwindow.h"
#include "abilityviewer.h"
#include "downloadframe.h"
#include "filebrowser.h"
#include "liveframe.h"
#include "playbackname.h"
#include "playbacktime.h"
#include "sdk/hk_dvr.h"
#include "sdk/hk_error.h"
#include "sdk/hk_sdk.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include <QMdiSubWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, const std::shared_ptr<HK_DVR> &dvr)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , myDVR(dvr)
{
    ui->setupUi(this);

    QIcon icon(":/resources/hk.ico");
    this->setWindowIcon(icon);
    createCameraActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createCameraActions()
{
    QAction *before = ui->menuCameras->actions().value(0);

    try
    {
        myCameras = getOnlineCameras(*myDVR);
    }
    catch (const HK_Error &error)
    {
        showError(this, error);
    }

    for (const Camera &camera : myCameras)
    {
        QAction *action = new QAction(camera.name, ui->menuCameras);
        action->setCheckable(true);
        action->setData(QVariant::fromValue<qulonglong>(camera.channel));
        ui->menuCameras->insertAction(before, action);
    }
}

std::vector<size_t> MainWindow::getCheckedCameras() const
{
    std::vector<size_t> checkedCameras;
    const QList<QAction *> actions = ui->menuCameras->actions();
    for (QAction *action : actions)
    {
        if (action->isChecked() && action->data().isValid())
        {
            const size_t channel = action->data().toULongLong();
            checkedCameras.push_back(channel);
        }
    }
    return checkedCameras;
}

void MainWindow::on_actionLive_triggered()
{
    for (const size_t channel : getCheckedCameras())
    {
        livePlay(channel);
    }
}

void MainWindow::livePlay(const size_t channel)
{
    LiveFrame *frame = new LiveFrame();

    QMdiSubWindow *window = ui->mdiArea->addSubWindow(frame);

    const bool subStream = ui->actionSubStream->isChecked();
    const QString title =
        QString("Live: %1 (%2)").arg(getCameraName(channel), QLatin1String(subStream ? "sub" : "main"));
    window->setWindowTitle(title);
    window->show();

    WId id = frame->getWindowHandle();
    const DWORD streamType = subStream ? 1 : 0;
    try
    {
        frame->setPlay(myDVR->getLivePlayer(channel, streamType, id));
    }
    catch (const HK_Error &error)
    {
        showError(this, error);
        window->close();
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
    for (const size_t channel : getCheckedCameras())
    {
        playback(channel);
    }
}

void MainWindow::playback(const size_t channel)
{
    PlaybackTime *frame = new PlaybackTime(nullptr, myDVR, channel);
    QObject::connect(frame, &PlaybackTime::downloadOnChannel, this, &MainWindow::openDownloadOnChannel);

    QMdiSubWindow *window = ui->mdiArea->addSubWindow(frame);
    const QString title = QString("Playback: %1").arg(getCameraName(channel));
    window->setWindowTitle(title);
    window->show();
}

void MainWindow::on_actionDownload_triggered()
{
    const QDateTime start = QDateTime::currentDateTime().addDays(-1);
    openDownloadOnChannel(std::nullopt, start);
}

void MainWindow::openDownloadOnChannel(const std::optional<size_t> channel, const QDateTime &start)
{
    DownloadFrame *frame = new DownloadFrame(nullptr, myDVR, myCameras, channel, start);

    QMdiSubWindow *window = ui->mdiArea->addSubWindow(frame);

    window->setWindowTitle("Download video");
    window->show();
}

void MainWindow::on_actionAbilities_triggered()
{
    AbilityViewer *frame = new AbilityViewer(nullptr, myDVR, myCameras);

    QMdiSubWindow *window = ui->mdiArea->addSubWindow(frame);

    window->setWindowTitle("Ability viewer");
    window->show();
}

void MainWindow::on_actionBrowse_triggered()
{
    FileBrowser *frame = new FileBrowser(nullptr, myDVR, myCameras);

    QObject::connect(frame, &FileBrowser::playbackByName, this, &MainWindow::openPlaybackByName);

    QMdiSubWindow *window = ui->mdiArea->addSubWindow(frame);
    window->setWindowTitle("File browser");
    window->show();
}

void MainWindow::openPlaybackByName(const QString &name, const QDateTime &start, const QDateTime &end)
{
    PlaybackName *frame = new PlaybackName(nullptr, myDVR, name, start, end);

    QMdiSubWindow *window = ui->mdiArea->addSubWindow(frame);

    window->setWindowTitle(name);
    window->show();
}

QString MainWindow::getCameraName(const size_t channel) const
{
    for (const Camera &camera : myCameras)
    {
        if (camera.channel == channel)
        {
            return camera.name;
        }
    }
    return QString("Camera %1").arg(1 + channel);
}
