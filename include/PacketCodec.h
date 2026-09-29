#pragma once

#include "Packet.h"

#include <cstddef>
#include <cstdint>
#include <vector>

class PacketCodec {
public:
    static constexpr std::size_t headerSize = sizeof(std::uint32_t) +
                                               sizeof(std::uint16_t);
    static constexpr std::uint32_t maxDataSize = 1024 * 1024;

    void append(const std::uint8_t* data, std::size_t size);
    [[nodiscard]] std::vector<Packet> decode();
    [[nodiscard]] static std::vector<std::uint8_t> encode(const Packet& packet);

private:
    std::vector<std::uint8_t> buffer_;
};
