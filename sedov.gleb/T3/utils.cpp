#include "utils.hpp"
#include <cmath>

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

std::vector< size_t > sedov::makeIndexSequence(size_t n)
{
  std::vector< size_t > ind(n);
  std::iota(ind.begin(), ind.end(), 1);
  return ind;
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
  std::vector< size_t > ind = makeIndexSequence(poly.points.size() - 2);
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

bool sedov::operator==(const Polygon & a, const Polygon & b)
{
  if (a.points.size() != b.points.size())
  {
    return false;
  }
  return std::is_permutation(a.points.begin(), a.points.end(), b.points.begin());
}

bool sedov::pointXLess(const Point & a, const Point & b)
{
  return a.x < b.x;
}

bool sedov::pointYLess(const Point & a, const Point & b)
{
  return a.y < b.y;
}

sedov::Point sedov::getMinPoint(const Polygon & p)
{
  auto itX = std::min_element(p.points.begin(), p.points.end(), pointXLess);
  auto itY = std::min_element(p.points.begin(), p.points.end(), pointYLess);
  return {itX->x, itY->y};
}

sedov::Point sedov::shiftPoint(const Point & pt, const Point & min)
{
  return Point{pt.x - min.x, pt.y - min.y};
}

sedov::Polygon sedov::shiftPolygon(const Polygon & p, const Point & min)
{
  Polygon result;
  result.points.resize(p.points.size());
  std::transform(p.points.begin(), p.points.end(), result.points.begin(),
    std::bind(shiftPoint, std::placeholders::_1, min));
  return result;
}

bool sedov::isSamePlacement(const Polygon & candidate, const Polygon & reference)
{
  if (candidate.points.size() != reference.points.size())
  {
    return false;
  }
  Point minC = getMinPoint(candidate);
  Point minR = getMinPoint(reference);
  Polygon shiftedC = shiftPolygon(candidate, minC);
  Polygon shiftedR = shiftPolygon(reference, minR);
  return shiftedC == shiftedR;
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
