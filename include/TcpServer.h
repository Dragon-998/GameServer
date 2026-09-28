#pragma once

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <winsock2.h>
#else
using socket_type = int;
constexpr socket_type invalid_socket = -1;
#endif

class TcpServer {
public:
    TcpServer();
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void start();
    void stop();

private:
#ifdef _WIN32
    SOCKET server_fd_{INVALID_SOCKET};
    SOCKET client_fd_{INVALID_SOCKET};
#else
    socket_type server_fd_{invalid_socket};
    socket_type client_fd_{invalid_socket};
#endif
};
