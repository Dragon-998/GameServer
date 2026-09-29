#include "Dispatcher.h"

#include "FightHandler.h"
#include "LoginHandler.h"
#include "MoveHandler.h"
#include "PlayerManager.h"
#include "ProtocolId.h"
#include "Session.h"

#include <iostream>
#include <utility>

Dispatcher::Dispatcher(PlayerManager& playerManager) {
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Login),
                      std::make_unique<LoginHandler>(playerManager));
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Move),
                      std::make_unique<MoveHandler>());
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Fight),
                      std::make_unique<FightHandler>());
}

void Dispatcher::dispatch(const Packet& packet, Session& session) const {
    const auto handler = handlers_.find(packet.protocolId);
    if (handler == handlers_.end()) {
        std::cout << "Unknown protocol id: " << packet.protocolId << std::endl;
        return;
    }

    handler->second->handle(packet, session);
}
