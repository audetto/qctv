#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hk_dvr.h"
#include "hk_error.h"
#include "playframe.h"
#include "utils.h"

#include <QMdiSubWindow>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent, const std::shared_ptr<HK_DVR> & dvr)
    : QMainWindow(parent)
    , myDVR(dvr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionPlay_triggered()
{
    const size_t channel = 1;
    PlayFrame * frame = new PlayFrame();

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    const QString title = QString("Live Camera %1").arg(channel);
    window->setWindowTitle(title);
    window->show();

    WId id = frame->getWindowHandle();
    try
    {
        frame->setPlay(myDVR->getPlayer(channel, id));
    }
    catch (const HK_Error & error)
    {
        showError(this, error);
    }
}

