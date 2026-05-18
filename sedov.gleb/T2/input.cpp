#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <complex>
#include <cmath>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <cerrno>
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
    char hash = 0, c = 0, open = 0, close = 0;
    double real = 0, imag = 0;
    is >> hash >> c >> open;
    if (hash != '#' || c != 'c' || open != '(')
    {
      is.setstate(std::ios_base::failbit);
      return is;
    }
    is >> real >> imag >> close;
    if (close != ')')
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

  std::istream & operator>>(std::istream & is, Delimeter && dest)
  {
    char c = 0;
    is >> c;
    if (c != dest.expected)
    {
      is.setstate(std::ios_base::failbit);
    }
    return is;
  }

  static bool readKey1(std::istream & is, DataStruct & inp)
  {
    while (std::isspace(is.peek()))
    {
      is.get();
    }
    if (is.peek() == '"')
    {
      return false;
    }
    std::string token;
    char c;
    while (is.get(c) && c != ':')
    {
      token += c;
    }
    if (!is)
    {
      return false;
    }
    while (!token.empty() && std::isspace(token.back()))
    {
      token.pop_back();
    }
    is.unget();
    bool hasExp = false;
    for (size_t i = 0; i < token.length(); ++i)
    {
      if (token[i] == 'e' || token[i] == 'E')
      {
        hasExp = true;
        break;
      }
    }
    if (!hasExp)
    {
      return false;
    }
    char * endptr = nullptr;
    double value = std::strtod(token.c_str(), &endptr);
    if (endptr != token.c_str() + token.length())
    {
      return false;
    }
    inp.key1.a = value;
    return true;
  }

  static bool readKey2(std::istream & is, DataStruct & inp)
  {
    is >> inp.key2;
    return is.good();
  }

  static bool readKey3(std::istream & is, DataStruct & inp)
  {
    char quote;
    is >> quote;
    if (quote != '"')
    {
      return false;
    }
    std::getline(is, inp.key3, '"');
    return is.good();
  }

  static void skipUnknownKey(std::istream & is)
  {
    char ch;
    int depth = 0;
    bool inQuotes = false;
    while (is.get(ch) && (depth > 0 || inQuotes || ch != ':'))
    {
      if (ch == '"')
      {
        inQuotes = !inQuotes;
      }
      if (!inQuotes && ch == '(')
      {
        depth++;
      }
      if (!inQuotes && ch == ')')
      {
        depth--;
      }
    }
    is.unget();
  }

  std::istream & operator>>(std::istream & is, KeyValueInp && inp)
  {
    if (inp.key == "key1")
    {
      if (inp.is_been[0])
      {
        return is;
      }
      if (readKey1(is, inp.ds))
      {
        inp.is_been[0] = true;
      }
      else
      {
        is.setstate(std::ios_base::failbit);
      }
    }
    else if (inp.key == "key2")
    {
      if (inp.is_been[1])
      {
        return is;
      }
      if (readKey2(is, inp.ds))
      {
        inp.is_been[1] = true;
      }
      else
      {
        is.setstate(std::ios_base::failbit);
      }
    }
    else if (inp.key == "key3")
    {
      if (inp.is_been[2])
      {
        return is;
      }
      if (readKey3(is, inp.ds))
      {
        inp.is_been[2] = true;
      }
      else
      {
        is.setstate(std::ios_base::failbit);
      }
    }
    else
    {
      skipUnknownKey(is);
    }
    return is;
  }

  std::istream & operator>>(std::istream & is, DataStruct & ds)
  {
    std::streampos pos = is.tellg();
    is >> Delimeter{'('} >> Delimeter{':'};
    if (!is)
    {
      return is;
    }
    DataStruct inp;
    std::vector< bool > is_been(3, false);
    while (is && is.peek() != ')')
    {
      std::string key;
      is >> key;
      if (!is)
      {
        return is;
      }
      is >> KeyValueInp{key, is_been, inp};
      if (!is)
      {
        return is;
      }
      if (is.peek() == ':')
      {
        is >> Delimeter{':'};
        if (!is)
        {
          return is;
        }
      }
    }
    is >> Delimeter{')'};
    if (!is)
    {
      return is;
    }
    if (is_been[0] && is_been[1] && is_been[2])
    {
      ds = inp;
    }
    else
    {
      is.clear();
      is.seekg(pos);
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
