#include <assert>
#include <rapid/rapid.h>
#include <sys/socket.h>
#include <future>

namespace rapid {

Server::Server() {}

void Server::set_port(int port) { _port = port; }

void Server::run() {
  std::assert(_port != kUndefinedPort);

  std::async(std::future::async, [&] {
    _stopped = false;

    auto sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_len = sizeof(addr);
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 5);

    while (!_stop_requested) {
      struct sockaddr_in client_addr;
      len = sizeof(client_addr);
      auto peer = accept(sock0, (struct sockaddr*)&client_addr, &len);
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