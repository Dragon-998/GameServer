#pragma once

#include "Entity.h"
#include "PlayerId.h"

class PlayerEntity : public Entity {
public:
    PlayerEntity(EntityId entityId, PlayerId ownerPlayerId, int x, int y);

    [[nodiscard]] PlayerId ownerPlayerId() const noexcept;

private:
    PlayerId ownerPlayerId_;
};
