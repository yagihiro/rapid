#pragma once
#include <memory>
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

  std::shared_ptr<Response> dispatch(const Request &request);

 private:
  std::vector<Route> _routes;
};
}