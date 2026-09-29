#include "Dispatcher.h"

#include "FightHandler.h"
#include "LoginHandler.h"
#include "MoveHandler.h"
#include "PlayerManager.h"
#include "PlayerStorage.h"
#include "ProtocolId.h"
#include "Session.h"
#include "World.h"

#include <iostream>
#include <utility>

Dispatcher::Dispatcher(PlayerManager& playerManager,
                       PlayerStorage& playerStorage, World& world) {
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Login),
                      std::make_unique<LoginHandler>(playerManager,
                                                     playerStorage, world));
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Move),
                      std::make_unique<MoveHandler>(playerManager, world));
    handlers_.emplace(static_cast<std::uint16_t>(ProtocolId::Fight),
                      std::make_unique<FightHandler>(playerManager, world));
}

void Dispatcher::dispatch(const Packet& packet, Session& session) const {
    const auto handler = handlers_.find(packet.protocolId);
    if (handler == handlers_.end()) {
        std::cout << "Unknown protocol id: " << packet.protocolId << std::endl;
        return;
    }

    handler->second->handle(packet, session);
}
