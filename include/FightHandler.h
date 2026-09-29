#pragma once

#include "Handler.h"
#include "BattleReport.h"

class PlayerManager;
class World;

class FightHandler : public Handler {
public:
    FightHandler(PlayerManager& playerManager, World& world);

    void handle(const Packet& packet, Session& session) override;

private:
    PlayerManager& playerManager_;
    World& world_;
    BattleId nextBattleId_{1};
};
