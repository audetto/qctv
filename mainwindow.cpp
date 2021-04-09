#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "hk_dvr.h"
#include "hk_error.h"

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
    try
    {
        myPlay = myDVR->getPlayer(0, id);
    }
    catch (const HK_Error & error)
    {
        show_error(error);
    }
}

void MainWindow::show_error(const HK_Error & error)
{
    const QString msg = QString("Last error = %1").arg(error.getError());
    QMessageBox::critical(this, QString::fromUtf8(error.what()), msg);
}
