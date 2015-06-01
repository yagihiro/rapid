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
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 5);

    while (!_stop_requested) {
      struct sockaddr_in client_addr;
      auto len = sizeof(client_addr);
      auto peer =
          accept(sock, (struct sockaddr*)&client_addr, (socklen_t*)&len);

      std::string body = "HELLO\r\n";
      char buf[2048];
      std::memset(buf, 0, sizeof(buf));
      std::sprintf(buf,
                   "HTTP/1.0 200 OK\r\n"
                   "Content-Length: %d\r\n"
                   "Content-Type: text/html\r\n"
                   "\r\n"
                   "%s",
                   static_cast<int>(body.size()), body.c_str());
      write(peer, buf, std::strlen(buf));
      close(peer);
    }

    close(sock);

    _stopped = true;
  });

  th.detach();
}

void Server::stop() { _stop_requested = true; }

bool Server::stopped() const { return _stopped; }
}