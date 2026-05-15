#include <iostream>
#include <vector>
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

  struct Delimeter_t
  {
    std::vector< char > expected;
    char & last;
  };

  std::istream & operator>>(std::istream & is, Delimeter_t del);

  struct KeyValueInp
  {
    std::string key;
    std::vector< bool > & is_been;
    DataStruct & ds;
  };

  std::istream & operator>>(std::istream & is, KeyValueInp inp);

  struct IOGuard
  {
    explicit IOGuard(std::basic_ios< char > & s):
      s_(s),
      precision_(s.precision()),
      width_(s.width()),
      flags_(s.flags()),
      fill_(s.fill())
    {}
    ~IOGuard()
    {
      s_.precision(precision_);
      s_.width(width_);
      s_.flags(flags_);
      s_.fill(fill_);
    }
  private:
    std::basic_ios< char > & s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  char check(std::istream & is, const std::vector< char > & expected);
  std::istream & getValue(std::istream & is, std::string key, std::vector< bool > & is_been, DataStruct & ds);
}
