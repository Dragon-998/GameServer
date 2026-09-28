#include "TcpServer.h"

#include <exception>
#include <iostream>

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

        std::cout << "客户端连接后，按回车退出" << std::endl;
        std::cin.get();

        server.stop();
        return 0;
    } catch (const std::exception& exception) {
        std::cerr << "服务器启动失败: " << exception.what() << std::endl;
        return 1;
    }
}
