#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(EntityId entityId, PlayerId ownerPlayerId, int x,
                           int y)
    : Entity(entityId, x, y), ownerPlayerId_(ownerPlayerId) {}

PlayerId PlayerEntity::ownerPlayerId() const noexcept {
    return ownerPlayerId_;
}
