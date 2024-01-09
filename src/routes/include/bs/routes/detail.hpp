//
// Created by Ash-- on 09/01/2024.
//

#ifndef BENCHMARKSERVER_DETAIL_HPP
#define BENCHMARKSERVER_DETAIL_HPP
#include <httplib.h>
#include <vector>
#include <string>
#include <tuple>

namespace bs::routes::detail
{
  using RequestHandler = std::function<void(const httplib::Request&, httplib::Response&)>;
  using RequestMethod = httplib::Server& (httplib::Server::*)(const std::string&, RequestHandler);
  using SubRoute = std::tuple<RequestMethod, std::string, RequestHandler>;
  using Route = std::vector<SubRoute>;

  template<typename DerivedRouter>
  struct Router {
      static void attachTo(httplib::Server& server) {
          for (detail::SubRoute sub_route : DerivedRouter::route) {
              auto [method, pattern, handler] = sub_route;
              (server.*method)(pattern, handler);
          }
      }
  };
}
#endif //BENCHMARKSERVER_DETAIL_HPP
