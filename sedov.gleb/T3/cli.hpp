#ifndef CLI_HPP
#define CLI_HPP
#include "commands.hpp"

namespace sedov
{
  struct command
  {
    static std::vector< Polygon > * allPolygons;
    command() = default;
  };

  std::istream & operator>>(std::istream & in, command &);
  void readData(std::istream & in, std::vector< Polygon > & allPolygons);
}

#endif
