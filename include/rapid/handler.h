#pragma once
#include <functional>
#include "request.h"
#include "response.h"

namespace rapid {

/**
 * Routing handler type
 */
using Handler = std::function<Response(Request &)>;
}