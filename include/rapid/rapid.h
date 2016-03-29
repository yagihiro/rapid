#pragma once
#include "handler.h"
#include "method.h"
#include "peer.h"
#include "request.h"
#include "response.h"
#include "route.h"
#include "routing_dispatcher.h"
#include "version.h"

namespace rapid {

class Server {
 public:
  Server();
  ~Server() = default;

  void set_port(uint16_t port);
  void use_tls();
  void set_tls_certificate_path(const std::string &path);
  void set_tls_private_key_path(const std::string &path);
  void run();
  void stop();

  bool stopped() const;

  /**
   * \defgroup Routing functions
   * @{
   */

  /**
   * Register a get handler
   */
  void get(const std::string &pattern, const Handler &handler);
  /**
   * Register a post handler
   */
  void post(const std::string &pattern, const Handler &handler);

  /** @} */

 private:
  static constexpr uint16_t kUndefinedPort = 0;

  Peer _peer;
  bool _use_tls = false;
  std::string _cert_path;
  std::string _pk_path;
  bool _stop_requested = false;
  bool _stopped = true;
  int _sock = -1;

  RoutingDispatcher _dispatcher;

  void boot();
  void dispatch_http_request(int socket);
  void set_server_peer(struct sockaddr_in *addr);
  void set_client_peer(struct sockaddr_in *addr);

  Server(Server const &) = delete;
  Server &operator=(Server const &) = delete;
  Server(Server &&) = delete;
  Server &operator=(Server &&) = delete;
};
}