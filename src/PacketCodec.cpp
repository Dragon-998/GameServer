#include "PacketCodec.h"

#include <stdexcept>
#include <utility>

namespace {

std::uint32_t readUint32NetworkOrder(const std::uint8_t* bytes) {
    return (static_cast<std::uint32_t>(bytes[0]) << 24) |
           (static_cast<std::uint32_t>(bytes[1]) << 16) |
           (static_cast<std::uint32_t>(bytes[2]) << 8) |
           static_cast<std::uint32_t>(bytes[3]);
}

std::uint16_t readUint16NetworkOrder(const std::uint8_t* bytes) {
    return static_cast<std::uint16_t>(
        (static_cast<std::uint16_t>(bytes[0]) << 8) | bytes[1]);
}

}  // namespace

void PacketCodec::append(const std::uint8_t* data, std::size_t size) {
    if (data == nullptr || size == 0) {
        return;
    }

    buffer_.insert(buffer_.end(), data, data + size);
}

std::vector<Packet> PacketCodec::decode() {
    std::vector<Packet> packets;

    while (buffer_.size() >= headerSize) {
        const auto* header = buffer_.data();
        const std::uint32_t dataSize = readUint32NetworkOrder(header);
        if (dataSize > maxDataSize) {
            throw std::runtime_error("Packet data is larger than 1 MiB");
        }

        const std::size_t packetSize = headerSize + dataSize;
        if (buffer_.size() < packetSize) {
            break;
        }

        Packet packet;
        packet.protocolId = readUint16NetworkOrder(header + sizeof(std::uint32_t));
        packet.data.assign(buffer_.begin() + headerSize,
                           buffer_.begin() + packetSize);
        packets.push_back(std::move(packet));

        buffer_.erase(buffer_.begin(), buffer_.begin() + packetSize);
    }

    return packets;
}
