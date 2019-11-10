#pragma once

#include "std_e/base/macros.hpp"

namespace std_e {

constexpr const auto identity = [](auto&& x)->decltype(auto){ return FWD(x); };

} // std_e
