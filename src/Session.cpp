#include "Session.h"

#include "PacketCodec.h"

#include <array>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#endif

namespace {

void closeSocket(SocketHandle socket) noexcept {
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

}  // namespace

Session::Session(SocketHandle socket, std::uint64_t sessionId)
    : socket_(socket), sessionId_(sessionId), connected_(true) {}

Session::~Session() {
    close();
}

int Session::Recv(PacketCodec& packetCodec) {
    if (!connected_ || socket_ == kInvalidSocket) {
        return -1;
    }

    std::array<std::uint8_t, 4096> buffer{};
    const int bytesReceived = recv(
        socket_, reinterpret_cast<char*>(buffer.data()),
        static_cast<int>(buffer.size()), 0);
    if (bytesReceived == 0) {
        close();
        return 0;
    }

    if (bytesReceived < 0) {
        close();
        return -1;
    }

    packetCodec.append(buffer.data(), static_cast<std::size_t>(bytesReceived));
    return bytesReceived;
}

int Session::Send(const std::string& message) {
    if (!connected_ || socket_ == kInvalidSocket || message.empty()) {
        return message.empty() ? 0 : -1;
    }

    const int bytesSent = send(socket_, message.data(),
                               static_cast<int>(message.size()), 0);
    if (bytesSent < 0) {
        close();
        return -1;
    }

    return bytesSent;
}

void Session::close() noexcept {
    if (!connected_) {
        return;
    }

    if (socket_ != kInvalidSocket) {
        closeSocket(socket_);
        socket_ = kInvalidSocket;
    }

    connected_ = false;
}

SocketHandle Session::socket() const noexcept {
    return socket_;
}

std::uint64_t Session::sessionId() const noexcept {
    return sessionId_;
}

bool Session::connected() const noexcept {
    return connected_;
}
