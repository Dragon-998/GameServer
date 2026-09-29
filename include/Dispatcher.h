#pragma once

#include "Handler.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

class Session;
class PlayerManager;
class PlayerStorage;

class Dispatcher {
public:
    Dispatcher(PlayerManager& playerManager, PlayerStorage& playerStorage);

    void dispatch(const Packet& packet, Session& session) const;

private:
    std::unordered_map<std::uint16_t, std::unique_ptr<Handler>> handlers_;
};
