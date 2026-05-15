#include <iostream>
#include <vector>
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
  std::sort(data.begin(), data.end());
  std::copy(data.begin(), data.end(), std::ostream_iterator< T >(std::cout, "\n"));
  return 0;
}
