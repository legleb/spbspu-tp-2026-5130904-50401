#include "commands.hpp"
#include <string>
#include <stdexcept>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <functional>
#include <numeric>

namespace
{
  double getArea(const sedov::Polygon & p)
  {
    return sedov::getArea(p);
  }

  bool isEven(const sedov::Polygon & p)
  {
    return sedov::hasEvenVertices(p);
  }

  bool isOdd(const sedov::Polygon & p)
  {
    return sedov::hasOddVertices(p);
  }

  bool hasVertexCount(const sedov::Polygon & p, size_t n)
  {
    return sedov::hasNVertices(p, n);
  }

  bool isDigitChar(char c)
  {
    return std::isdigit(c);
  }

  bool isNumber(const std::string & s)
  {
    return !s.empty() && std::all_of(s.begin(), s.end(), isDigitChar);
  }

  bool areaLess(const sedov::Polygon & a, const sedov::Polygon & b)
  {
    return sedov::areaLess(a, b);
  }

  bool vertexLess(const sedov::Polygon & a, const sedov::Polygon & b)
  {
    return sedov::verticesLess(a, b);
  }

  template < class Pred >
  void printFilteredSum(std::ostream & out, const std::vector< sedov::Polygon > & polygons, Pred pred)
  {
    std::vector< sedov::Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), pred);
    std::vector< double > areas;
    areas.reserve(filtered.size());
    std::transform(filtered.begin(), filtered.end(), std::back_inserter(areas), getArea);
    out << std::fixed << std::setprecision(1) << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
}

void sedov::area(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }
  out << std::fixed << std::setprecision(1);
  if (param == "MEAN")
  {
    if (polygons.empty())
    {
      throw std::invalid_argument("invalid");
    }
    std::vector< double > areas;
    areas.reserve(polygons.size());
    std::transform(polygons.begin(), polygons.end(), std::back_inserter(areas), getArea);
    double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << sum / polygons.size() << "\n";
  }
  else if (param == "EVEN")
  {
    printFilteredSum(out, polygons, isEven);
  }
  else if (param == "ODD")
  {
    printFilteredSum(out, polygons, isOdd);
  }
  else if (isNumber(param))
  {
    size_t n = std::stoul(param);
    if (n < 3)
    {
      throw std::invalid_argument("invalid");
    }
    printFilteredSum(out, polygons, std::bind(hasVertexCount, std::placeholders::_1, n));
  }
  else
  {
    throw std::invalid_argument("invalid");
  }
}

void sedov::max(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid");
  }
  if (param == "AREA")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
  }
  else if (param == "VERTEXES")
  {
    auto it = std::max_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << "\n";
  }
  else
  {
    throw std::invalid_argument("invalid");
  }
}

void sedov::min(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }
  if (polygons.empty())
  {
    throw std::invalid_argument("invalid");
  }
  if (param == "AREA")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*it) << "\n";
  }
  else if (param == "VERTEXES")
  {
    auto it = std::min_element(polygons.begin(), polygons.end(), vertexLess);
    out << it->points.size() << "\n";
  }
  else
  {
    throw std::invalid_argument("invalid");
  }
}

void sedov::count(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  std::string param;
  if (!(in >> param))
  {
    throw std::invalid_argument("invalid");
  }
  if (param == "EVEN")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEven) << "\n";
  }
  else if (param == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOdd) << "\n";
  }
  else if (isNumber(param))
  {
    size_t n = std::stoul(param);
    if (n < 3)
    {
      throw std::invalid_argument("invalid");
    }
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(hasVertexCount, std::placeholders::_1, n)) << "\n";
  }
  else
  {
    throw std::invalid_argument("invalid");
  }
}

void sedov::rects(std::istream &, std::ostream & out, std::vector< Polygon > & polygons)
{
  out << std::count_if(polygons.begin(), polygons.end(), isRect) << "\n";
}

void sedov::same(std::istream & in, std::ostream & out, std::vector< Polygon > & polygons)
{
  Polygon target;
  if (!(in >> target))
  {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (target.points.size() < 3)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto pred = std::bind(isSame, std::placeholders::_1, std::cref(target));
  out << std::count_if(polygons.begin(), polygons.end(), pred) << "\n";
}
