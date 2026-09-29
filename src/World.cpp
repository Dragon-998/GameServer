#include "World.h"

#include "DummyEnemy.h"
#include "Entity.h"
#include "Map.h"
#include "Player.h"
#include "PlayerEntity.h"

#include <iostream>
#include <memory>
#include <optional>
#include <utility>

World::World() {
    Map* map = createMap(defaultMapId);
    if (map != nullptr) {
        const EntityId entityId = map->generateEntityId();
        auto dummyEnemy = std::make_unique<DummyEnemy>(entityId, 1, 0);
        if (map->addEntity(std::move(dummyEnemy)) != nullptr) {
            std::cout << "Created training DummyEnemy " << entityId
                      << " at (1, 0)" << std::endl;
        }
    }
}

World::~World() = default;

Map* World::createMap(MapId mapId) {
    auto map = std::make_unique<Map>(mapId);
    const auto [storedMap, inserted] = maps_.emplace(mapId, std::move(map));
    if (!inserted) {
        return nullptr;
    }

    std::cout << "Map " << mapId << " created" << std::endl;
    return storedMap->second.get();
}

Map* World::getMap(MapId mapId) noexcept {
    const auto found = maps_.find(mapId);
    return found == maps_.end() ? nullptr : found->second.get();
}

const Map* World::getMap(MapId mapId) const noexcept {
    const auto found = maps_.find(mapId);
    return found == maps_.end() ? nullptr : found->second.get();
}

bool World::enterWorld(Player& player) {
    if (player.entityLocation().has_value()) {
        return findPlayerEntity(player) != nullptr;
    }

    Map* map = getMap(defaultMapId);
    if (map == nullptr) {
        std::cerr << "Default map is not available" << std::endl;
        return false;
    }

    constexpr int kSpawnX = 0;
    constexpr int kSpawnY = 0;
    const EntityId entityId = map->generateEntityId();
    auto playerEntity = std::make_unique<PlayerEntity>(
        entityId, player.playerId(), kSpawnX, kSpawnY);
    if (map->addEntity(std::move(playerEntity)) == nullptr) {
        std::cerr << "Failed to add PlayerEntity " << entityId << " to map "
                  << map->mapId() << std::endl;
        return false;
    }

    player.setEntityLocation(EntityLocation{map->mapId(), entityId});
    if (findPlayerEntity(player) == nullptr) {
        std::cerr << "Failed to find PlayerEntity " << entityId
                  << " after adding" << std::endl;
        return false;
    }

    std::cout << "Player " << player.playerId() << " entered map "
              << map->mapId() << std::endl;
    std::cout << "Created PlayerEntity " << entityId << " at (" << kSpawnX
              << ", " << kSpawnY << ")" << std::endl;
    return true;
}

PlayerEntity* World::findPlayerEntity(const Player& player) noexcept {
    const auto& location = player.entityLocation();
    if (!location.has_value()) {
        return nullptr;
    }

    Map* map = getMap(location->mapId);
    if (map == nullptr) {
        return nullptr;
    }

    auto* playerEntity = dynamic_cast<PlayerEntity*>(
        map->getEntity(location->entityId));
    if (playerEntity == nullptr ||
        playerEntity->ownerPlayerId() != player.playerId()) {
        return nullptr;
    }

    return playerEntity;
}
