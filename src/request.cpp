#include <rapid/rapid.h>
#include <algorithm>
#include <cctype>
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
    auto set = [&](std::smatch &m, const std::string &method) {
      _environments["REQUEST_METHOD"] = method;
      _environments["REQUEST_URI"] = m[1];
      _environments["SERVER_PROTOCOL"] = m[2];
    };

    _matchers = {
        {std::regex("^([a-zA-Z0-9\\-]+): (.+)$"),
         [&](std::smatch &m) {
           std::string key = m[1];
           std::transform(key.begin(), key.end(), key.begin(),
                          [&](std::string::value_type c1) {
                            auto c2 = ::toupper(c1);
                            if (c2 == '-') {
                              c2 = '_';
                            }
                            return c2;
                          });
           _environments[key] = m[2];
         }},
        {std::regex("^GET (.+) (HTTP/\\d\\.\\d)$"),
         [&](std::smatch &m) {
           _owner->_method = rapid::Method::Get;
           _owner->_request_uri = m[1];
           set(m, "GET");
         }},
        {std::regex("^POST (.+) (HTTP/\\d\\.\\d)$"),
         [&](std::smatch &m) {
           _owner->_method = rapid::Method::Post;
           _owner->_request_uri = m[1];
           set(m, "POST");
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

  const Request::Environments &environments() const { return _environments; }

  void add_additional_environments(const Peer &peer) {
    _environments["GATEWAY_INTERFACE"] = "CGI/1.1";
    _environments["SERVER_ADDR"] = peer.server_addr;
    _environments["SERVER_NAME"] = peer.server_name;
    _environments["SERVER_SOFTWARE"] =
        std::string(RAPID_SOFTWARE_NAME "/" RAPID_VERSION);
    _environments["SERVER_PORT"] = peer.server_port;

    char buf[16];
    sprintf(buf, "%d", ::time(nullptr));
    _environments["REQUEST_TIME"] = buf;

    _environments["REMOTE_ADDR"] = peer.client_addr;
    _environments["REMOTE_PORT"] = peer.client_port;
  }

 private:
  Request *_owner = nullptr;
  Matchers _matchers;
  Request::Environments _environments;
};

Request::Request(const std::string &raw_input) : Request(raw_input, Peer()) {}

Request::Request(const std::string &raw_input, const Peer &peer)
    : _impl(new Impl(this)), _peer(peer) {
  _impl->parse(raw_input);
  _impl->add_additional_environments(_peer);
}

Request::~Request() {}

const Request::Environments &Request::environments() const {
  return _impl->environments();
}

std::string Request::user_agent() const {
  try {
    return environments().at("USER_AGENT");
  } catch (std::out_of_range &e) {
    return "";
  }
}
}