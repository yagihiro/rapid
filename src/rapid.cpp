#include <cassert>
#include <future>
#include <netinet/in.h>
#include <rapid/rapid.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace rapid {

Server::Server() {}

void Server::set_port(int port) { _port = port; }

void Server::run() {
  assert(_port != kUndefinedPort);

  std::async(std::launch::async, [&] {
    _stopped = false;

    auto sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_len = sizeof(addr);
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 5);

    while (!_stop_requested) {
      struct sockaddr_in client_addr;
      auto len = sizeof(client_addr);
      auto peer = accept(sock, (struct sockaddr *)&client_addr, (socklen_t *)&len);
      write(peer, "HELLO", 5);
      close(peer);
    }

    close(sock);

    _stopped = true;
  });
}

void Server::stop() { _stop_requested = true; }

bool Server::stopped() const { return _stopped; }
}