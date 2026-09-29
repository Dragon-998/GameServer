#include "MoveHandler.h"

#include <iostream>

void MoveHandler::handle(const Packet& packet, Session& session) {
    (void)packet;
    (void)session;
    std::cout << "MoveHandler received packet" << std::endl;
}
