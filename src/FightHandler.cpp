#include "FightHandler.h"

#include "Battle.h"
#include "Entity.h"
#include "FightMessage.h"
#include "Map.h"
#include "PacketCodec.h"
#include "Player.h"
#include "PlayerEntity.h"
#include "PlayerManager.h"
#include "ProtocolId.h"
#include "Session.h"
#include "Warrior.h"
#include "World.h"

#include <charconv>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <system_error>
#include <vector>

namespace {

bool parseFightRequest(const Packet& packet, FightRequest& request) {
    const std::string payload(packet.data.begin(), packet.data.end());
    constexpr std::string_view prefix = "targetEntityId=";
    if (payload.compare(0, prefix.size(), prefix) != 0) {
        return false;
    }

    const std::string_view value(payload.data() + prefix.size(),
                                 payload.size() - prefix.size());
    if (value.empty()) {
        return false;
    }

    const char* begin = value.data();
    const char* end = begin + value.size();
    const auto result =
        std::from_chars(begin, end, request.targetEntityId);
    return result.ec == std::errc{} && result.ptr == end &&
           request.targetEntityId != 0;
}

const char* eventTypeName(BattleEventType type) noexcept {
    switch (type) {
        case BattleEventType::RoundStart:
            return "round_start";
        case BattleEventType::Attack:
            return "attack";
        case BattleEventType::BattleEnd:
            return "battle_end";
    }
    return "unknown";
}

std::string serializeReport(const BattleReport& report) {
    std::ostringstream output;
    output << "success=1\nbattleId=" << report.battleId() << '\n';

    for (const BattleEvent& event : report.events()) {
        output << "event=" << eventTypeName(event.type)
               << "&round=" << event.round;
        if (event.type == BattleEventType::Attack) {
            output << "&attacker=" << event.attackerId
                   << "&target=" << event.targetId
                   << "&damage=" << event.damage
                   << "&remainingHp=" << event.remainingHp;
        } else if (event.type == BattleEventType::BattleEnd &&
                   event.winnerId.has_value()) {
            output << "&winner=" << *event.winnerId;
        }
        output << '\n';
    }

    return output.str();
}

void sendFightResponse(Session& session, const std::string& payload) {
    Packet response;
    response.protocolId =
        static_cast<std::uint16_t>(ProtocolId::FightResponse);
    response.data.assign(payload.begin(), payload.end());

    const std::vector<std::uint8_t> bytes = PacketCodec::encode(response);
    if (session.Send(bytes) < 0) {
        std::cerr << "Fight response send failed." << std::endl;
    }
}

void sendFightError(Session& session, const char* message) {
    sendFightResponse(session, std::string("success=0&message=") + message);
}

}  // namespace

FightHandler::FightHandler(PlayerManager& playerManager, World& world)
    : playerManager_(playerManager), world_(world) {}

void FightHandler::handle(const Packet& packet, Session& session) {
    std::cout << "FightHandler received packet" << std::endl;

    const std::optional<PlayerId> playerId = session.playerId();
    if (!playerId.has_value()) {
        sendFightError(session, "not logged in");
        return;
    }

    Player* player = playerManager_.getPlayer(*playerId);
    if (player == nullptr) {
        sendFightError(session, "player not found");
        return;
    }

    const auto& location = player->entityLocation();
    if (!location.has_value()) {
        sendFightError(session, "player not in world");
        return;
    }

    PlayerEntity* attackerEntity = world_.findPlayerEntity(*player);
    if (attackerEntity == nullptr) {
        sendFightError(session, "player entity not found");
        return;
    }

    FightRequest request;
    if (!parseFightRequest(packet, request)) {
        sendFightError(session, "invalid request format");
        return;
    }

    Map* map = world_.getMap(location->mapId);
    if (map == nullptr) {
        sendFightError(session, "map not found");
        return;
    }

    Entity* targetEntity = map->getEntity(request.targetEntityId);
    if (targetEntity == nullptr) {
        sendFightError(session, "target entity not found");
        return;
    }
    if (targetEntity->entityId() == attackerEntity->entityId()) {
        sendFightError(session, "cannot attack self");
        return;
    }

    std::optional<PlayerId> targetPlayerId;
    if (const auto* targetPlayerEntity =
            dynamic_cast<const PlayerEntity*>(targetEntity)) {
        targetPlayerId = targetPlayerEntity->ownerPlayerId();
        if (*targetPlayerId == *playerId) {
            sendFightError(session, "cannot attack self");
            return;
        }
    }

    Battle battle(nextBattleId_++);
    const bool attackerAdded = battle.addWarrior(
        Warrior(1, playerId, attackerEntity->entityId()));
    const bool targetAdded = battle.addWarrior(
        Warrior(2, targetPlayerId, targetEntity->entityId()));
    if (!attackerAdded || !targetAdded || !battle.start()) {
        sendFightError(session, "failed to start battle");
        return;
    }

    sendFightResponse(session, serializeReport(battle.report()));
}
