#pragma once
#include <memory>
#include <string>
#include "method.h"

namespace rapid {

/**
 HTTP Request class

 Request class cupsules HTTP requesting parameters and environment informations.
 */
class Request {
 public:
  /**
   Constructor with raw input via receiving packets.

   @param raw_input raw input via receiving packets
   */
  Request(const std::string &raw_input);

  /**
   Constructor with raw input via receiving packets, client address string, and
   client port number.

   @param raw_input raw input via receiving packets
   @param address client address
   @param port client port
   */
  Request(const std::string &raw_input, const std::string &address,
          uint16_t port);

  /**
   Destructor
   */
  ~Request();

  /**
   Return the HTTP Method
   */
  Method method() const { return _method; }

  /**
   Return the REQUEST_URI
   */
  std::string request_uri() const { return _request_uri; }

  /**
   Return the USER_AGENT
   */
  std::string user_agent() const { return _user_agent; }

  /**
   Return the REMOTE_HOST, it has to contain an ip address because RAPID doesn't
   resolv host name.
   */
  std::string remote_host() const { return _client_addr; }

  /**
   @see remote_host()
   */
  std::string remote_addr() const { return _client_addr; }

  /**
    Return the REMOTE_PORT
   */
  uint16_t remote_port() const { return _client_port; }

 private:
  class Impl;
  /** pimpl */
  std::unique_ptr<Impl> _impl;

  /** client address string */
  std::string _client_addr;

  /** client port */
  uint16_t _client_port;

  /** HTTP Method */
  Method _method = Method::Get;
  /** REQUEST_URI */
  std::string _request_uri;
  /** USER_AGENT */
  std::string _user_agent;
};
}