#include "utils.hpp"
#include <cmath>
#include <cctype>
#include <limits>

sedov::GuardIO::GuardIO(std::basic_ios< char > & s):
  s_(s),
  precision_(s.precision()),
  width_(s.width()),
  flags_(s.flags()),
  fill_(s.fill())
{}

sedov::GuardIO::~GuardIO()
{
  s_.precision(precision_);
  s_.width(width_);
  s_.flags(flags_);
  s_.fill(fill_);
}

sedov::Triangle sedov::makeTriangle(const std::vector< Point > & pts, size_t i)
{
  return Triangle{pts[0], pts[i], pts[i + 1]};
}

double sedov::triangleArea(Triangle tr)
{
  double v1 = static_cast< double >(tr.a.x) * (tr.b.y - tr.c.y);
  double v2 = static_cast< double >(tr.b.x) * (tr.c.y - tr.a.y);
  double v3 = static_cast< double >(tr.c.x) * (tr.a.y - tr.b.y);
  return 0.5 * std::fabs(v1 + v2 + v3);
}

double sedov::getArea(const Polygon & poly)
{
  if (poly.points.size() < 3)
  {
    throw std::invalid_argument("poly size must be >= 3");
  }
  std::vector< size_t > ind(poly.points.size() - 2);
  std::iota(ind.begin(), ind.end(), 1);
  std::vector< Triangle > tr(ind.size());
  std::transform(ind.begin(), ind.end(), tr.begin(),
    std::bind(makeTriangle, std::cref(poly.points), std::placeholders::_1));
  std::vector< double > areas(tr.size());
  std::transform(tr.begin(), tr.end(), areas.begin(), triangleArea);
  return std::accumulate(areas.begin(), areas.end(), 0.0);
}

bool sedov::hasEvenVertices(const Polygon & p)
{
  return p.points.size() % 2 == 0;
}

bool sedov::hasOddVertices(const Polygon & p)
{
  return p.points.size() % 2 != 0;
}

bool sedov::hasNVertices(const Polygon & p, size_t n)
{
  return p.points.size() == n;
}

bool sedov::areaLess(const Polygon & lhs, const Polygon & rhs)
{
  return getArea(lhs) < getArea(rhs);
}

bool sedov::verticesLess(const Polygon & lhs, const Polygon & rhs)
{
  return lhs.points.size() < rhs.points.size();
}

bool sedov::operator==(const Point & a, const Point & b)
{
  return a.x == b.x && a.y == b.y;
}

bool sedov::isRect(const Polygon & p)
{
  if (p.points.size() != 4)
  {
    return false;
  }
  const auto & pts = p.points;
  long long x01 = pts[1].x - pts[0].x;
  long long y01 = pts[1].y - pts[0].y;
  long long x12 = pts[2].x - pts[1].x;
  long long y12 = pts[2].y - pts[1].y;
  long long x23 = pts[3].x - pts[2].x;
  long long y23 = pts[3].y - pts[2].y;
  long long x30 = pts[0].x - pts[3].x;
  long long y30 = pts[0].y - pts[3].y;
  return (x01 * x12 + y01 * y12 == 0) &&
    (x12 * x23 + y12 * y23 == 0) &&
    (x23 * x30 + y23 * y30 == 0) &&
    (x30 * x01 + y30 * y01 == 0);
}

namespace
{
  sedov::Point translatePoint(sedov::Point origin, sedov::Point p)
  {
    return sedov::Point{p.x - origin.x, p.y - origin.y};
  }

  bool checkShift(const std::vector< sedov::Point > & n1, const std::vector< sedov::Point > & n2, size_t shift,
    size_t n)
  {
    std::vector< sedov::Point > rotated(n);
    std::rotate_copy(n2.begin(), n2.begin() + shift, n2.end(), rotated.begin());
    return std::equal(n1.begin(), n1.end(), rotated.begin());
  }
}

bool sedov::isSame(const Polygon & p1, const Polygon & p2)
{
  if (p1.points.size() != p2.points.size())
  {
    return false;
  }
  if (p1.points.empty())
  {
    return true;
  }
  size_t n = p1.points.size();
  std::vector< Point > n1(n), n2(n);
  const Point & orig1 = p1.points[0];
  const Point & orig2 = p2.points[0];
  std::transform(p1.points.begin(), p1.points.end(), n1.begin(),
    std::bind(translatePoint, orig1, std::placeholders::_1));
  std::transform(p2.points.begin(), p2.points.end(), n2.begin(),
    std::bind(translatePoint, orig2, std::placeholders::_1));
  std::vector< size_t > shifts(n);
  std::iota(shifts.begin(), shifts.end(), 0);
  auto pred = std::bind(checkShift, std::cref(n1), std::cref(n2), std::placeholders::_1, n);
  return std::any_of(shifts.begin(), shifts.end(), pred);
}

void sedov::readPointsRecursive(std::istream & in, std::vector< Point > & points, size_t remaining)
{
  if (remaining == 0)
  {
    return;
  }
  Point pt;
  if (!(in >> pt))
  {
    in.setstate(std::ios_base::failbit);
    return;
  }
  points.push_back(pt);
  readPointsRecursive(in, points, remaining - 1);
}

std::istream & sedov::operator>>(std::istream & in, DelimeterIO && dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '\0';
  in >> c;
  if (in && dest.exp != c)
  {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::istream & sedov::operator>>(std::istream & in, Point & dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimeterIO{'('} >> dest.x >> DelimeterIO{';'} >> dest.y >> DelimeterIO{')'};
  return in;
}

std::istream & sedov::operator>>(std::istream & in, Polygon & p)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  GuardIO guard(in);
  Polygon poly;
  size_t cnt = 0;
  in >> cnt;
  if (!in || cnt < 3)
  {
    in.setstate(std::ios_base::failbit);
    return in;
  }
  poly.points.reserve(cnt);
  readPointsRecursive(in, poly.points, cnt);
  if (!in || poly.points.size() != cnt)
  {
    in.setstate(std::ios_base::failbit);
    return in;
  }
  p = std::move(poly);
  return in;
}
