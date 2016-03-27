#include <rapid/rapid.h>
#include <algorithm>
#include <map>
#include <regex>
#include <sstream>
#include <tuple>
#include <vector>

namespace rapid {

class Request::Impl {
 public:
  using Matcher = struct {
    std::regex re;
    std::function<void(std::smatch &)> fn;
  };
  using Matchers = std::vector<Matcher>;

  Impl(Request *owner) : _owner(owner) {}
  ~Impl() { _owner = nullptr; }

  void init_matchers() {
    _matchers = {
        {std::regex("^User-Agent: (.+)$"),
         [&](std::smatch &m) { return _owner->_user_agent = m[1]; }},
        {std::regex("^GET (.+) HTTP/\\d\\.\\d$"),
         [&](std::smatch &m) {
           _owner->_method = rapid::Method::Get;
           _owner->_request_uri = m[1];
         }},
        {std::regex("^POST (.+) HTTP/\\d\\.\\d$"),
         [&](std::smatch &m) {
           _owner->_method = rapid::Method::Post;
           _owner->_request_uri = m[1];
         }},
    };
  }

  std::vector<std::string> split(const std::string &input) {
    std::istringstream stream(input);
    std::string result;
    std::vector<std::string> results;
    while (std::getline(stream, result, '\n')) {
      result.erase(result.size() - 1);
      results.emplace_back(result);
    }
    return std::move(results);
  }

  void parse(const std::string &raw_input) {
    init_matchers();
    auto rows = split(raw_input);

    if (!rows.empty()) {
      for (auto &row : rows) {
        for (auto &matcher : _matchers) {
          std::smatch match;
          if (std::regex_match(row, match, matcher.re)) {
            matcher.fn(match);
          }
        }
      }
    }
  }

 private:
  Request *_owner = nullptr;
  Matchers _matchers;
};

Request::Request(const std::string &raw_input) : _impl(new Impl(this)) {
  _impl->parse(raw_input);
}
Request::~Request() {}
}