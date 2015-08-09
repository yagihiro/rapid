#pragma once
#include <string>
#include "method.h"

namespace rapid {

class Request {
 public:
  Request(const std::string &raw_input);

  Method method() const { return _method; }
  std::string request_uri() const { return _request_uri; }

 private:
  Method _method = Method::Get;
  std::string _request_uri;
};
}