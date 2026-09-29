#pragma once

#include <cstdint>

enum class ProtocolId : std::uint16_t {
    Login = 1001,
    Move = 1002,
    Fight = 1003,
    LoginResponse = 2001,
    MoveResponse = 2002,
};
