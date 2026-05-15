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

  std::istream & operator>>(std::istream & is, DblSci & ds);
  std::ostream & operator<<(std::ostream & os, const DblSci & ds);
  bool operator<(const DblSci & lhs, const DblSci & rhs);
  bool operator==(const DblSci & lhs, const DblSci & rhs);

  struct CmpLsp
  {
    std::complex< double > a;
  };

  std::istream & operator>>(std::istream & is, CmpLsp & cl);
  std::ostream & operator<<(std::ostream & os, const CmpLsp & cl);
  bool operator<(const CmpLsp & lhs, const CmpLsp & rhs);
  bool operator==(const CmpLsp & lhs, const CmpLsp & rhs);

  struct DataStruct
  {
    DblSci key1;
    CmpLsp key2;
    std::string key3;
  };

  std::istream & operator>>(std::istream & is, DataStruct & ds);
  std::ostream & operator<<(std::ostream & os, const DataStruct & ds);
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
