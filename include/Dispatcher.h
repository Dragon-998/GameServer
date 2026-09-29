#pragma once

#include "Handler.h"

#include <cstdint>
#include <memory>
#include <unordered_map>

class Session;
class PlayerManager;

class Dispatcher {
public:
    explicit Dispatcher(PlayerManager& playerManager);

    void dispatch(const Packet& packet, Session& session) const;

private:
    std::unordered_map<std::uint16_t, std::unique_ptr<Handler>> handlers_;
};
