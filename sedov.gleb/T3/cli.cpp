#include "cli.hpp"
#include <map>
#include <limits>

void sedov::readData(std::istream & in, std::vector< Polygon > & allPolygons)
{
  if (in.eof())
  {
    return;
  }
  Polygon p;
  if (in >> p) {
    allPolygons.push_back(p);
    readData(in, allPolygons);
    return;
  }
  if (in.eof())
  {
    return;
  }
  in.clear();
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  readData(in, allPolygons);
}

std::vector< sedov::Polygon > * sedov::command::allPolygons = nullptr;

std::istream & sedov::operator>>(std::istream & in, command &)
{
  std::string name;
  in >> name;
  if (!in)
  {
    return in;
  }
  static std::map< std::string, void (*)(std::istream &, std::ostream &, std::vector< Polygon > &) > commands;
  commands["AREA"] = area;
  commands["MAX"] = max;
  commands["MIN"] = min;
  commands["COUNT"] = count;
  commands["RECTS"] = rects;
  commands["SAME"] = same;
  try
  {
    commands.at(name)(in, std::cout, *command::allPolygons);
  }
  catch (...)
  {
    std::cout << "<INVALID COMMAND>\n";
    if (in.fail())
    {
      in.clear();
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return in;
}
