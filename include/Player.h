#pragma once

#include "PlayerId.h"

#include <string>

class Player {
public:
    Player(PlayerId playerId, std::string name);

    [[nodiscard]] PlayerId playerId() const noexcept;
    [[nodiscard]] const std::string& name() const noexcept;

private:
    PlayerId playerId_;
    std::string name_;
};
