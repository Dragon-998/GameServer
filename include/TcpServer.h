#pragma once

#include "Session.h"

#include <cstdint>
#include <memory>

class TcpServer {
public:
    TcpServer();
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    void start();
    void stop();

private:
    SocketHandle server_fd_{kInvalidSocket};
    SocketHandle client_fd_{kInvalidSocket};
    std::unique_ptr<Session> session_;
    std::uint64_t nextSessionId_{1};
};
