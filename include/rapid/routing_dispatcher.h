#pragma once
#include <string>
#include <vector>
#include "request.h"
#include "response.h"
#include "route.h"

namespace rapid {

class RoutingDispatcher {
 public:
  RoutingDispatcher();

  void add(Route &&route);

  Response dispatch(const Request &request);

 private:
  std::vector<Route> _routes;
};
}