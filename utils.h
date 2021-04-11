#ifndef UTILS_H
#define UTILS_H

#include "HCNetSDK.h"

#include <QString>
#include <string>

class HK_Error;
class QWidget;
class QDateTime;

inline char * cast(const char * c)
{
  return const_cast<char*>(c);
}

inline char * cast(const std::string & s)
{
  return cast(s.c_str());
}

inline char * cast(const QString & s)
{
  return cast(s.toStdString());
}

void showError(QWidget *parent, const HK_Error & error);

NET_DVR_TIME_V50 qDateTime2NetDVR(const QDateTime & datetime);

QDateTime netDVR2QDateTime(const NET_DVR_TIME & dvr);

#endif // UTILS_H
