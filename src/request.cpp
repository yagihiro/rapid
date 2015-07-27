#include <algorithm>
#include <map>
#include <rapid/rapid.h>
#include <regex>
#include <sstream>
#include <vector>

namespace rapid {

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

Request::Request(const std::string &raw_input) {
  auto rows = split(raw_input);

  if (!rows.empty()) {
    using Matcher = std::map<rapid::Method, std::regex>;
    Matcher rs = {
        {rapid::Method::Get, std::regex("^GET (.+) HTTP/\\d\\.\\d$")},
        {rapid::Method::Post, std::regex("^POST (.+) HTTP/\\d\\.\\d$")},
    };
    std::smatch sm;

    std::for_each(rs.begin(), rs.end(), [&](Matcher::value_type &pair) {
      bool result = std::regex_search(rows[0], sm, pair.second);
      if (result) {
        _method = pair.first;
        _request_uri = sm[1];
        return;
      }
    });
  }
}
}