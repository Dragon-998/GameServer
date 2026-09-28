#pragma once

#include <cstdint>
#include <vector>

struct Packet {
    std::uint16_t protocolId{0};
    std::vector<std::uint8_t> data;
};
