#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <complex>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include "input.hpp"

namespace sedov
{
  std::istream & operator>>(std::istream & is, DblSci & ds)
  {
    std::istream::sentry s(is);
    if (!s)
    {
      return is;
    }
    IOGuard guard(is);
    std::string token;
    is >> token;
    if (!is)
    {
      return is;
    }
    bool hasExponent = false;
    for (size_t i = 0; i < token.length(); ++i)
    {
      if (token[i] == 'e' || token[i] == 'E')
      {
        hasExponent = true;
        break;
      }
    }
    if (!hasExponent)
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    char * endptr = nullptr;
    double value = std::strtod(token.c_str(), &endptr);
    if (endptr != token.c_str() + token.length())
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    ds.a = value;
    return is;
  }

  std::ostream & operator<<(std::ostream & os, const DblSci & ds)
  {
    IOGuard guard(os);
    double val = ds.a;
    if (std::abs(val) < 1e-12)
    {
      os << "0.0e+0";
      return os;
    }
    if (val < 0)
    {
      os << "-";
      val = -val;
    }
    int exp = 0;
    while (val >= 10.0)
    {
      val /= 10.0;
      exp++;
    }
    while (val < 1.0 && val > 0)
    {
      val *= 10.0;
      exp--;
    }
    double mant = std::round(val * 10.0) / 10.0;
    if (std::abs(mant - 10.0) < 1e-12)
    {
      mant = 1.0;
      exp++;
    }
    os << std::fixed << std::setprecision(1) << mant;
    os << "e";
    if (exp >= 0)
    {
      os << "+";
    }
    os << exp;
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
    is >> real >> imag;
    is >> close;
    if (close != ')')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    int next = is.peek();
    if (next != EOF && next != ':' && next != ')')
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
    using d_t = Delimeter_t;
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
      std::getline(is >> d_t{{'"'}, q}, ds.key3, '"');
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
    char last = 0;
    using d_t = Delimeter_t;
    std::vector< bool > is_been(3, false);
    std::string k1, k2, k3;
    is >> d_t{{'('}, last} >> d_t{{':'}, last}
      >> k1 >> KeyValueInp{k1, is_been, inp} >> d_t{{':'}, last}
      >> k2 >> KeyValueInp{k2, is_been, inp} >> d_t{{':'}, last}
      >> k3 >> KeyValueInp{k3, is_been, inp} >> d_t{{':'}, last}
      >> d_t{{')'}, last};
    if (is && is_been[0] && is_been[1] && is_been[2])
    {
      ds = inp;
    }
    else
    {
      is.setstate(std::ios_base::failbit);
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
    if (lhs.key1 < rhs.key1)
    {
      return true;
    }
    if (rhs.key1 < lhs.key1)
    {
      return false;
    }
    if (lhs.key2 < rhs.key2)
    {
      return true;
    }
    if (rhs.key2 < lhs.key2)
    {
      return false;
    }
    return lhs.key3 < rhs.key3;
  }
}
