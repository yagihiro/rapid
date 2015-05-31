#pragma once

namespace rapid {

class Server {
 public:
  Server();
  ~Server() = default;

  void set_port(int port);
  void run();
  void stop();

  bool stopped() const;

 private:
  static constexpr int kUndefinedPort = 0;

  int _port = kUndefinedPort;
  bool _stop_requested = false;
  bool _stopped = true;

  Server(Server const&) = delete;
  Server& operator=(Server const&) = delete;
  Server(Server&&) = delete;
  Server& operator=(Server&&) = delete;
};
}