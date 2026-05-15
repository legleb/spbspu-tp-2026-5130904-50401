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

  std::istream & operator>>(std::istream & is, DataStruct & ds)
  {
    std::istream::sentry s(is);
    if (!s)
    {
      return is;
    }
    std::streampos pos = is.tellg();
    IOGuard guard(is);
    DataStruct inp;
    bool gotK1 = false, gotK2 = false, gotK3 = false;
    std::string key;
    char ch;
    is >> ch;
    if (ch != '(')
    {
      goto fail;
    }
    is >> ch;
    if (ch != ':')
    {
      goto fail;
    }
    is >> key;
    if (key == "key1")
    {
      while (std::isspace(is.peek()))
      {
        is.get();
      }
      if (is.peek() == '"')
      {
        goto fail;
      }
      std::string token;
      char c;
      while (is.get(c) && c != ':')
      {
        token += c;
      }
      if (!is)
      {
        goto fail;
      }
      while (!token.empty() && std::isspace(token.back()))
      {
        token.pop_back();
      }
      is.unget();
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
        goto fail;
      }
      char * endptr = nullptr;
      double value = std::strtod(token.c_str(), &endptr);
      if (endptr != token.c_str() + token.length())
      {
        goto fail;
      }
      inp.key1.a = value;
      gotK1 = true;
    }
    is >> ch;
    if (ch != ':')
    {
      goto fail;
    }
    is >> key;
    if (key == "key2")
    {
      is >> inp.key2;
      if (is)
      {
        gotK2 = true;
      }
      else 
      {
        goto fail;
      }
    }
    is >> ch;
    if (ch != ':')
    {
      goto fail;
    }
    is >> key;
    if (key == "key3")
    {
      char quote;
      is >> quote;
      if (quote == '"')
      {
        std::getline(is, inp.key3, '"');
        if (is)
        {
          gotK3 = true;
        }
        else
        {
          goto fail;
        }
      }
      else
      {
        goto fail;
      }
    }
    is >> ch;
    if (ch != ':')
    {
      goto fail;
    }
    is >> ch;
    if (ch != ')')
    {
      goto fail;
    }
    if (gotK1 && gotK2 && gotK3)
    {
      ds = inp;
      return is;
    }

  fail:
    is.clear();
    is.seekg(pos);
    is.setstate(std::ios_base::failbit);
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
