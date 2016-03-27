#include <arpa/inet.h>
#include <netinet/in.h>
#include <rapid/rapid.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <string>
#include <thread>

#include <openssl/err.h>
#include <openssl/ssl.h>

namespace rapid {

class TlsManager {
 public:
  static TlsManager *instance() {
    static TlsManager *_instance = nullptr;
    if (_instance) {
      return _instance;
    }
    _instance = new TlsManager;
    return _instance;
  }

  bool failed() const { return _failed; }
  bool succeed() const { return !_failed; }

  void init() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();
    auto method = TLSv1_server_method();
    auto ctx = SSL_CTX_new(method);
    if (!ctx) {
      _failed = true;
      ERR_print_errors_fp(stderr);
      return;
    }
    _ctx = ctx;
  }

  bool load_certificate(const std::string &cert_path) {
    if (_ctx == nullptr) {
      return false;
    }
    if (SSL_CTX_use_certificate_file(_ctx, cert_path.c_str(),
                                     SSL_FILETYPE_PEM) <= 0) {
      return false;
    }
    return true;
  }

  bool load_private_key(const std::string &private_key_path) {
    if (_ctx == nullptr) {
      return false;
    }
    if (SSL_CTX_use_PrivateKey_file(_ctx, private_key_path.c_str(),
                                    SSL_FILETYPE_PEM) <= 0) {
      return false;
    }
    return true;
  }

  bool verify_private_key() {
    if (_ctx == nullptr) {
      return false;
    }
    if (!SSL_CTX_check_private_key(_ctx)) {
      return false;
    }
    return true;
  }

  bool recv_tls(int peer, const std::string &addr, uint16_t port,
                const std::function<ResponsePtr(Request &)> &fn) {
    if (_ctx == nullptr) {
      return false;
    }

    SSL *ssl = SSL_new(_ctx);
    SSL_set_fd(ssl, peer);
    if (SSL_accept(ssl) == -1) {
      // ...
    } else {
      char buf[4096];
      auto bytes = SSL_read(ssl, buf, sizeof(buf));
      if (0 < bytes) {
        buf[bytes] = 0;
        Request request(buf, addr, port);
        auto response = fn(request);
        auto response_s = response->message();
        SSL_write(ssl, response_s->c_str(),
                  static_cast<int>(response_s->size()));
      }
    }
    SSL_free(ssl);

    return true;
  }

 private:
  bool _failed = false;
  SSL_CTX *_ctx = nullptr;
};

Server::Server() {}

void Server::set_port(int port) { _port = port; }

void Server::use_tls() { _use_tls = true; }

void Server::set_tls_certificate_path(const std::string &path) {
  _cert_path = path;
}

void Server::set_tls_private_key_path(const std::string &path) {
  _pk_path = path;
}

void Server::run() {
  assert(_port != kUndefinedPort);

  auto th = std::thread([&] {
    _stopped = false;

    boot();

    while (!_stop_requested) {
      dispatch_http_request(_sock);
    }

    close(_sock);
    _sock = -1;

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

void Server::boot() {
  if (_sock == -1) {
    if (_use_tls) {
      TlsManager::instance()->init();
      TlsManager::instance()->load_certificate(_cert_path);
      TlsManager::instance()->load_private_key(_pk_path);
      TlsManager::instance()->verify_private_key();
    }

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 1;
    setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(_sock, (struct sockaddr *)&addr, sizeof(addr));
    listen(_sock, 5);
  }
}

void Server::dispatch_http_request(int socket) {
  struct sockaddr_in client_addr;
  auto len = sizeof(client_addr);
  auto peer =
      accept(socket, (struct sockaddr *)&client_addr, (socklen_t *)&len);
  char s[INET6_ADDRSTRLEN];
  auto caddr =
      inet_ntop(client_addr.sin_family, &client_addr.sin_addr, s, sizeof(s));
  auto cport = ntohs(client_addr.sin_port);

  if (_use_tls) {
    TlsManager::instance()->recv_tls(peer, caddr, cport,
                                     [&](Request &request) -> ResponsePtr {
                                       return _dispatcher.dispatch(request);
                                     });
  } else {
    char inbuf[4096];
    auto inlen = recv(peer, inbuf, sizeof(inbuf), 0);
    inbuf[inlen] = '\0';

    Request request(inbuf, caddr, cport);
    auto response = _dispatcher.dispatch(request);
    auto response_s = response->message();
    write(peer, response_s->c_str(), response_s->size());
  }
  close(peer);
}
}
