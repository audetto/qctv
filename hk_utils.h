#pragma once

#include <string>

inline char * cast(const char * c)
{
  return const_cast<char*>(c);
}

inline char * cast(const std::string & s)
{
  return cast(s.c_str());
}
