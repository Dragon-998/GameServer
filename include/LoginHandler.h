#pragma once

#include "Handler.h"

class PlayerManager;
class PlayerStorage;
class World;

class LoginHandler : public Handler {
public:
    LoginHandler(PlayerManager& playerManager, PlayerStorage& playerStorage,
                 World& world);

    void handle(const Packet& packet, Session& session) override;

private:
    PlayerManager& playerManager_;
    PlayerStorage& playerStorage_;
    World& world_;
};
