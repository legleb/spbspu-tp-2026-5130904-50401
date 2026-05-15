#include <iostream>
#include <iomanip>
#include <sstream>
#include <cctype>
#include <cmath>
#include "input.hpp"

namespace sedov
{
  IOGuard::IOGuard(std::basic_ios< char > & s):
    s_(s),
    precision_(s.precision()),
    width_(s.width()),
    flags_(s.flags()),
    fill_(s.fill())
  {}
  
  IOGuard::~IOGuard()
  {
    s_.precision(precision_);
    s_.width(width_);
    s_.flags(flags_);
    s_.fill(fill_);
  }

  std::istream & operator>>(std::istream & is, DblSci & ds)
  {
    std::istream::sentry s(is);
    if (!s)
    {
      return is;
    }
    IOGuard guard(is);
    double value;
    is >> value;
    if (is)
    {
      ds.a = value;
    }
    return is;
  }

  std::ostream & operator<<(std::ostream & os, const DblSci & ds)
  {
    IOGuard guard(os);
    std::ostringstream oss;
    oss << std::scientific << std::setprecision(1) << ds.a;
    std::string result = oss.str();
    for (size_t i = 0; i < result.length(); ++i)
    {
      result[i] = std::tolower(result[i]);
    }
    os << result;
    return os;
  }

  bool operator<(const DblSci & lhs, const DblSci & rhs)
  {
    return lhs.a < rhs.a;
  }

  bool operator==(const DblSci & lhs, const DblSci & rhs)
  {
    return std::abs(lhs.a - rhs.a) < 1e-12;
  }

  std::istream & operator>>(std::istream & is, DataStruct &)
  {
    return is;
  }
  
  std::ostream & operator<<(std::ostream & os, const DataStruct &)
  {
    return os;
  }
  
  bool operator<(const DataStruct &, const DataStruct &)
  {
    return false;
  }
}
