#include "utils.h"

#include "sdk/hk_error.h"

#include <QMessageBox>
#include <QDateTime>

void showError(QWidget *parent, const HK_Error & error)
{
    const QString msg = QString("Last error = %1\n%2").arg(QString::number(error.getError()), QString::fromUtf8(error.getMessage()));
    QMessageBox::critical(parent, QString::fromUtf8(error.what()), msg);
}

NET_DVR_TIME_V50 qDateTime2NetDVR(const QDateTime & datetime)
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

QDateTime netDVR2QDateTime(const NET_DVR_TIME & dvr)
{
    const QDate date(dvr.dwYear, dvr.dwMonth, dvr.dwDay);
    const QTime time(dvr.dwHour, dvr.dwMinute, dvr.dwSecond);
    return QDateTime(date, time);
}
