#pragma once

#include "Handler.h"

class PlayerManager;
class World;

class MoveHandler : public Handler {
public:
    MoveHandler(PlayerManager& playerManager, World& world);

    void handle(const Packet& packet, Session& session) override;

private:
    PlayerManager& playerManager_;
    World& world_;
};
