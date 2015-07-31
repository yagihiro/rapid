#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace rapid {

class Response {
 public:
  Response() {}

  static Response not_found() {
    // TODO
    return Response();
  }

  std::shared_ptr<std::string> message() const {
    return std::make_shared<std::string>();
  }

  void set_status_code(int code) { _status = code; }

  void set_headers(const std::map<std::string, std::string> &headers) {
    _headers = headers;
  }

  void set_body(const std::vector<std::string> &body) { _body = body; }

 private:
  int _status = 404;
  std::map<std::string, std::string> _headers;
  std::vector<std::string> _body;
};

using ResponsePtr = std::shared_ptr<Response>;
}