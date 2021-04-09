#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hk_dvr.h"

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
    QFrame * widget = new QFrame();
    widget->setMinimumSize(560, 384);
    QMdiSubWindow * window = ui->mdiArea->addSubWindow(widget);
    window->setWindowTitle("Camera");
    window->show();
    WId id = widget->winId();
    myPlay = myDVR->getPlayer(0, id);
    // not good, design upside down!
    if (!myPlay)
    {
        show_error("NET_DVR_RealPlay_V40");
    }
}

void MainWindow::show_error(const char * function)
{
    const QString msg = QString("Last error = %1").arg(NET_DVR_GetLastError());
    QMessageBox::critical(this, QString::fromUtf8(function), msg);
}
