#pragma once

#include <cstdint>

using MapId = std::uint32_t;
using EntityId = std::uint64_t;

struct EntityLocation {
    MapId mapId;
    EntityId entityId;
};
