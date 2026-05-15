#ifndef RELOAD_INPUT_HPP
#define RELOAD_INPUT_HPP
#include <iostream>
#include <string>
#include <complex>

namespace sedov
{
  struct DblSci
  {
    double a;
  };

  struct CmpLsp
  {
    std::complex< double > a;
  };

  struct DataStruct
  {
    DblSci key1;
    CmpLsp key2;
    std::string key3;
  };

  std::istream & operator>>(std::istream & is, DataStruct & ds);
  std::ostream & operator<<(std::ostream&  os, const DataStruct & ds);
  bool operator<(const DataStruct & lhs, const DataStruct & rhs);

  struct IOGuard
  {
    explicit IOGuard(std::basic_ios< char > & s);
    ~IOGuard();
  private:
    std::basic_ios< char > & s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };
}

#endif
