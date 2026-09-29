#include "Dispatcher.h"

#include "FightHandler.h"
#include "LoginHandler.h"
#include "MoveHandler.h"
#include "ProtocolId.h"

#include <iostream>
#include <utility>

Dispatcher::Dispatcher() {
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Login),
                      std::make_unique<LoginHandler>());
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Move),
                      std::make_unique<MoveHandler>());
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Fight),
                      std::make_unique<FightHandler>());
}

void Dispatcher::dispatch(const Packet& packet) const {
    const auto handler = handlers_.find(packet.protocolId);
    if (handler == handlers_.end()) {
        std::cout << "Unknown protocol id: " << packet.protocolId << std::endl;
        return;
    }

    handler->second->handle(packet);
}
