#pragma once

#include "WorldTypes.h"

#include <memory>
#include <unordered_map>

class Entity;

class Map {
public:
    explicit Map(MapId mapId);
    ~Map();

    [[nodiscard]] MapId mapId() const noexcept;
    [[nodiscard]] EntityId generateEntityId() noexcept;
    [[nodiscard]] Entity* addEntity(std::unique_ptr<Entity> entity);

    [[nodiscard]] Entity* getEntity(EntityId entityId) noexcept;
    [[nodiscard]] const Entity* getEntity(EntityId entityId) const noexcept;

private:
    MapId mapId_;
    EntityId nextEntityId_{1};
    std::unordered_map<EntityId, std::unique_ptr<Entity>> entities_;
};
