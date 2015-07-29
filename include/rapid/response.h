#pragma once

namespace rapid {

class Response {
 public:
  Response() {}

  static Response not_found() {
    // TODO
    return Response();
  }

 private:
  int _status = 404;
};
}