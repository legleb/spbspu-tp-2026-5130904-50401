#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <cstddef>
#include <numeric>
#include <functional>

namespace sedov
{
  struct Point
  {
    int x, y;
  };

  struct Polygon
  {
    std::vector< Point > points;
  };

  struct DelimeterIO
  {
    char exp;
  };

  struct Triangle
  {
    Point a, b, c;
  };

  struct GuardIO
  {
    explicit GuardIO(std::basic_ios< char > & s);
    ~GuardIO();
  private:
    std::basic_ios< char > & s_;
    std::streamsize precision_;
    std::streamsize width_;
    std::basic_ios< char >::fmtflags flags_;
    char fill_;
  };

  Triangle makeTriangle(const std::vector< Point > & pts, size_t i);
  double triangleArea(Triangle tr);
  double getArea(const Polygon & poly);

  bool hasEvenVertices(const Polygon & p);
  bool hasOddVertices(const Polygon & p);
  bool hasNVertices(const Polygon & p, size_t n);

  bool areaLess(const Polygon & lhs, const Polygon & rhs);
  bool verticesLess(const Polygon & lhs, const Polygon & rhs);

  bool operator==(const Point & a, const Point & b);

  bool isRect(const Polygon & candidate);

  bool isSame(const Polygon & p1, const Polygon & p2);

  void readPointsRecursive(std::istream & is, std::vector< Point > & points, size_t remaining);

  std::istream & operator>>(std::istream & in, DelimeterIO && dest);
  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);
}

#endif
