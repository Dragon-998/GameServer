#pragma once

#include "Player.h"

#include <memory>
#include <string>
#include <unordered_map>

class PlayerManager {
public:
    Player& getOrCreate(PlayerId playerId, const std::string& name);

    [[nodiscard]] Player* find(PlayerId playerId) noexcept;
    [[nodiscard]] const Player* find(PlayerId playerId) const noexcept;

private:
    std::unordered_map<PlayerId, std::unique_ptr<Player>> players_;
};
