#pragma once

#include "PlayerId.h"
#include "WorldTypes.h"

#include <optional>
#include <string>

class Player {
public:
    Player(PlayerId playerId, std::string name);

    [[nodiscard]] PlayerId playerId() const noexcept;
    [[nodiscard]] const std::string& name() const noexcept;
    void setName(std::string name);

    [[nodiscard]] const std::optional<EntityLocation>& entityLocation()
        const noexcept;
    void setEntityLocation(EntityLocation location) noexcept;

private:
    PlayerId playerId_;
    std::string name_;
    std::optional<EntityLocation> entityLocation_;
};
