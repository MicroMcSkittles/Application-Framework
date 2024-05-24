#pragma once
#include <memory>

// Returns a number with one bit fliped.
#define BIT(x) 1 << x
// Binds a event function.
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)