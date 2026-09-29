#include "FightHandler.h"

#include <iostream>

void FightHandler::handle(const Packet& packet, Session& session) {
    (void)packet;
    (void)session;
    std::cout << "FightHandler received packet" << std::endl;
}
