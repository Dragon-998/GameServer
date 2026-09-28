#include "Session.h"

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
