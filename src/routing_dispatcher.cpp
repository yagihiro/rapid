#include <rapid/rapid.h>

namespace rapid {

RoutingDispatcher::RoutingDispatcher() {}

void RoutingDispatcher::add(Route &&route) { _routes.emplace_back(route); }

std::shared_ptr<Response> RoutingDispatcher::dispatch(const Request &request) {
  for (auto &one : _routes) {
    if (one.method() == request.method()) {
      if (one.pattern() == request.request_uri()) {
        auto handler = one.handler();
        auto response = handler(request);
        return response;
      }
    }
  }
  return std::make_shared<Response>(Response::not_found());
}
}