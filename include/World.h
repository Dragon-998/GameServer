#pragma once

#include "WorldTypes.h"

#include <memory>
#include <unordered_map>

class Map;
class Player;
class PlayerEntity;

class World {
public:
    static constexpr MapId defaultMapId = 1;

    World();
    ~World();

    [[nodiscard]] Map* createMap(MapId mapId);
    [[nodiscard]] Map* getMap(MapId mapId) noexcept;
    [[nodiscard]] const Map* getMap(MapId mapId) const noexcept;

    [[nodiscard]] bool enterWorld(Player& player);
    [[nodiscard]] PlayerEntity* findPlayerEntity(const Player& player) noexcept;

private:
    std::unordered_map<MapId, std::unique_ptr<Map>> maps_;
};
