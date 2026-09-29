#include "Entity.h"

Entity::Entity(EntityId entityId, int x, int y)
    : entityId_(entityId), x_(x), y_(y) {}

EntityId Entity::entityId() const noexcept {
    return entityId_;
}

int Entity::x() const noexcept {
    return x_;
}

int Entity::y() const noexcept {
    return y_;
}
