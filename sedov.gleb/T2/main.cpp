#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>
#include "input.hpp"

int main()
{
  using T = sedov::DataStruct;
  std::vector< T > data;
  using iit_t = std::istream_iterator< T >;
  while (!std::cin.eof())
  {
    std::copy(iit_t{std::cin}, iit_t{}, std::back_inserter(data));
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  using oit_t = std::ostream_iterator< T >;
  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
  return 0;
}
