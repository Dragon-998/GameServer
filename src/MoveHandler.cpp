#include "MoveHandler.h"

#include <iostream>

void MoveHandler::handle(const Packet& packet) {
    (void)packet;
    std::cout << "MoveHandler received packet" << std::endl;
}
