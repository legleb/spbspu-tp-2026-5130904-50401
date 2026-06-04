#include <iostream>
#include <fstream>
#include "cli.hpp"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "Too much or no file\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }
  std::vector< sedov::Polygon > allPolygons;
  sedov::readData(file, allPolygons);
  sedov::command::allPolygons = &allPolygons;
  std::vector< sedov::command > v;
  std::copy(std::istream_iterator< sedov::command >{std::cin},
    std::istream_iterator< sedov::command >{},
    std::back_inserter(v));
  return 0;
}
