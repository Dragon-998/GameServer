#include "PlayerManager.h"

#include <memory>

Player* PlayerManager::addPlayer(PlayerId playerId, const std::string& name) {
    if (hasPlayer(playerId)) {
        return nullptr;
    }

    auto player = std::make_unique<Player>(playerId, name);
    const auto [insertedPlayer, inserted] =
        players_.emplace(playerId, std::move(player));
    return inserted ? insertedPlayer->second.get() : nullptr;
}

Player* PlayerManager::getPlayer(PlayerId playerId) noexcept {
    const auto found = players_.find(playerId);
    return found == players_.end() ? nullptr : found->second.get();
}

const Player* PlayerManager::getPlayer(PlayerId playerId) const noexcept {
    const auto found = players_.find(playerId);
    return found == players_.end() ? nullptr : found->second.get();
}

bool PlayerManager::hasPlayer(PlayerId playerId) const noexcept {
    return players_.find(playerId) != players_.end();
}
