#include "commands.hpp"
#include <string>
#include <stdexcept>
#include <iomanip>
#include <limits>

void sedov::same(std::istream & in, std::ostream & out, std::vector< Polygon > & poly)
{
  Polygon ref;
  std::streampos pos = in.tellg();
  if (!(in >> ref))
  {
    in.clear();
    in.seekg(pos);
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto pred = std::bind(isSamePlacement, std::placeholders::_1, std::cref(ref));
  size_t result = std::count_if(poly.begin(), poly.end(), pred);
  out << result << '\n';
}

void sedov::rects(std::istream &, std::ostream & out, std::vector< Polygon > & poly)
{
  size_t result = std::count_if(poly.begin(), poly.end(), isRect);
  out << result << '\n';
}

void sedov::area(std::istream & in, std::ostream & out, std::vector< Polygon > & poly)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::runtime_error("Missing str");
  }
  std::vector< Polygon > filtered;
  if (str == "MEAN")
  {
    if (poly.empty())
    {
      throw std::runtime_error("No polygons for mean");
    }
    std::vector< double > areas(poly.size());
    std::transform(poly.begin(), poly.end(), areas.begin(), getArea);
    double total = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << std::fixed << std::setprecision(1) << total / poly.size() << '\n';
    return;
  }
  else if (str == "EVEN")
  {
    std::copy_if(poly.begin(), poly.end(), std::back_inserter(filtered), hasEvenVertices);
  }
  else if (str == "ODD")
  {
    std::copy_if(poly.begin(), poly.end(), std::back_inserter(filtered), hasOddVertices);
  }
  else
  {
    size_t n = std::stoul(str);
    if (n < 3)
    {
      throw std::runtime_error("Invalid vertex count");
    }
    auto pred = std::bind(hasNVertices, std::placeholders::_1, n);
    std::copy_if(poly.begin(), poly.end(), std::back_inserter(filtered), pred);
  }
  std::vector< double > areas(filtered.size());
  std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
  double total = std::accumulate(areas.begin(), areas.end(), 0.0);
  out << std::fixed << std::setprecision(1) << total << '\n';
}

void sedov::count(std::istream & in, std::ostream & out, std::vector< Polygon > & poly)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::runtime_error("Missing str");
  }
  size_t result = 0;
  if (str == "EVEN")
  {
    result = std::count_if(poly.begin(), poly.end(), hasEvenVertices);
  }
  else if (str == "ODD")
  {
    result = std::count_if(poly.begin(), poly.end(), hasOddVertices);
  }
  else
  {
    size_t n = std::stoul(str);
    if (n < 3)
    {
      throw std::runtime_error("Invalid vertex count");
    }
    auto pred = std::bind(hasNVertices, std::placeholders::_1, n);
    result = std::count_if(poly.begin(), poly.end(), pred);
  }
  out << result << '\n';
}

void sedov::max(std::istream & in, std::ostream & out, std::vector< Polygon > & poly)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::runtime_error("Missing str");
  }
  if (poly.empty())
  {
    throw std::runtime_error("No polygons for MAX");
  }
  if (str == "AREA")
  {
    auto it = std::max_element(poly.begin(), poly.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*it) << '\n';
  }
  else if (str == "VERTEXES")
  {
    auto it = std::max_element(poly.begin(), poly.end(), verticesLess);
    out << it->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("No command available");
  }
}

void sedov::min(std::istream & in, std::ostream & out, std::vector< Polygon > & poly)
{
  std::string str;
  if (!(in >> str))
  {
    throw std::runtime_error("Missing str");
  }
  if (poly.empty())
  {
    throw std::runtime_error("No polygons for MIN");
  }
  if (str == "AREA")
  {
    auto it = std::min_element(poly.begin(), poly.end(), areaLess);
    out << std::fixed << std::setprecision(1) << getArea(*it) << '\n';
  }
  else if (str == "VERTEXES")
  {
    auto it = std::min_element(poly.begin(), poly.end(), verticesLess);
    out << it->points.size() << '\n';
  }
  else
  {
    throw std::invalid_argument("No command available");
  }
}
