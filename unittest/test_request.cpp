#include <rapid/rapid.h>
#include "catch.hpp"

TEST_CASE("Testing rapid::Request Class") {
  std::string _chrome_get_request(
      "GET /path/to?foo=1&bar=2 HTTP/1.1\r\nHost: "
      "localhost:8080\r\nConnection: keep-alive\r\nAccept: "
      "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/"
      "*;q=0.8\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 "
      "(Macintosh; Intel Mac OS X 10_11_4) AppleWebKit/537.36 (KHTML, like "
      "Gecko) Chrome/49.0.2623.87 Safari/537.36\r\nAccept-Encoding: gzip, "
      "deflate, sdch\r\nAccept-Language: ja,en-US;q=0.8,en;q=0.6\r\n\r\n");

  std::string _safari_get_request(
      "GET /path/to?foo=1&bar=2 HTTP/1.1\r\nHost: localhost:8080\r\nAccept: "
      "text/html,application/xhtml+xml,application/xml;q=0.9,*/"
      "*;q=0.8\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) "
      "AppleWebKit/601.5.17 (KHTML, like Gecko) Version/9.1 "
      "Safari/601.5.17\r\nAccept-Language: ja-jp\r\nAccept-Encoding: gzip, "
      "deflate\r\nConnection: keep-alive\r\n\r\n");

  SECTION("Chrome GET request, right request") {
    rapid::Request request(_chrome_get_request);

    REQUIRE(request.method() == rapid::Method::Get);
    REQUIRE(request.request_uri() == "/path/to?foo=1&bar=2");
    REQUIRE(request.user_agent() ==
            "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) "
            "AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.87 "
            "Safari/537.36");
  }

  SECTION("Safari GET request, right request") {
    rapid::Request request(_safari_get_request);

    REQUIRE(request.method() == rapid::Method::Get);
    REQUIRE(request.request_uri() == "/path/to?foo=1&bar=2");
    REQUIRE(request.user_agent() ==
            "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) "
            "AppleWebKit/601.5.17 (KHTML, like Gecko) Version/9.1 "
            "Safari/601.5.17");
  }
}