#pragma once

#include "WorldTypes.h"

class Entity {
public:
    Entity(EntityId entityId, int x, int y);
    virtual ~Entity() = default;

    [[nodiscard]] EntityId entityId() const noexcept;
    [[nodiscard]] int x() const noexcept;
    [[nodiscard]] int y() const noexcept;
    [[nodiscard]] bool moveBy(int dx, int dy) noexcept;

private:
    EntityId entityId_;
    int x_;
    int y_;
};
