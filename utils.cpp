#include "utils.h"

#include "sdk/hk_error.h"

#include <QMessageBox>
#include <QDateTime>

void showError(QWidget *parent, const HK_Error & error)
{
    const QString msg = QString("%1\nError: %2\n%3").arg(
        QString::fromUtf8(error.getFunction()),
        QString::number(error.getError()), 
        QString::fromStdString(error.getHKMessage()));

    QMessageBox::critical(parent, "Error", msg);
}

NET_DVR_TIME_V50 qDateTime2NetDVR50(const QDateTime & datetime)
{
    NET_DVR_TIME_V50 dvrTime = {};

    const QDate & date = datetime.date();
    dvrTime.wYear = date.year();
    dvrTime.byMonth = date.month();
    dvrTime.byDay = date.day();

    const QTime & time = datetime.time();
    dvrTime.byHour = time.hour();
    dvrTime.byMinute = time.minute();
    dvrTime.bySecond = time.second();

    return dvrTime;
}

NET_DVR_TIME qDateTime2NetDVR(const QDateTime & datetime)
{
    NET_DVR_TIME dvrTime = {};

    const QDate & date = datetime.date();
    dvrTime.dwYear = date.year();
    dvrTime.dwMonth = date.month();
    dvrTime.dwDay = date.day();

    const QTime & time = datetime.time();
    dvrTime.dwHour = time.hour();
    dvrTime.dwMinute = time.minute();
    dvrTime.dwSecond = time.second();

    return dvrTime;
}

QDateTime netDVR2QDateTime(const NET_DVR_TIME & dvr)
{
    const QDate date(dvr.dwYear, dvr.dwMonth, dvr.dwDay);
    const QTime time(dvr.dwHour, dvr.dwMinute, dvr.dwSecond);
    return QDateTime(date, time);
}
