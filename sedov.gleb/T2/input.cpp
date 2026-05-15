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
