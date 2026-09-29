#include "Entity.h"
#include "Map.h"
#include "Player.h"
#include "PlayerEntity.h"
#include "World.h"

#include <iostream>

namespace {

bool verify(bool condition, const char* message) {
    if (!condition) {
        std::cerr << message << std::endl;
    }
    return condition;
}

}  // namespace

int main() {
    World world;
    Map* map = world.getMap(World::defaultMapId);
    if (!verify(map != nullptr, "World should create its default Map.")) {
        return 1;
    }

    Player player(10001, "test");
    if (!verify(!player.entityLocation().has_value(),
                "A new Player should not have an EntityLocation.") ||
        !verify(world.enterWorld(player), "Player failed to enter the World.")) {
        return 1;
    }

    const auto& location = player.entityLocation();
    if (!verify(location.has_value() &&
                    location->mapId == World::defaultMapId &&
                    location->entityId == EntityId{1},
                "Player did not save its Map and Entity IDs.")) {
        return 1;
    }

    Entity* entity = map->getEntity(location->entityId);
    auto* playerEntity = dynamic_cast<PlayerEntity*>(entity);
    if (!verify(playerEntity != nullptr &&
                    playerEntity->ownerPlayerId() == player.playerId() &&
                    playerEntity->x() == 0 && playerEntity->y() == 0,
                "Map lookup did not return the expected PlayerEntity.")) {
        return 1;
    }

    if (!verify(world.findPlayerEntity(player) == playerEntity,
                "Player's saved location did not find its PlayerEntity.") ||
        !verify(map->getEntity(99999) == nullptr,
                "Looking up an unknown EntityId should return nullptr.")) {
        return 1;
    }

    std::cout << "World tests passed." << std::endl;
    return 0;
}
