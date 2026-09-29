#include "Session.h"

#include "PacketCodec.h"

#include <array>
#include <limits>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
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

int Session::Send(const std::vector<std::uint8_t>& bytes) {
    if (!connected_ || socket_ == kInvalidSocket) {
        return -1;
    }

    if (bytes.empty()) {
        return 0;
    }

    if (bytes.size() > static_cast<std::size_t>(
                           std::numeric_limits<int>::max())) {
        return -1;
    }

    std::size_t totalSent = 0;
    while (totalSent < bytes.size()) {
        int flags = 0;
#ifdef MSG_NOSIGNAL
        flags |= MSG_NOSIGNAL;
#endif
        const int bytesSent = send(
            socket_, reinterpret_cast<const char*>(bytes.data() + totalSent),
            static_cast<int>(bytes.size() - totalSent), flags);
        if (bytesSent <= 0) {
            close();
            return -1;
        }
        totalSent += static_cast<std::size_t>(bytesSent);
    }

    return static_cast<int>(totalSent);
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
