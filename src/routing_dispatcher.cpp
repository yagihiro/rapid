#include <rapid/rapid.h>

namespace rapid {

RoutingDispatcher::RoutingDispatcher() {}

void RoutingDispatcher::add(Route &&route) { _routes.emplace_back(route); }

Response RoutingDispatcher::dispatch(const Request &request) {
  for (auto &one : _routes) {
    if (one.method() == request.method()) {
      if (one.pattern() == request.request_uri()) {
        auto handler = one.handler();
        auto response = handler(request);
        return std::move(response);
      }
    }
  }
  return std::move(Response::not_found());
}
}