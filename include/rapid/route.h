#pragma once
#include <string>
#include "handler.h"
#include "method.h"

namespace rapid {

class Route {
 public:
  Route(Method method, const std::string &pattern, const Handler &handler);

  Method method() const { return _method; }

  std::string pattern() const { return _pattern; }

  Handler handler() const { return _handler; }

 private:
  Method _method;
  std::string _pattern;
  Handler _handler;
};
}