#include <rapid/rapid.h>
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

Request::Request(const std::string &raw_input) { auto rows = split(raw_input); }
}