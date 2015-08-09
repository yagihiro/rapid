#pragma once
#include "handler.h"
#include "method.h"
#include "request.h"
#include "response.h"
#include "route.h"
#include "routing_dispatcher.h"

namespace rapid {

class Server {
 public:
  Server();
  ~Server() = default;

  void set_port(int port);
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
  static constexpr int kUndefinedPort = 0;

  int _port = kUndefinedPort;
  bool _stop_requested = false;
  bool _stopped = true;

  RoutingDispatcher _dispatcher;

  Server(Server const &) = delete;
  Server &operator=(Server const &) = delete;
  Server(Server &&) = delete;
  Server &operator=(Server &&) = delete;
};
}