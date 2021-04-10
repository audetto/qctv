#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <string>

class HK_Error;
class QWidget;

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


#endif // UTILS_H
