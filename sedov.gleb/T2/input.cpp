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

  std::istream & operator>>(std::istream & is, CmpLsp & cl)
  {
    std::istream::sentry s(is);
    if (!s)
    {
      return is;
    }
    IOGuard guard(is);
    char hash = 0, c = 0, open = 0, close = 0;
    double real = 0, imag = 0;
    is >> hash >> c >> open;
    if (hash != '#' || c != 'c' || open != '(')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    is >> real >> imag >> close;
    if (!is || close != ')')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    cl.a = std::complex< double >(real, imag);
    return is;
  }

  std::ostream & operator<<(std::ostream & os, const CmpLsp & cl)
  {
    IOGuard guard(os);
    os << std::fixed << std::setprecision(1);
    os << "#c(" << cl.a.real() << " " << cl.a.imag() << ")";
    return os;
  }

  bool operator<(const CmpLsp & lhs, const CmpLsp & rhs)
  {
    return std::abs(lhs.a) < std::abs(rhs.a);
  }

  bool operator==(const CmpLsp & lhs, const CmpLsp & rhs)
  {
    return std::abs(lhs.a - rhs.a) < 1e-12;
  }

  char check(std::istream & is, const std::vector< char > & expected)
  {
    char c = 0;
    is >> c;
    bool found = false;
    for (size_t i = 0; i < expected.size(); ++i)
    {
      if (expected[i] == c)
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      is.setstate(std::ios_base::failbit);
    }
    return c;
  }

  std::istream & operator>>(std::istream & is, Delimeter_t del)
  {
    del.last = check(is, del.expected);
    return is;
  }

  std::istream & getValue(std::istream & is, std::string key, std::vector< bool > & is_been, DataStruct & ds)
  {
    if (key == "key1")
    {
      if (is_been[0])
      {
        is.setstate(std::ios_base::failbit);
        return is;
      }
      is >> ds.key1;
      is_been[0] = true;
    }
    else if (key == "key2")
    {
      if (is_been[1])
      {
        is.setstate(std::ios_base::failbit);
        return is;
      }
      is >> ds.key2;
      is_been[1] = true;
    }
    else if (key == "key3")
    {
      if (is_been[2])
      {
        is.setstate(std::ios_base::failbit);
        return is;
      }
      char q = 0;
      is >> q;
      if (q != '"')
      {
        is.setstate(std::ios_base::failbit);
        return is;
      }
      std::getline(is, ds.key3, '"');
      is_been[2] = true;
    }
    else
    {
      is.setstate(std::ios_base::failbit);
    }
    return is;
  }

  std::istream & operator>>(std::istream & is, KeyValueInp inp)
  {
    return getValue(is, inp.key, inp.is_been, inp.ds);
  }

  std::istream & operator>>(std::istream & is, DataStruct & ds)
  {
    std::istream::sentry s(is);
    if (!s)
    {
      return is;
    }
    IOGuard guard(is);
    DataStruct inp;
    std::vector< bool > is_been(3, false);
    std::string k1, k2, k3;
    char open_paren = 0;
    is >> open_paren;
    if (open_paren != '(')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    char colon = 0;
    is >> colon;
    if (colon != ':')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    is >> k1;
    getValue(is, k1, is_been, inp);
    is >> colon;
    if (colon != ':')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    is >> k2;
    getValue(is, k2, is_been, inp);
    is >> colon;
    if (colon != ':')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    is >> k3;
    getValue(is, k3, is_been, inp);
    is >> colon;
    if (colon != ':')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    char close_paren = 0;
    is >> close_paren;
    if (close_paren != ')')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    if (is)
    {
      ds = inp;
    }
    return is;
  }

  std::ostream & operator<<(std::ostream & os, const DataStruct & ds)
  {
    IOGuard g(os);
    os << "(:key1 " << ds.key1 << ":key2 " << ds.key2 << ":key3 \"" << ds.key3 << "\":)";
    return os;
  }

  bool operator<(const DataStruct & lhs, const DataStruct & rhs)
  {
    if (lhs.key1.a < rhs.key1.a)
    {
      return true;
    }
    if (rhs.key1.a < lhs.key1.a)
    {
      return false;
    }
    double lhs_abs = std::abs(lhs.key2.a);
    double rhs_abs = std::abs(rhs.key2.a);
    if (lhs_abs < rhs_abs)
    {
      return true;
    }
    if (rhs_abs < lhs_abs)
    {
      return false;
    }
    return lhs.key3 < rhs.key3;
  }
}
