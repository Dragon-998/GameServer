#pragma once

#include "Player.h"

#include <memory>
#include <string>
#include <unordered_map>

class PlayerManager {
public:
    [[nodiscard]] Player* addPlayer(PlayerId playerId,
                                    const std::string& name);

    [[nodiscard]] Player* getPlayer(PlayerId playerId) noexcept;
    [[nodiscard]] const Player* getPlayer(PlayerId playerId) const noexcept;
    [[nodiscard]] bool hasPlayer(PlayerId playerId) const noexcept;

private:
    std::unordered_map<PlayerId, std::unique_ptr<Player>> players_;
};
