#pragma once

#include <iostream>

#define BIT(x) (1 << x)

#define O2D_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#define LOG(x) std::cout << "LOG: " << x << std::endl;