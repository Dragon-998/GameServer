#include "TcpServer.h"

#include "PacketCodec.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {

void configureConsoleEncoding() {
#ifdef _WIN32
    // Source files are compiled as UTF-8; make Windows consoles decode output the same way.
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
}

}  // namespace

int main() {
    configureConsoleEncoding();

    try {
        TcpServer server;

        server.start();

        Session* session = server.session();
        PacketCodec packetCodec;
        if (session != nullptr && session->connected()) {
            while (session->connected()) {
                const int bytesReceived = session->Recv(packetCodec);
                if (bytesReceived <= 0) {
                    break;
                }

                const std::vector<Packet> packets = packetCodec.decode();
                for (const Packet& packet : packets) {
                    const std::string text(packet.data.begin(),
                                           packet.data.end());
                    std::cout << "protocolId: " << packet.protocolId << '\n'
                              << "data: " << text << std::endl;
                }
            }
        }

        std::cout << "客户端连接后，按回车退出" << std::endl;
        std::cin.get();

        server.stop();
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "服务器启动失败: " << exception.what() << std::endl;
        return 1;
    }
}
