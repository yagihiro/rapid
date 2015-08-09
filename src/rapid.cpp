#include <cassert>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <rapid/rapid.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace rapid {

Server::Server() {}

void Server::set_port(int port) { _port = port; }

void Server::run() {
  assert(_port != kUndefinedPort);

  auto th = std::thread([&] {
    _stopped = false;

    auto sock = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_len = sizeof(addr);
    bind(sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(sock, 5);

    while (!_stop_requested) {
      struct sockaddr_in client_addr;
      auto len = sizeof(client_addr);
      auto peer =
          accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&len);

      char inbuf[4096];
      auto inlen = recv(peer, inbuf, sizeof(inbuf), 0);
      inbuf[inlen] = '\0';

      Request request(inbuf);
      auto response = _dispatcher.dispatch(request);
      auto response_s = response->message();
      write(peer, response_s->c_str(), response_s->size());

      close(peer);
    }

    close(sock);

    _stopped = true;
  });

  th.detach();
}

void Server::stop() { _stop_requested = true; }

bool Server::stopped() const { return _stopped; }

void Server::get(const std::string &pattern, const Handler &handler) {
  _dispatcher.add(Route(Method::Get, pattern, handler));
}

void Server::post(const std::string &pattern, const Handler &handler) {
  _dispatcher.add(Route(Method::Post, pattern, handler));
}
}