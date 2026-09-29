#include "Map.h"

#include "Entity.h"

#include <utility>

Map::Map(MapId mapId) : mapId_(mapId) {}

Map::~Map() = default;

MapId Map::mapId() const noexcept {
    return mapId_;
}

EntityId Map::generateEntityId() noexcept {
    return nextEntityId_++;
}

Entity* Map::addEntity(std::unique_ptr<Entity> entity) {
    if (entity == nullptr) {
        return nullptr;
    }

    const EntityId entityId = entity->entityId();
    const auto [storedEntity, inserted] =
        entities_.emplace(entityId, std::move(entity));
    return inserted ? storedEntity->second.get() : nullptr;
}

Entity* Map::getEntity(EntityId entityId) noexcept {
    const auto found = entities_.find(entityId);
    return found == entities_.end() ? nullptr : found->second.get();
}

const Entity* Map::getEntity(EntityId entityId) const noexcept {
    const auto found = entities_.find(entityId);
    return found == entities_.end() ? nullptr : found->second.get();
}
