#include "PlayerManager.h"

#include <memory>

Player* PlayerManager::addPlayer(PlayerId playerId, const std::string& name) {
    return addPlayer(std::make_unique<Player>(playerId, name));
}

Player* PlayerManager::addPlayer(std::unique_ptr<Player> player) {
    if (player == nullptr || hasPlayer(player->playerId())) {
        return nullptr;
    }

    const PlayerId playerId = player->playerId();
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
