#include "PlayerManager.h"

#include <memory>

Player& PlayerManager::getOrCreate(PlayerId playerId, const std::string& name) {
    const auto existing = players_.find(playerId);
    if (existing != players_.end()) {
        return *existing->second;
    }

    auto player = std::make_unique<Player>(playerId, name);
    Player& createdPlayer = *player;
    players_.emplace(playerId, std::move(player));
    return createdPlayer;
}

Player* PlayerManager::find(PlayerId playerId) noexcept {
    const auto found = players_.find(playerId);
    return found == players_.end() ? nullptr : found->second.get();
}

const Player* PlayerManager::find(PlayerId playerId) const noexcept {
    const auto found = players_.find(playerId);
    return found == players_.end() ? nullptr : found->second.get();
}
