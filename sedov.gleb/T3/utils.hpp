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

  std::vector< size_t > makeIndexSequence(size_t n);
  Triangle makeTriangle(const std::vector< Point > & pts, size_t i);
  double triangleArea(Triangle tr);
  double getArea(const Polygon & poly);

  bool hasEvenVertices(const Polygon & p);
  bool hasOddVertices(const Polygon & p);
  bool hasNVertices(const Polygon & p, size_t n);

  bool areaLess(const Polygon & lhs, const Polygon & rhs);
  bool verticesLess(const Polygon & lhs, const Polygon & rhs);

  bool operator==(const Point & a, const Point & b);
  bool operator==(const Polygon & a, const Polygon & b);
  bool pointXLess(const Point & a, const Point & b);
  bool pointYLess(const Point & a, const Point & b);

  Point getMinPoint(const Polygon & p);
  Point shiftPoint(const Point & pt, const Point & min);
  Polygon shiftPolygon(const Polygon & p, const Point & min);
  bool isSamePlacement(const Polygon & candidate, const Polygon & reference);

  bool isRect(const Polygon & candidate);

  void readPointsRecursive(std::istream & is, std::vector< Point > & points, size_t remaining);

  std::istream & operator>>(std::istream & in, DelimeterIO && dest);
  std::istream & operator>>(std::istream & in, Point & dest);
  std::istream & operator>>(std::istream & in, Polygon & dest);
}

#endif
