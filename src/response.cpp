#include <rapid/rapid.h>
#include <sstream>

namespace rapid {

// see: http://tools.ietf.org/html/rfc2616#section-6.1.1
static std::map<int, std::string> _reason_phrases = {
    {100, "Continue"},
    {101, "Switching Protocols"},
    {200, "OK"},
    {201, "Created"},
    {202, "Accepted"},
    {203, "Non-Authoritative Information"},
    {204, "No Content"},
    {205, "Reset Content"},
    {206, "Partial Content"},
    {300, "Multiple Choices"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {303, "See Other"},
    {304, "Not Modified"},
    {305, "Use Proxy"},
    {307, "Temporary Redirect"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {402, "Payment Required"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {405, "Method Not Allowed"},
    {406, "Not Acceptable"},
    {407, "Proxy Authentication Required"},
    {408, "Request Time-out"},
    {409, "Conflict"},
    {410, "Gone"},
    {411, "Length Required"},
    {412, "Precondition Failed"},
    {413, "Request Entity Too Large"},
    {414, "Request-URI Too Large"},
    {415, "Unsupported Media Type"},
    {416, "Requested range not satisfiable"},
    {417, "Expectation Failed"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {502, "Bad Gateway"},
    {503, "Service Unavailable"},
    {504, "Gateway Time-out"},
    {505, "HTTP Version not supported"},
};

static const char *_crlf = "\r\n";

Response::Response() {}

Response Response::not_found() {
  // TODO
  return Response();
}

std::shared_ptr<std::string> Response::message() const {
  std::stringstream stream;

  stream << "HTTP/1.1 " << _status << " " << _reason_phrases[_status] << _crlf;
  stream << "Content-Length: " << _body.size() << _crlf;

  auto it = _headers.find("Content-Type");
  if (it != _headers.end()) {
    stream << "Content-Type: " << it->second << _crlf;
  } else {
    stream << "Content-Type: application/json" << _crlf;
  }
  stream << _crlf;
  stream << _body;

  return std::make_shared<std::string>(stream.str());
}

void Response::set_status_code(int code) { _status = code; }

void Response::set_headers(const std::map<std::string, std::string> &headers) {
  _headers = headers;
}

void Response::set_body(const std::string &body) { _body = body; }
}