#pragma once
#include <string>
#include "method.h"

namespace rapid {

class Request {
 public:
  Request(const std::string &raw_input);

  Method method() const { return _method; }

 private:
  Method _method = Method::Get;
  std::string _request_uri;
};
}