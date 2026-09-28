#pragma once

#include <cstdint>
#include <string>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
using SocketHandle = SOCKET;
constexpr SocketHandle kInvalidSocket = INVALID_SOCKET;
#else
using SocketHandle = int;
constexpr SocketHandle kInvalidSocket = -1;
#endif

class Session {
public:
    Session(SocketHandle socket, std::uint64_t sessionId);
    ~Session();

    Session(const Session&) = delete;
    Session& operator=(const Session&) = delete;

    int Recv(char* buffer, int bufferSize);
    int Send(const std::string& message);
    void close() noexcept;

    [[nodiscard]] SocketHandle socket() const noexcept;
    [[nodiscard]] std::uint64_t sessionId() const noexcept;
    [[nodiscard]] bool connected() const noexcept;

private:
    SocketHandle socket_{kInvalidSocket};
    std::uint64_t sessionId_{0};
    bool connected_{false};
};
