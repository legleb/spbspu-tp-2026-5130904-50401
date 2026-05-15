#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "input.hpp"

int main()
{
  using T = sedov::DataStruct;
  std::vector<T> data;
  while (true)
  {
    std::copy(std::istream_iterator< T >(std::cin), std::istream_iterator< T >(), std::back_inserter(data));
    if (std::cin.eof())
    {
      break;
    }
    if (std::cin.fail())
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< T >(std::cout, "\n"));
  return 0;
}
