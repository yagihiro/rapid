#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace rapid {

class Response {
 public:
  Response();

  static Response not_found();

  std::shared_ptr<std::string> message() const;

  void set_status_code(int code);

  void set_headers(const std::map<std::string, std::string> &headers);

  void set_body(const std::string &body);

 private:
  int _status = 404;
  std::map<std::string, std::string> _headers;
  std::string _body;
};

using ResponsePtr = std::shared_ptr<Response>;
}