#include "utils.h"

#include "sdk/hk_dvr.h"
#include "sdk/hk_error.h"

#include <QComboBox>
#include <QDateTime>
#include <QMessageBox>

std::vector<Camera> getOnlineCameras(const HK_DVR &dvr)
{
    std::vector<Camera> cameras;
    const std::vector<HK_IPChannel> ipChannels = dvr.getIPChannels();
    for (size_t i = 0; i < ipChannels.size(); ++i)
    {
        const HK_IPChannel &channel = ipChannels[i];
        if (channel.online)
        {
            const QString name =
                channel.name.empty() ? QString("Camera %1").arg(1 + i) : QString::fromStdString(channel.name);
            cameras.emplace_back(i, name);
        }
    }

    return cameras;
}

void fillCameraComboBox(QComboBox *comboBox, const std::vector<Camera> &cameras)
{
    comboBox->clear();
    for (const Camera &camera : cameras)
    {
        comboBox->addItem(camera.name, QVariant::fromValue<qulonglong>(camera.channel));
    }
}

void showError(QWidget *parent, const HK_Error &error)
{
    const QString msg = QString("%1\nError: %2\n%3")
                            .arg(QString::fromUtf8(error.getFunction()), QString::number(error.getError()),
                                 QString::fromStdString(error.getHKMessage()));

    QMessageBox::critical(parent, "Error", msg);
}

NET_DVR_TIME_V50 qDateTime2NetDVR50(const QDateTime &datetime)
{
    NET_DVR_TIME_V50 dvrTime = {};

    const QDate &date = datetime.date();
    dvrTime.wYear = date.year();
    dvrTime.byMonth = date.month();
    dvrTime.byDay = date.day();

    const QTime &time = datetime.time();
    dvrTime.byHour = time.hour();
    dvrTime.byMinute = time.minute();
    dvrTime.bySecond = time.second();

    return dvrTime;
}

NET_DVR_TIME qDateTime2NetDVR(const QDateTime &datetime)
{
    NET_DVR_TIME dvrTime = {};

    const QDate &date = datetime.date();
    dvrTime.dwYear = date.year();
    dvrTime.dwMonth = date.month();
    dvrTime.dwDay = date.day();

    const QTime &time = datetime.time();
    dvrTime.dwHour = time.hour();
    dvrTime.dwMinute = time.minute();
    dvrTime.dwSecond = time.second();

    return dvrTime;
}

NET_DVR_TIME_EX qDateTime2NetDVREX(const QDateTime &datetime)
{
    NET_DVR_TIME_EX dvrTime = {};

    const QDate &date = datetime.date();
    dvrTime.wYear = date.year();
    dvrTime.byMonth = date.month();
    dvrTime.byDay = date.day();

    const QTime &time = datetime.time();
    dvrTime.byHour = time.hour();
    dvrTime.byMinute = time.minute();
    dvrTime.bySecond = time.second();

    return dvrTime;
}

NET_DVR_TIME_SEARCH_COND qDateTime2NetDVRSearchCond(const QDateTime &datetime)
{
    NET_DVR_TIME_SEARCH_COND dvrTime = {};

    const QDate &date = datetime.date();
    dvrTime.wYear = date.year();
    dvrTime.byMonth = date.month();
    dvrTime.byDay = date.day();

    const QTime &time = datetime.time();
    dvrTime.byHour = time.hour();
    dvrTime.byMinute = time.minute();
    dvrTime.bySecond = time.second();

    return dvrTime;
}

QDateTime netDVR2QDateTime(const NET_DVR_TIME &dvr)
{
    const QDate date(dvr.dwYear, dvr.dwMonth, dvr.dwDay);
    const QTime time(dvr.dwHour, dvr.dwMinute, dvr.dwSecond);
    return QDateTime(date, time);
}

QDateTime netDVR2QDateTime(const NET_DVR_TIME_SEARCH &dvr)
{
    const QDate date(dvr.wYear, dvr.byMonth, dvr.byDay);
    const QTime time(dvr.byHour, dvr.byMinute, dvr.bySecond);
    return QDateTime(date, time);
}
