#include <rapid/rapid.h>

namespace rapid {

RoutingDispatcher::RoutingDispatcher() {}

void RoutingDispatcher::add(Route &&route) { _routes.emplace_back(route); }

void RoutingDispatcher::dispatch(const Request &request) {
  for (auto &one : _routes) {
    if (one.method() == request.method()) {
    }
  }
}
}