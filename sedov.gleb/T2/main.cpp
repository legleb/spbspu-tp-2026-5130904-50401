#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <limits>
#include "input.hpp"

int main()
{
  using T = sedov::DataStruct;
  std::vector< T > data;
  while (!std::cin.eof())
  {
    T temp;
    if (std::cin >> temp)
    {
      data.push_back(temp);
    }
    else
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  if (data.empty())
  {
    return 0;
  }
  std::sort(data.begin(), data.end());
  using oit_t = std::ostream_iterator< T >;
  std::copy(data.begin(), data.end(), oit_t{std::cout, "\n"});
  return 0;
}
