#include "FightHandler.h"

#include <iostream>

void FightHandler::handle(const Packet& packet) {
    (void)packet;
    std::cout << "FightHandler received packet" << std::endl;
}
