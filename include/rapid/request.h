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
   */
  Request(const std::string &raw_input);

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

 private:
  class Impl;
  /** pimpl */
  std::unique_ptr<Impl> _impl;

  /** HTTP Method */
  Method _method = Method::Get;
  /** REQUEST_URI */
  std::string _request_uri;
  /** USER_AGENT */
  std::string _user_agent;
};
}