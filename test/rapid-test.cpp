#include <cstdlib>
#include <iostream>
#include <rapid/rapid.h>
#include <signal.h>
#include <thread>

static sig_atomic_t _received_sigint = 0;

// Ctrl-C
static void on_sigint(int sig) { _received_sigint = 1; }

int main(int argc, char *argv[]) {
  if (signal(SIGINT, on_sigint) == SIG_ERR) {
    return EXIT_FAILURE;
  }

  rapid::Server server;
  server.set_port(8080);
  server.get("/", [](const rapid::Request &req) -> rapid::ResponsePtr {
    rapid::Response response;

    response.set_status_code(200);
    response.set_headers({{"Content-Type", "application/json"}});
    response.set_body("{'key':'value'}");

    return std::make_shared<rapid::Response>(response);
  });
  server.run();

  do {
    std::this_thread::sleep_for(std::chrono::milliseconds(32));
  } while (_received_sigint == 0);

  return EXIT_SUCCESS;
}