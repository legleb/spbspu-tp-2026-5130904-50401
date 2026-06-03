#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "utils.hpp"

namespace sedov
{
  void area(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void count(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void max(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void min(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void rects(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void same(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void echo(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void inframe(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void intersections(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void maxseq(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void perms(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void rmecho(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
  void lessarea(std::istream & in, std::ostream & out, std::vector< Polygon > & poly);
}

#endif
