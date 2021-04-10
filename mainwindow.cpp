#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hk_dvr.h"
#include "hk_error.h"
#include "playframe.h"

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
    PlayFrame * frame = new PlayFrame();
    frame->setMinimumSize(400, 225);

    QMdiSubWindow * window = ui->mdiArea->addSubWindow(frame);
    window->setWindowTitle("Camera");
    window->show();

    WId id = frame->winId();
    try
    {
        frame->setPlay(myDVR->getPlayer(0, id));
    }
    catch (const HK_Error & error)
    {
        show_error(error);
    }
}

void MainWindow::show_error(const HK_Error & error)
{
    const QString msg = QString("Last error = %1\n%2").arg(QString::number(error.getError()), QString::fromUtf8(error.getMessage()));
    QMessageBox::critical(this, QString::fromUtf8(error.what()), msg);
}
