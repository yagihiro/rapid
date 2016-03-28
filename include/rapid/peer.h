#pragma once
#include <string>

namespace rapid {

/**
 Client and Server address/port pairs
 */
using Peer = struct _Peer {
  std::string client_addr;
  uint16_t client_port;
  std::string server_addr;
  uint16_t server_port;
};
}