//
// Created by Ash-- on 08/01/2024.
//

#ifndef BENCHMARKSERVER_AOC_ROUTE_HPP
#define BENCHMARKSERVER_AOC_ROUTE_HPP
#include <httplib.h>
#include "bs/routes/detail.hpp"


namespace bs::routes
{
  struct aoc : public detail::Router<aoc> {
  private:
      static const detail::Route route;
      friend detail::Router<aoc>;
  };
} // bs
#endif //BENCHMARKSERVER_AOC_ROUTE_HPP
