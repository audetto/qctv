#include "filebrowser.h"
#include "HCNetSDK.h"
#include "sdk/hk_dvr.h"
#include "sdk/hk_error.h"
#include "ui_filebrowser.h"
#include "utils.h"
#include <QDateTime>
#include <QLocale>

FileBrowser::FileBrowser(QWidget *parent, const std::shared_ptr<HK_DVR> &dvr, const std::vector<Camera> &cameras)
    : QWidget(parent)
    , ui(new Ui::FileBrowser)
    , myDVR(dvr)
{
    ui->setupUi(this);
    fillCameraComboBox(ui->channel, cameras);

    const QDateTime start = QDateTime::currentDateTime().addDays(-1);
    ui->startDateTime->setDateTime(start);

    const std::chrono::milliseconds duration = std::chrono::minutes(60);
    const QDateTime end = start.addDuration(duration);
    ui->endDateTime->setDateTime(end);
}

FileBrowser::~FileBrowser()
{
    delete ui;
}

void FileBrowser::on_search_clicked()
{
    if (ui->channel->currentIndex() >= 0)
    {
        const size_t channel = ui->channel->currentData().toULongLong();

        try
        {
            const QDateTime start = ui->startDateTime->dateTime();
            const QDateTime end = ui->endDateTime->dateTime();

            const std::vector<NET_DVR_FINDDATA_V50> files =
                myDVR->findFiles(channel, qDateTime2NetDVRSearchCond(start), qDateTime2NetDVRSearchCond(end));

            ui->table->clearContents();
            ui->table->setRowCount(0);

            QLocale locale;

            for (const auto &value : files)
            {
                const int row = ui->table->rowCount();
                ui->table->setRowCount(row + 1);

                int col = 0;
                QTableWidgetItem *nameItem = new QTableWidgetItem(value.sFileName);
                nameItem->setData(Qt::UserRole, QVariant::fromValue(value));
                ui->table->setItem(row, col++, nameItem);

                const QString startTime = netDVR2QDateTime(value.struStartTime).toString();
                QTableWidgetItem *startItem = new QTableWidgetItem(startTime);
                ui->table->setItem(row, col++, startItem);

                const QString endTime = netDVR2QDateTime(value.struStopTime).toString();
                QTableWidgetItem *endItem = new QTableWidgetItem(endTime);
                ui->table->setItem(row, col++, endItem);

                const QString type = QString::number(value.byFileType);
                QTableWidgetItem *typeItem = new QTableWidgetItem(type);
                ui->table->setItem(row, col++, typeItem);

                const QString size = locale.toString(value.dwFileSize);
                QTableWidgetItem *sizeItem = new QTableWidgetItem(size);
                ui->table->setItem(row, col++, sizeItem);
            }

            ui->table->resizeColumnsToContents();
        }
        catch (const HK_Error &error)
        {
            showError(this, error);
        }
    }
}

void FileBrowser::on_open_clicked()
{
    const int row = ui->table->currentRow();
    if (row >= 0)
    {
        const QTableWidgetItem *item = ui->table->item(row, 0);
        const NET_DVR_FINDDATA_V50 data = item->data(Qt::UserRole).value<NET_DVR_FINDDATA_V50>();
        const QString name(data.sFileName);
        const QDateTime start = netDVR2QDateTime(data.struStartTime);
        const QDateTime end = netDVR2QDateTime(data.struStopTime);
        emit playbackByName(name, start, end);
    }
}
