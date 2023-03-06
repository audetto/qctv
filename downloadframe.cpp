#include "downloadframe.h"
#include "ui_downloadframe.h"

DownloadFrame::DownloadFrame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadFrame)
{
    ui->setupUi(this);
}

DownloadFrame::~DownloadFrame()
{
    delete ui;
}
