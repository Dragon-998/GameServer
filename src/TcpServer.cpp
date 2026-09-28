#include "TcpServer.h"

#include <cerrno>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

namespace {

constexpr std::uint16_t kServerPort = 9000;

#ifdef _WIN32
using socket_type = SOCKET;
constexpr socket_type invalid_socket = INVALID_SOCKET;
#else
using socket_type = int;
constexpr socket_type invalid_socket = -1;
#endif

void closeSocket(socket_type socket) noexcept {
#ifdef _WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}

std::runtime_error socketError(const char* operation) {
#ifdef _WIN32
    return std::runtime_error(std::string(operation) + " failed, error=" +
                              std::to_string(WSAGetLastError()));
#else
    return std::runtime_error(std::string(operation) + " failed: " +
                              std::strerror(errno));
#endif
}

std::string clientAddress(const sockaddr_in& address) {
    char ip[INET_ADDRSTRLEN]{};
    if (inet_ntop(AF_INET, &address.sin_addr, ip, sizeof(ip)) == nullptr) {
        return "unknown address";
    }
    return std::string(ip) + ":" + std::to_string(ntohs(address.sin_port));
}

}  // namespace

TcpServer::TcpServer() {
#ifdef _WIN32
    WSADATA data{};
    const int result = WSAStartup(MAKEWORD(2, 2), &data);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed, error=" +
                                 std::to_string(result));
    }
#endif
}

TcpServer::~TcpServer() {
    stop();
#ifdef _WIN32
    WSACleanup();
#endif
}

void TcpServer::start() {
    if (server_fd_ != invalid_socket) {
        throw std::runtime_error("server is already started");
    }

    server_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd_ == invalid_socket) {
        throw socketError("socket");
    }

    try {
        int reuseAddress = 1;
#ifdef _WIN32
        const char* optionValue = reinterpret_cast<const char*>(&reuseAddress);
#else
        const void* optionValue = &reuseAddress;
#endif
        if (setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, optionValue,
                       sizeof(reuseAddress)) != 0) {
            throw socketError("setsockopt");
        }

        sockaddr_in serverAddress{};
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(kServerPort);

        if (bind(server_fd_, reinterpret_cast<const sockaddr*>(&serverAddress),
                 sizeof(serverAddress)) != 0) {
            throw socketError("bind");
        }

        if (listen(server_fd_, SOMAXCONN) != 0) {
            throw socketError("listen");
        }

        std::cout << "服务器已监听 0.0.0.0:9000，等待客户端连接..." << std::endl;

        sockaddr_in clientAddressValue{};
#ifdef _WIN32
        int clientAddressLength = sizeof(clientAddressValue);
#else
        socklen_t clientAddressLength = sizeof(clientAddressValue);
#endif
        client_fd_ = accept(server_fd_,
                            reinterpret_cast<sockaddr*>(&clientAddressValue),
                            &clientAddressLength);
        if (client_fd_ == invalid_socket) {
            throw socketError("accept");
        }

        std::cout << "客户端已连接: " << clientAddress(clientAddressValue)
                  << std::endl;
    } catch (...) {
        stop();
        throw;
    }
}

void TcpServer::stop() {
    const bool hadSocket = client_fd_ != invalid_socket ||
                           server_fd_ != invalid_socket;

    if (client_fd_ != invalid_socket) {
        closeSocket(client_fd_);
        client_fd_ = invalid_socket;
    }

    if (server_fd_ != invalid_socket) {
        closeSocket(server_fd_);
        server_fd_ = invalid_socket;
    }

    if (hadSocket) {
        std::cout << "服务器停止" << std::endl;
    }
}
