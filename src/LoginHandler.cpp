#include "LoginHandler.h"

#include <iostream>

void LoginHandler::handle(const Packet& packet) {
    (void)packet;
    std::cout << "LoginHandler received packet" << std::endl;
}
