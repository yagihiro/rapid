#include <rapid/rapid.h>

namespace rapid {

Route::Route(Method method, const std::string &pattern, const Handler &handler)
    : _method(method), _pattern(pattern), _handler(handler) {}
}