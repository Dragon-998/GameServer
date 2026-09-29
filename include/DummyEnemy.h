#pragma once

#include "Entity.h"

class DummyEnemy final : public Entity {
public:
    DummyEnemy(EntityId entityId, int x, int y);
};
