#include "Entity.h"

#include <limits>

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

bool Entity::moveBy(int dx, int dy) noexcept {
    if ((dx > 0 && x_ > std::numeric_limits<int>::max() - dx) ||
        (dx < 0 && x_ < std::numeric_limits<int>::min() - dx) ||
        (dy > 0 && y_ > std::numeric_limits<int>::max() - dy) ||
        (dy < 0 && y_ < std::numeric_limits<int>::min() - dy)) {
        return false;
    }

    x_ += dx;
    y_ += dy;
    return true;
}
