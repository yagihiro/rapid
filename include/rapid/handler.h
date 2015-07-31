#pragma once
#include <functional>
#include <memory>
#include "request.h"
#include "response.h"

namespace rapid {

/**
 * Routing handler type
 */
using Handler = std::function<ResponsePtr(const Request &)>;
}