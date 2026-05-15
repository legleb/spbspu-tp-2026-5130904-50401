#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include "input.hpp"

int main()
{
  using T = sedov::DataStruct;
  std::vector< T > data;
  T temp;
  while (!std::cin.eof())
  {
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
  for (size_t i = 0; i < data.size(); ++i)
  {
    std::cout << data[i] << "\n";
  }
  return 0;
}
